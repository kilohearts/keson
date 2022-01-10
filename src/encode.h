#pragma once

#include <cstdint>
#include <string>
#include <ostream>

#include "conf.h"

namespace keson
{
    static const uint32_t Flag_PRETTY_PRINT         = (1 << 0);
    static const uint32_t Flag_QUOTE_KEYS           = (1 << 1);
    static const uint32_t Flag_QUOTE_STRING_VALUES  = (1 << 2);
    static const uint32_t Flag_QUOTE_NUMERIC_VALUES = (1 << 3);
    static const uint32_t Flag_QUOTE_BOOLEAN_VALUES = (1 << 4);
    static const uint32_t Flag_PREFER_SINGLE_QUOTES = (1 << 5);
    static const uint32_t Flag_FLEXIBLE_QUOTES      = (1 << 6);
    static const uint32_t Flag_EXTENDED_SAFE_CHARS  = (1 << 7);
    static const uint32_t Flag_CRLF_NEWLINES        = (1 << 8);
    static const uint32_t Flag_INDENT_WITH_SPACES   = (1 << 9);

    static const uint32_t Flags_JSON_STYLE_QUOTES = Flag_QUOTE_KEYS | Flag_QUOTE_STRING_VALUES;
    static const uint32_t Flags_PTREE_STYLE_QUOTES = Flag_QUOTE_KEYS | Flag_QUOTE_STRING_VALUES | Flag_QUOTE_NUMERIC_VALUES | Flag_QUOTE_BOOLEAN_VALUES;
    static const uint32_t Flags_RELAXED_QUOTES = Flag_FLEXIBLE_QUOTES | Flag_EXTENDED_SAFE_CHARS;

    class Writer;
    class Node;

    class MapWriter {
    public:
        ~MapWriter();

        Writer& operator[](const std::string& key);

    private:
        friend class Writer;

        MapWriter(Writer* parent);

        Writer* _parent;
        bool _first = true;
    };

    class VectorWriter {
    public:
        ~VectorWriter();

        Writer& next();
        
    private:
        friend class Writer;

        VectorWriter(Writer* parent);

        Writer* _parent;
        bool _first = true;
    };

    class Writer {
    public:
        Writer(std::ostream& stream, uint32_t flags = 0);

        VectorWriter vector();

        MapWriter map();

        void operator=(const std::string& value);;
        void operator=(int8_t value);
        void operator=(uint8_t value);
        void operator=(int16_t value);
        void operator=(uint16_t value);
        void operator=(int32_t value);
        void operator=(uint32_t value);
        void operator=(int64_t value);
        void operator=(uint64_t value);
        void operator=(float value);
        void operator=(double value);
        void operator=(bool value);
#if KESON_ENABLE_WSTRING
        void operator=(const std::wstring& value);
#endif

        void printNewline();

        void printComment(const std::string& value);
#if KESON_ENABLE_WSTRING
        void printComment(const std::wstring& value);
#endif

    private:
        friend class MapWriter;
        friend class VectorWriter;

        void printKey(const std::string& value);
        
        void printString(const std::string& value);
        
        void printNumeric(const std::string& value);
        
        void printBoolean(const std::string& value);

        bool isClean(const std::string& value);
        
        void printCleanIfPossible(const std::string& value);

        void printClean(const std::string& value);
        
        char decideQuoteChar(const std::string& value);

        void printQuoted(const std::string& value);

        std::ostream& _stream;
        uint32_t _flags;
        uint32_t _depth;
    };

    void encode(Writer& w, const Node& node);

    void encode(std::ostream& s, const Node& node, uint32_t flags = 0);

    std::string encode(const Node& node, uint32_t flags = 0);
}
