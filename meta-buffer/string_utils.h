#pragma once

namespace str_utils_ce
{
inline constexpr
size_t strlen_ce(const char* str) noexcept
{
    int i = 0;
    while(str[i]) i++;
    return i;
}
inline constexpr
bool str_equal_ce(const char *src, const char *dst) noexcept
{
    int i = 0;
    for (; src[i] && dst[i]; i++)
    {
        if(src[i] == dst[i])
            continue;
        return false;
    }
    if (src[i] != 0 || dst[i] != 0)
        return false;
    return true;
}
inline constexpr
int find_char_offset(const char *src, const char ch) noexcept
{
    for (int i = 0; src[i] ;i++)
    {
        if (src[i] == ch)
            return i;
    }
    return -1;
}
//template<size_t N>
inline constexpr
auto str_copy_ce(const char *src, char *dst, std::size_t dst_size,
                 // char (&dst)[N],
                           std::size_t from, std::size_t to) noexcept
{
    for (size_t src_index = 0, dst_index = 0;
         src[src_index] && dst_index < dst_size ; src_index++)
    {
        if (src_index > from && src_index < to)
            dst[dst_index++] = src[src_index];
    }
    return dst;
}

}
