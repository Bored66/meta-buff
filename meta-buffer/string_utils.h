#pragma once

namespace str_utils_ce
{
inline
constexpr size_t strlen_ce(const char* const str)
{
    if (str && *str != 0)
        return 1 + strlen_ce(str+1);
    return 0;
}
template<size_t N>
inline
constexpr auto strcpy_ce(const char (&src)[N], char (&dst)[N]) -> decltype(dst)
{
    for (int i = 0;src; i++)
    {
        dst[i] = src[i];
        if(src[i] == 0) break;
    }
    return dst;
}
template<size_t N>
inline
constexpr auto create_from_ce(const char (&src)[N])
{
    const char str1[N];
    constexpr auto str2 = str_utils_ce::strcpy_ce(src, str1);
    return str1;
}
inline
constexpr bool str_equal_ce(const char *src, const char *dst)
{
    int i = 0;
    for (; src[i] != 0 && dst[i] != 0; i++)
    {
        if(src[i] == dst[i])
            continue;
        return false;
    }
    if (src[i] != 0 || dst[i] != 0)
        return false;
    return true;
}
}
