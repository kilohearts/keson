#pragma once

#include <string>

#include "conf.h"

namespace keson
{
#if KESON_ENABLE_WSTRING
    std::string to_utf8(const std::wstring& s);

    std::wstring from_utf8(const std::string& s);
#endif

    std::string to_string(uint8_t  v);
    std::string to_string(int8_t   v);
    std::string to_string(uint16_t v);
    std::string to_string(int16_t  v);
    std::string to_string(uint32_t v);
    std::string to_string(int32_t  v);
    std::string to_string(uint64_t v);
    std::string to_string(int64_t  v);
    std::string to_string(float    v);
    std::string to_string(double   v);
    std::string to_string(bool     v);
    
    template<typename T> T        from_string           (const std::string& s);
    template<>           uint8_t  from_string<uint8_t>  (const std::string& s);
    template<>           int8_t   from_string<int8_t>   (const std::string& s);
    template<>           uint16_t from_string<uint16_t> (const std::string& s);
    template<>           int16_t  from_string<int16_t>  (const std::string& s);
    template<>           uint32_t from_string<uint32_t> (const std::string& s);
    template<>           int32_t  from_string<int32_t>  (const std::string& s);
    template<>           uint64_t from_string<uint64_t> (const std::string& s);
    template<>           int64_t  from_string<int64_t>  (const std::string& s);
    template<>           float    from_string<float>    (const std::string& s);
    template<>           double   from_string<double>   (const std::string& s);
    template<>           bool     from_string<bool>     (const std::string& s);
}
