#pragma once

#include <tuple>

#include "field_type_utils.h"

template <typename Fld, typename... Flds>
struct extract_cmd_params_types;

template <typename Fld,
          typename... Flds>
struct extract_cmd_params_types<std::tuple<Fld, Flds...>>
{
    using item_type = tuple_element_t<0, std::tuple<Fld, Flds...>>;
    using skip = extract_cmd_params_types<std::tuple<Flds...>>;
    using combine = extract_cmd_params_types<std::tuple<Flds...>,
                std::tuple<item_type>>;
    using result = typename select_type_t< is_integral_const<item_type>::is_const,
                        skip,
                        combine>::result;
};

template <typename Fld,
          typename... Flds, typename... FldsOut>
struct extract_cmd_params_types<std::tuple<Fld, Flds...>, std::tuple<FldsOut...>>
{
    using item_type = tuple_element_t<0, std::tuple<Fld, Flds...>>;
    using skip = extract_cmd_params_types<std::tuple<Flds...>,
                std::tuple<FldsOut...>>;
    using combine = extract_cmd_params_types<std::tuple<Flds...>,
                std::tuple<FldsOut...,item_type>>;
    using result = typename select_type_t< is_integral_const<item_type>::is_const,
                        skip,
                        combine>::result;
};

template <typename Fld, typename... FldsOut >
struct extract_cmd_params_types<std::tuple<Fld>, std::tuple<FldsOut...> >
{
   using item_type = tuple_element_t<0, std::tuple<Fld>>;
   using result = select_type_t< is_integral_const<item_type>::is_const,
        std::tuple<FldsOut...>,
        std::tuple<FldsOut..., item_type>>;
};

template <typename Fld>
struct extract_cmd_params_types<std::tuple<Fld>>
{
   using item_type = tuple_element_t<0, std::tuple<Fld>>;
   using result = select_type_t< is_integral_const<item_type>::is_const,
        void,
        std::tuple<item_type>>;
};

template <typename... Flds>
using extract_cmd_params_types_result =
    typename extract_cmd_params_types<Flds...>::result;
