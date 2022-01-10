#include "encode.h"
#include "node.h"
#include "util.h"

#include <cassert>
#include <sstream>

namespace keson
{
    inline MapWriter::~MapWriter() {
        _parent->_depth -= 1;
        if (!_first) {
            _parent->printNewline();
        }
        _parent->_stream << "}";
    }

    Writer& MapWriter::operator[](const std::string& key) {
        if (!_first) {
            _parent->_stream << ",";
        }
        _first = false;
        _parent->printNewline();
        _parent->printKey(key);
        return *_parent;
    }

    inline MapWriter::MapWriter(Writer* parent) : _parent(parent) {
        _parent->_stream << "{";
        _parent->_depth += 1;
    }

    inline VectorWriter::~VectorWriter() {
        _parent->_depth -= 1;
        if (!_first) {
            _parent->printNewline();
        }
        _parent->_stream << "]";
    }

    inline Writer& VectorWriter::next() {
        if (!_first) {
            _parent->_stream << ",";
        }
        _first = false;
        _parent->printNewline();
        return *_parent;
    }

    inline VectorWriter::VectorWriter(Writer* parent) : _parent(parent) {
        _parent->_stream << "[";
        _parent->_depth += 1;
    }

    inline Writer::Writer(std::ostream& stream, uint32_t flags)
        : _stream(stream)
        , _flags(flags)
        , _depth(0)
    { }

    inline VectorWriter Writer::vector() {
        return VectorWriter(this);
    }

    inline MapWriter Writer::map() {
        return MapWriter(this);
    }
    
    void Writer::operator=(const std::string& value)  { printString(value); }
    void Writer::operator=(int8_t value)              { printNumeric(to_string(value)); }
    void Writer::operator=(uint8_t value)             { printNumeric(to_string(value)); }
    void Writer::operator=(int16_t value)             { printNumeric(to_string(value)); }
    void Writer::operator=(uint16_t value)            { printNumeric(to_string(value)); }
    void Writer::operator=(int32_t value)             { printNumeric(to_string(value)); }
    void Writer::operator=(uint32_t value)            { printNumeric(to_string(value)); }
    void Writer::operator=(int64_t value)             { printNumeric(to_string(value)); }
    void Writer::operator=(uint64_t value)            { printNumeric(to_string(value)); }
    void Writer::operator=(float value)               { printNumeric(to_string(value)); }
    void Writer::operator=(double value)              { printNumeric(to_string(value)); }
    void Writer::operator=(bool value)                { printBoolean(to_string(value)); }
#if KESON_ENABLE_WSTRING
    void Writer::operator=(const std::wstring& value) { printString(to_utf8(value)); }
#endif

    inline void Writer::printNewline() {
        if ((_flags & Flag_PRETTY_PRINT) == 0) { return; }
        if ((_flags & Flag_CRLF_NEWLINES) != 0) {
            _stream << "\r\n";
        }
        else {
            _stream << '\n';
        }
        for (uint32_t i = 0; i < _depth; i++) {
            if ((_flags & Flag_INDENT_WITH_SPACES) != 0) {
                _stream << "    ";
            }
            else {
                _stream << '\t';
            }
        }
    }

    void Writer::printComment(const std::string& value)
    {
        if ((_flags & Flag_PRETTY_PRINT) == 0) { return; }
        _stream << "/* " << value << " */";
    }

#if KESON_ENABLE_WSTRING
    void Writer::printComment(const std::wstring& value) { printComment(to_utf8(value)); }
#endif

    inline void Writer::printKey(const std::string& value) {
        if ((_flags & Flag_QUOTE_KEYS) != 0) {
            printQuoted(value);
        } else {
            printCleanIfPossible(value);
        }

        if ((_flags & Flag_PRETTY_PRINT) != 0) {
            _stream << ": ";
        } else {
            _stream << ':';
        }
    }

    inline void Writer::printString(const std::string& value) {
        if ((_flags & Flag_QUOTE_STRING_VALUES) != 0) {
            printQuoted(value);
        }
        else {
            printCleanIfPossible(value);
        }
    }

    inline void Writer::printNumeric(const std::string& value) {
        if ((_flags & Flag_QUOTE_NUMERIC_VALUES) != 0) {
            printQuoted(value);
        }
        else {
            printClean(value);
        }
    }

    inline void Writer::printBoolean(const std::string& value) {
        if ((_flags & Flag_QUOTE_BOOLEAN_VALUES) != 0) {
            printQuoted(value);
        }
        else {
            printClean(value);
        }
    }

    inline bool Writer::isClean(const std::string& value) {
        if (value.empty()) { return false; }
        const char EXTENDED_SAFE_CHARS[] = "-+=.@$/\\?!#*";
        bool allowExtended = ((_flags & Flag_EXTENDED_SAFE_CHARS) != 0);
        bool commentStarting = false;
        for (char c : value) {
            bool ucase = ('A' <= c && c <= 'Z');
            bool lcase = ('a' <= c && c <= 'z');
            bool digit = ('0' <= c && c <= '9');
            bool underscore = (c == '_');
            bool extended = false;
            if (allowExtended) {
                extended = strchr(EXTENDED_SAFE_CHARS, c) != nullptr;
                if (commentStarting && (c == '/' || c == '*')) {
                    return false;
                }
                commentStarting = (c == '/');
            }
            if (!ucase && !lcase && !digit && !underscore && !extended) {
                return false;
            }
        }
        return true;
    }

    inline void Writer::printCleanIfPossible(const std::string& value) {
        if (isClean(value)) {
            printClean(value);
        }
        else {
            printQuoted(value);
        }
    }

    inline void Writer::printClean(const std::string& value) {
        _stream << value;
    }

    inline char Writer::decideQuoteChar(const std::string& value) {
        char preferredQuotes = ((_flags & Flag_PREFER_SINGLE_QUOTES) != 0) ? '\'' : '"';
        if ((_flags & Flag_FLEXIBLE_QUOTES) == 0) {
            return preferredQuotes;
        }

        bool seenSingle = false;
        bool seenDouble = false;

        for (char c : value) {
            seenSingle |= (c == '\'');
            seenDouble |= (c == '"');
            if (seenSingle && seenDouble) {
                return preferredQuotes;
            }
        }

        if (!seenSingle && !seenDouble) {
            return preferredQuotes;
        } if (seenSingle) {
            return '"';
        }
        else {
            assert(seenDouble);
            return '\'';
        }
    }

    inline void Writer::printQuoted(const std::string& value) {
        static const char HEX[] = "0123456789abcdef";
        char quoteChar = decideQuoteChar(value);
        _stream << quoteChar;
        for (unsigned char c : value) {
            if (c == quoteChar) {
                _stream.put('\\');
                _stream.put(quoteChar);
            } else if (c == '\\') {
                _stream.put('\\');
                _stream.put('\\');
            } else if (c == '\n') {
                _stream.put('\\');
                _stream.put('n');
            } else if (c == '\r') {
                _stream.put('\\');
                _stream.put('r');
            } else if (c == '\v') {
                _stream.put('\\');
                _stream.put('v');
            } else if (c == '\t') {
                _stream.put('\\');
                _stream.put('t');
            } else if (c == '\b') {
                _stream.put('\\');
                _stream.put('b');
            } else if (c == '\f') {
                _stream.put('\\');
                _stream.put('f');
            } else if (c < 32 || c == 127) {
                _stream.put('\\');
                _stream.put('x');
                _stream.put(HEX[c >> 4]);
                _stream.put(HEX[c & 15]);
            } else {
                _stream.put(c);
            }
        }
        _stream << quoteChar;
    }


    void encode(Writer& w, const Node& node) {
        if (node.isAtom()) {
            w = node.atom();
        }
        else if (node.isVector()) {
            auto cw = w.vector();
            for (auto& child : node) {
                encode(cw.next(), child);
            }
        }
        else if (node.isMap()) {
            auto cw = w.map();
            for (auto& keyChild : node.map()) {
                if (!keyChild.second.isNull()) {
                    encode(cw[keyChild.first], keyChild.second);
                }
            }
        }
        else {
            assert(node.isNull());
            w = "null";
        }
    }

    void encode(std::ostream& s, const Node& node, uint32_t flags) {
        Writer w(s, flags);
        encode(w, node);

        if ((flags & Flag_PRETTY_PRINT) != 0)
        {
            w.printNewline();
        }
    }

    std::string encode(const Node& node, uint32_t flags) {
        std::ostringstream oss;
        encode(oss, node, flags);
        return oss.str();
    }
}
