#pragma once
#include <cstdint>
#include <type_traits>

template<typename FldType>
struct is_integral_const
{
    static constexpr bool is_const = false;
    using type = FldType;
    static constexpr std::size_t value = 0;
};

template<typename FldType, FldType Code>
struct is_integral_const<std::integral_constant<FldType, Code>>
{
    static constexpr bool is_const = true;
    using type = FldType;
    static constexpr FldType value = Code;
};
template<typename FldType>
struct field_type
{
    using type = FldType;
    using orig_type = FldType;
    static constexpr bool is_function = false;
};
template<typename FldType, const char *Name>
struct field
{
    using type = typename field_type<FldType>::type;
    const char *name{Name};
    static constexpr bool is_function = false;
    static constexpr bool has_value = false;
};
template<typename FldType, const char *Name,
         FldType(*FuncPtr)(uint8_t*seq, std::size_t size)>
struct crc_field
{
    using type = FldType;
    using proc_type = FldType(*)(uint8_t*seq, std::size_t size);
    const char *name{Name};
    static constexpr proc_type proc_crc = FuncPtr;
    static constexpr bool is_function = true;
    static constexpr bool has_value = true;//TODO: from applying proc_crc
};
