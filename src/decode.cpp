#include "decode.h"
#include "node.h"

#include <sstream>
#include <charconv>
#include <cassert>

namespace keson
{
    class Parser
    {
    public:
        Parser(std::istream& stream)
            : _stream(stream)
        { }

        Node parseNode()
        {
            skipAir();
            switch (peek())
            {
            case '{':
                return parseMap();
            case '[':
                return parseVector();
            default:
                return parseAtom();
            }
        }

    private:
        uint32_t parseHex(const char* chars, int len)
        {
            uint32_t v;
            auto result = std::from_chars(chars, chars + len, v, 16);
            if (result.ec != (std::errc)0 || result.ptr != chars + len)
            {
                throw ParseError("Internal error when parsing hex");
            }
            return v;
        }

        std::string encodeUtf8(uint32_t v)
        {
            if (v < 0x7f)
            {
                return std::string(1, (char)v);
            }
            
            if (v < 0x7ff)
            {
                char b[] = {
                    0b1100'0000 | (char)(v >> 6),
                    0b1000'0000 | (char)(v & 0b0011'1111)
                };
                return std::string(b, b + sizeof(b) / sizeof(char));
            }
            
            if (v < 0xffff)
            {
                char b[] = {
                    0b1110'0000 | (char)(v >> 12),
                    0b1000'0000 | (char)((v >> 6) & 0b0011'1111),
                    0b1000'0000 | (char)(v & 0b0011'1111)
                };
                return std::string(b, b + sizeof(b) / sizeof(char));
            }
            
            if (v < 0x10ffff)
            {
                char b[] = {
                    0b1111'0000 | (char)(v >> 18),
                    0b1000'0000 | (char)((v >> 12) & 0b0011'1111),
                    0b1000'0000 | (char)((v >> 6) & 0b0011'1111),
                    0b1000'0000 | (char)(v & 0b0011'1111)
                };
                return std::string(b, b + sizeof(b) / sizeof(char));
            }
            
            throw ParseError("Unicode out of range.");
        }

        std::string parseHexEscape(int len)
        {
            assert(len <= 4);
            char chars[4];
            for (int i = 0; i < len; i++)
            {
                char c = next();
                if (!isHexDigit(c))
                {
                    throw ParseError("Expected hex digit");
                }
                chars[i] = c;
            }

            return encodeUtf8((char32_t)parseHex(chars, len));
        }

        std::string parseDelimitedHexEscape()
        {
            expect('{');
            std::string chars;
            while (true)
            {
                if (peek() == '}')
                {
                    next();
                    break;
                }
                else if (isHexDigit(peek()))
                {
                    if (chars.length() >= 6)
                    {
                        throw ParseError("Unicode escape sequence too long");
                    }
                    chars += next();
                }
                else
                {
                    throw ParseError("Expected hex digit");
                }
            }

            return encodeUtf8((char32_t)parseHex(chars.c_str(), chars.size()));
        }

        std::string parseEscape()
        {
            switch (peek())
            {
            case '0':
                return "\0";
            case '\'':
                return "\'";
            case '"':
                return "\"";
            case '\\':
                return "\\";
            case 'n':
                return "\n";
            case 'r':
                return "\r";
            case 'v':
                return "\v";
            case 't':
                return "\t";
            case 'b':
                return "\b";
            case 'f':
                return "\f";
            case 'u':
                next();
                return peek() == '{' ? parseDelimitedHexEscape() : parseHexEscape(4);
            case 'x':
                next();
                return parseHexEscape(2);
            }
            throw ParseError("Invalid escape sequence");
        }

        std::string parseAtom()
        {
            std::string result;
            char delimiter = 0;
            
            if (peek() == '"' || peek() == '\'')
            {
                delimiter = next();
            }

            while (true)
            {
                if (delimiter != 0)
                {
                    if (peek() == delimiter)
                    {
                        next();
                        break;
                    }
                }
                else if (isNakedDelimiter(peek()))
                {
                    break;
                }

                char c = next();

                if (delimiter != 0 && c == '\\')
                {
                    result += parseEscape();
                }
                else
                {
                    result += c;
                }
            }

            return result;
        }

        Node parseMap()
        {
            expect('{');
            
            Node result;
            std::string key;
            
            while (true) {
                skipAir();
                switch (peek())
                {
                case '}':
                    next();
                    return result;
                case ',':
                    next();
                    break;
                default:
                    key = parseAtom();
                    skipAir();
                    expect('=', ':');
                    result[std::move(key)] = parseNode();
                    break;
                }
            }
        }

        Node parseVector()
        {
            expect('[');

            Node result;

            while (true) {
                skipAir();
                switch (peek())
                {
                case ']':
                    next();
                    return result;
                case ',':
                    next();
                    break;
                default:
                    result.push_back(parseNode());
                    break;
                }
            }
        }

        int peek()
        {
            return _stream.peek();
        }

        char next()
        {
            int c = _stream.get();
            if (isEOF(c))
            {
                throw ParseError("Unexpected end of file");
            }
            return (char)c;
        }

        void expect(char c)
        {
            if (peek() != c)
            {
                throw ParseError(std::string("Expected '") + c + "'");
            }
            next();
        }

        void expect(char a, char b)
        {
            int c = peek();
            if (c != a && c != b)
            {
                throw ParseError(std::string("Expected '") + a + "' or '" + b + "'");
            }
            next();
        }

        bool isWhitespace(int c)
        {
            return c == ' ' || c == '\t' || c == '\n' || c == '\r';
        }

        bool isHexDigit(int c)
        {
            return c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F' || c >= '0' && c <= '9';
        }

        bool isEOF(int c)
        {
            return c == std::char_traits<char>::eof();
        }

        bool isNakedDelimiter(int c)
        {
            return isEOF(c) || isWhitespace(c) || c == ':' || c == '=' || c == '[' || c == ']' || c == '{' || c == '}' || c == '"' || c == '\'' || c == '/' || c == ',';
        }

        void skipAir()
        {
            while (true)
            {
                if (isWhitespace(peek()))
                {
                    next();
                }
                else if (peek() == '/') 
                {
                    next();
                    if (peek() == '/')
                    {
                        next();
                        skipLineComment();
                    }
                    else if (peek() == '*')
                    {
                        next();
                        skipBlockComment();
                    }
                    else
                    {
                        throw ParseError("Unexpected character: '/'");
                    }
                }
                else
                {
                    break;
                }
            }
        }

        void skipLineComment()
        {
            while (true)
            {
                if (peek() == '\n')
                {
                    next();
                    return;
                }
                else if (isEOF(peek()))
                {
                    return;
                }
                else
                {
                    next();
                }
            }
        }

        void skipBlockComment()
        {
            int depth = 1;

            while (depth > 0)
            {
                if (peek() == '*')
                {
                    next();
                    if (peek() == '/')
                    {
                        depth -= 1;
                    }
                }
                if (peek() == '/')
                {
                    next();
                    if (peek() == '*')
                    {
                        depth += 1;
                    }
                }
                else if (isEOF(peek()))
                {
                    return;
                }
                else
                {
                    next();
                }
            }
        }

        std::istream& _stream;
    };

    std::variant<Node, ParseError> decode(std::istream& s) {
        try
        {
            Parser parser(s);
            return parser.parseNode();
        }
        catch (ParseError& e)
        {
            return e;
        }
    }

    std::variant<Node, ParseError> decode(const std::string& s) {
        std::istringstream st(s);
        return decode(st);
    }
}
