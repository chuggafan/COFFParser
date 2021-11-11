#pragma once
#include <cassert>
#include <cstdlib>
#include <cstdint>
#if __cplusplus >= 202002L
#    include <bit>
#    define constexpr_cpp20 constexpr
#    define constexpr_cpp17 constexpr
#else
#    if __cplusplus >= 201703L
#        define constexpr_cpp17 constexpr
#    else
#        define constexpr_cpp17
#    endif
#    define constexpr_cpp20
#endif
namespace OrangeC
{
namespace Utils
{
constexpr_cpp20 inline bool is_little_endian()
{
#if __cplusplus < 202002L
    union
    {
        int i;
        alignas(alignof(int)) char c[sizeof(int)];
    } x;
    x.i = 1;
    if (x.c[0] == 1)
    {
        return true;
    }
    return false;  // Assume big-endian, as mixed endian systems are rare enough
#else
    return std::endian::native == std::endian::little;
#endif
}
template <typename T>
inline T swap_bytes(T value)
{
#ifdef _MSC_VER
#    define bswap_16(a) _byteswap_ushort(a)
#    define bswap_32(a) _byteswap_ulong(a)
#    define bswap_64(a) _byteswap_uint64(a)
#else
#    define bswap_16(a) __builtin_bswap16(a)
#    define bswap_32(a) __builtin_bswap32(a)
#    define bswap_64(a) __builtin_bswap64(a)
#endif
    switch (sizeof(T))  // A switch like this should be available to be constant-optimized, if it isn't I'll be pretty mad
    {
        case sizeof(uint16_t):
            return bswap_16(value);
            break;
        case sizeof(uint32_t):
            return bswap_32(value);
            break;
        case sizeof(uint64_t):
            return bswap_64(value);
            break;
        default:
            assert(false);
            break;
    }
    return 0;
#undef bswap_16
#undef bswap_32
#undef bswap_64
}
template <typename T>
inline T little_to_platform(T value)
{
    if constexpr_cpp20 (is_little_endian())
    {
        return value;
    }
    return swap_bytes(value);
}
template <typename T>
inline T big_to_platform(T value)
{
    if constexpr_cpp20 (is_little_endian())
    {
        return swap_bytes(value);
    }
    return value;
}
}  // namespace Utils
}  // namespace OrangeC