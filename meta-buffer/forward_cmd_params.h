#pragma once

template<typename Type, size_t I, typename Arg>
constexpr auto forward_param(Type & tuple, Arg&& arg)
{
    using ref_type = decltype(std::get<I>(tuple));
    using conv_to = typename std::remove_reference<ref_type>::type;
    std::get<I>(tuple) = static_cast<conv_to>(arg);
    return true;
}
template<typename Type, size_t...I, typename...Args>
constexpr auto forward_params_impl(std::index_sequence<I...>, Args&&...args)
{
    constexpr auto info = get_cmd_field_info<Type>();
    using Tuple = decltype(get_cmd_field_types(Type{}));
    Tuple tuple{};
    unpack_variadic
    (
        forward_param<Tuple,info.nonconst_flds[I]>(tuple, args)...
    );
    return tuple;
}

template<typename Type, typename...Args>
constexpr auto forward_params(Args&&...args)
{
    constexpr auto info = get_cmd_field_info<Type>();
    constexpr auto params_count = info.params_count();
    constexpr auto fact_count = std::min(params_count, sizeof...(args));
    static_assert(params_count >= sizeof...(args),
              "Too many parameters supplied exceeding number non-const of fields");
    return forward_params_impl<Type>(std::make_index_sequence<fact_count>{}, args...);
}

