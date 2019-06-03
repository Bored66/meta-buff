#pragma once

#include <tuple>
#include <typeinfo>

#include <iostream>
#include <cstdint>

#include "field_types.h"
#include "string_utils.h"

#include "pod_extractor/pod_inspector.h"
#include "pod_extractor/typeid_array.h"

#include "pod_extractor/tests/tests.h"
template<bool L, typename T>
using enable_if_t = typename std::enable_if<L,T>::type;

template <typename Type>
void print_type_name()
{
    using namespace str_utils_ce;
    const size_t offset = 5;//skipping "with " after [
    const char *func_name{__PRETTY_FUNCTION__};
    auto start = find_char_offset(func_name, '[');
    if (start > -1) start += offset;
    auto end = find_char_offset(func_name, ']');
    if (start == -1 || end == -1)
    {
        std::cout << func_name << std::endl;
        return;
    }
    char type_str[end - start]{};
    str_copy_ce(func_name, type_str, end - start, start, end);
    std::cout << type_str << std::endl;
}

template <typename field_type>
void print_field_name_type(const char*fldName)
{
    std::cout << fldName << ": " ;
    print_type_name<field_type>();
}
template <typename Fld>
enable_if_t<is_integral_const<typename Fld::type>::is_const, void>
print_field_typeinfo()
{
    using item_type = typename Fld::type;
    using type_under = typename is_integral_const<item_type>::type;
    print_field_name_type<type_under>(Fld{}.name);
    std::cout << " Value: " << std::hex
              << uint32_t(is_integral_const<item_type>::value);
    std::cout << std::endl;
}
template <typename Fld>
enable_if_t<is_integral_const<typename Fld::type>::is_const == false &&
        Fld::is_function == false, void>
print_field_typeinfo()
{
    using item_type = typename Fld::type;
    print_field_name_type<item_type>(Fld{}.name);
    std::cout << std::endl;
}
template <typename Fld>
enable_if_t<Fld::is_function, void>
print_field_typeinfo()
{
    using item_type = typename Fld::type;
    print_field_name_type<item_type>(Fld{}.name);
    print_type_name<typename Fld::proc_type>();
    std::cout << " " << uint32_t(Fld::proc_crc(nullptr, 3)) << std::endl;
}

template <int N, typename Tuple>
using tuple_element_t = typename std::tuple_element<N, Tuple>::type;

template <int N, typename Tuple>
using tuple_element_field_t = typename tuple_element_t<N, Tuple>::type;

template <typename Fld, typename Fld2>
void print_cmd_field_types(std::tuple<Fld, Fld2> )
{
    using item_type = tuple_element_t<0, std::tuple<Fld, Fld2>>;
    print_field_typeinfo<item_type>();
    using item_type2 = typename
        std::tuple_element<1, std::tuple<Fld, Fld2>>::type;
    print_field_typeinfo<item_type2>();
}

template <typename Fld, typename... Flds>
typename std::enable_if<(0 < sizeof...(Flds)), void>::type
print_cmd_field_types(std::tuple<Fld, Flds...> )
{
    using item_type = tuple_element_t<0, std::tuple<Fld, Flds...>>;
    print_field_typeinfo<item_type>();
    print_cmd_field_types(std::tuple<Flds...>{});
}

template <typename Fld, typename... Flds>
struct extract_cmd_field_types;

template <typename Fld, typename... Flds>
struct extract_cmd_field_types<std::tuple<Fld, Flds...>>
{
   using item_type = tuple_element_field_t<0, std::tuple<Fld, Flds...>>;
   using result = typename extract_cmd_field_types<std::tuple<Flds...>,
        std::tuple<item_type>>::result;
};

template <typename Fld, typename... Flds, typename... FldsOut >
struct extract_cmd_field_types<std::tuple<Fld, Flds...>, std::tuple<FldsOut...> >
{
   using item_type = tuple_element_field_t<0, std::tuple<Fld, Flds...>>;
   using result = typename extract_cmd_field_types<std::tuple<Flds...>,
        std::tuple<FldsOut..., item_type>>::result;
};

template <typename Fld, typename... FldsOut >
struct extract_cmd_field_types<std::tuple<Fld>, std::tuple<FldsOut...> >
{
   using item_type = tuple_element_field_t<0, std::tuple<Fld>>;
   using result = std::tuple<FldsOut..., item_type>;
};

template <typename Cmd>
using type_list_res = typename extract_cmd_field_types<Cmd>::result;

template <typename... Flds>
constexpr
auto get_cmd_field_types(std::tuple<Flds...> )
-> decltype(type_list_res<std::tuple<Flds...>>{})
{ return {}; }

template <typename... Params>
constexpr
void unpack_variadic(Params...)
{}
struct swallow
{
    template<typename... Args>
    swallow(Args &&...)
    {
    }
};


template <typename Fld,
          typename... Flds>
struct pop_int_const;

template <uint8_t C, typename... Flds>
struct pop_int_const<std::integral_constant<uint8_t, C>, std::tuple<Flds...>>
{
   using result = std::tuple<Flds...>;
};

template <typename Fld, typename... Flds>
struct pop_int_const<Fld, std::tuple<Flds...>>
{
   using result = std::tuple<Flds...,Fld>;
};

template <bool, typename... Flds>
struct select_type;

template <typename T, typename F>
struct select_type<true, T, F>
{
    using type = T;
};
template <typename T, typename F>
struct select_type<false, T, F>
{
    using type = F;
};
template <bool Cond, typename T, typename F>
using select_type_t = typename select_type<Cond, T, F>::type;

constexpr bool or_op(bool cond, bool cond2)
{ return cond || cond2; }
template<typename item_type>
struct if_not_param
{ bool value = is_integral_const<item_type>::is_const ||
           false ; };


template <typename Fld>
enable_if_t<is_pod_struct<Fld>::value==false &&
        is_integral_const<Fld>::is_const == false,void>
print_item_value(const Fld& fld)
{ //std::cout << __PRETTY_FUNCTION__;
    std::cout  << fld ;
}

void print_item_value(char fld)
{ //std::cout << __PRETTY_FUNCTION__;
    std::cout  << int16_t(fld) ;
}
void print_item_value(unsigned char fld)
{ //std::cout << __PRETTY_FUNCTION__;
    std::cout  << uint16_t(fld) ;
}

template <typename Fld>
enable_if_t<is_pod_struct<Fld>::value == true,void>
print_item_value(const Fld& fld)
{ dump_struct(fld);}

template <typename Fld>
auto print_item(const Fld& fld, size_t number)
{
    std::cout << number << ":[";
    print_item_value(fld);
    std::cout    << "] ";
    return number;
}

template <typename... Flds, size_t... I>
void print_flat_tuple_impl(const std::tuple<Flds...> & tpl, std::index_sequence<I...>)
{
    std::cout << std::hex;
    swallow{ print_item(std::get<I>(tpl), I)... };
}

template <typename... Flds>
void print_flat_tuple(const std::tuple<Flds...> & tpl)
{
    print_flat_tuple_impl(tpl, std::make_index_sequence<sizeof...(Flds)>{});
    std::cout << std::endl;
}
template <typename... Flds>
void print_field_names(const std::tuple<Flds...> &)
{
    const char *names[] = { Flds().name... };
    std::cout << "\nDump field names: ";
    for (auto name : names)
        std::cout << name << " ";
    std::cout << std::endl;
}
template <typename... Flds>
constexpr
int find_field_name_offset(const std::tuple<Flds...> &, const char* name)
{
    using namespace str_utils_ce;
    const char *names[] = { Flds().name... };
    for (size_t i = 0; i < sizeof(names)/sizeof(names[0]);i++)
    {
        if (str_equal_ce(names[i], name))
            return i;
    }
    return -1;
}

