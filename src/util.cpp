#include "util.h"

#include <charconv>
#include <cassert>

#if KESON_ENABLE_WSTRING
#include <codecvt>
#endif

namespace keson
{
#if KESON_ENABLE_WSTRING
    // TODO: Replace with something not deprecated
    std::string to_utf8(const std::wstring& s) {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.to_bytes(s);
    }

    std::wstring from_utf8(const std::string& s) {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(s);
    }
#endif
    template <typename T>
    static std::string arithmetic_to_string(T v) {
        char tmp[100];
        auto result = std::to_chars(tmp, tmp + sizeof(tmp), v);
        assert(result.ec == (std::errc)0);
        return std::string(tmp, result.ptr);
    }

    template <typename T>
    static T int_from_string(const std::string& s) {
        // TODO: Handle 0x, 0o, 0b prefixes?
        T v;
        auto result = std::from_chars(s.data(), s.data() + s.size(), v);
        assert(result.ec == (std::errc)0);
        assert(result.ptr == s.data() + s.size());
        return v;
    }

    template <typename T>
    static T float_from_string(const std::string& s) {
        T v;
        auto result = std::from_chars(s.data(), s.data() + s.size(), v);
        assert(result.ec == (std::errc)0);
        assert(result.ptr == s.data() + s.size());
        return v;
    }

    std::string to_string(uint8_t  v) { return arithmetic_to_string(v); }
    std::string to_string(int8_t   v) { return arithmetic_to_string(v); }
    std::string to_string(uint16_t v) { return arithmetic_to_string(v); }
    std::string to_string(int16_t  v) { return arithmetic_to_string(v); }
    std::string to_string(uint32_t v) { return arithmetic_to_string(v); }
    std::string to_string(int32_t  v) { return arithmetic_to_string(v); }
    std::string to_string(uint64_t v) { return arithmetic_to_string(v); }
    std::string to_string(int64_t  v) { return arithmetic_to_string(v); }
    std::string to_string(double   v) { return arithmetic_to_string(v); }
    std::string to_string(float    v) { return arithmetic_to_string(v); }
    std::string to_string(bool     v) { return v ? "true" : "false"; }
    
    template<> uint8_t  from_string<uint8_t>  (const std::string& s) { return int_from_string<uint8_t>(s);  }
    template<> int8_t   from_string<int8_t>   (const std::string& s) { return int_from_string<int8_t>(s);   }
    template<> uint16_t from_string<uint16_t> (const std::string& s) { return int_from_string<uint16_t>(s); }
    template<> int16_t  from_string<int16_t>  (const std::string& s) { return int_from_string<int16_t>(s);  }
    template<> uint32_t from_string<uint32_t> (const std::string& s) { return int_from_string<uint32_t>(s); }
    template<> int32_t  from_string<int32_t>  (const std::string& s) { return int_from_string<int32_t>(s);  }
    template<> uint64_t from_string<uint64_t> (const std::string& s) { return int_from_string<uint64_t>(s); }
    template<> int64_t  from_string<int64_t>  (const std::string& s) { return int_from_string<int64_t>(s);  }
    template<> float    from_string<float>    (const std::string& s) { return float_from_string<float>(s);  }
    template<> double   from_string<double>   (const std::string& s) { return float_from_string<double>(s); }
    template<> bool     from_string<bool>     (const std::string& s) { return s == "true";                  }
}
