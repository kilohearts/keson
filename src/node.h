#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <variant>

#include "conf.h"

namespace keson
{
    class Node {
    public:
        using Null   = std::monostate;
        using Atom   = std::string;
        using Vector = std::vector<Node>;
        using Map    = std::unordered_map<std::string, Node>;

        Node();
        Node(std::string         value);
        Node(const char*         value);
        Node(Vector              value);
        Node(Map                 value);
        Node(int8_t              value);
        Node(uint8_t             value);
        Node(int16_t             value);
        Node(uint16_t            value);
        Node(int32_t             value);
        Node(uint32_t            value);
        Node(int64_t             value);
        Node(uint64_t            value);
        Node(float               value);
        Node(double              value);
        Node(bool                value);
#if KESON_ENABLE_WSTRING
        Node(const std::wstring& value);
        Node(const wchar_t*      value);
#endif

        bool isNull() const;
        bool isAtom() const;
        bool isVector() const;
        bool isMap() const;

        //////////
        // Atom //
        //////////

        const Atom& atom() const;
        Atom& atom();

        std::string value_or(std::string   fallback) const;
        int8_t      value_or(int8_t        fallback) const;
        uint8_t     value_or(uint8_t       fallback) const;
        int16_t     value_or(int16_t       fallback) const;
        uint16_t    value_or(uint16_t      fallback) const;
        int32_t     value_or(int32_t       fallback) const;
        uint32_t    value_or(uint32_t      fallback) const;
        int64_t     value_or(int64_t       fallback) const;
        uint64_t    value_or(uint64_t      fallback) const;
        float       value_or(float         fallback) const;
        double      value_or(double        fallback) const;
        bool        value_or(bool          fallback) const;
#if KESON_ENABLE_WSTRING
        std::wstring value_or(std::wstring fallback) const;
#endif

        operator std::string() const;
        operator int8_t() const;
        operator uint8_t() const;
        operator int16_t() const;
        operator uint16_t() const;
        operator int32_t() const;
        operator uint32_t() const;
        operator int64_t() const;
        operator uint64_t() const;
        operator float() const;
        operator double() const;
        operator bool() const;
#if KESON_ENABLE_WSTRING
        operator std::wstring() const;
#endif

        void operator=(std::string         value);
        void operator=(const char*         value);
        void operator=(int8_t              value);
        void operator=(uint8_t             value);
        void operator=(int16_t             value);
        void operator=(uint16_t            value);
        void operator=(int32_t             value);
        void operator=(uint32_t            value);
        void operator=(int64_t             value);
        void operator=(uint64_t            value);
        void operator=(float               value);
        void operator=(double              value);
        void operator=(bool                value);
#if KESON_ENABLE_WSTRING
        void operator=(const std::wstring& value);
        void operator=(const wchar_t*      value);
#endif

        ////////////
        // Vector //
        ////////////

        const Vector& vector() const;
        Vector& vector();

        size_t length() const;

        const Node* begin() const;
        Node* begin();

        const Node* end() const;
        Node* end();

        Node& operator[](size_t pos);
        const Node& operator[](size_t pos) const;

        void push_back(Node node);
        Node& push_back();

        /////////
        // Map //
        /////////

        const Map& map() const;
        Map& map();

        Node& operator[](const std::string& key);
        const Node& operator[](const std::string& key) const;
        Node& operator[](const char* key);
        const Node& operator[](const char* key) const;

    private:
        friend void swap(Node& a, Node& b);

        static const Node NULL_NODE;
        std::variant<Null, Atom, Vector, Map> _value;
    };
}

namespace std { void swap(keson::Node& a, keson::Node& b); }
