#pragma once

#include <tuple>

#include "field_type_utils.h"

template<typename FldType>
constexpr enable_if_t<is_pod_struct<FldType>::value == false,std::size_t>
field_size()
{
    return sizeof(FldType);
}
template<typename FldType>
constexpr enable_if_t<is_pod_struct<FldType>::value == true,std::size_t>
field_size()
{
    auto info = get_pod_meta_infos<FldType>();
    size_t total = 0;
    for (size_t i = 0; i < info.index; ++i)
        total += info.type_sizes[i];
    return total;
}

template<std::size_t N>
struct cmd_info
{
    uint8_t const_vals[N]{};
    std::size_t field_size[N]{};
    enum field_type {OTHER=0, CRC};
    field_type other_vals[N]{};
    uint8_t nonconst_flds[N]{};
    static constexpr std::size_t size() noexcept { return N; }
    constexpr void add_const_val(const uint8_t cv, const std::size_t index) noexcept
    {
        const_vals[index] = cv;
    }
    constexpr std::size_t add_field_size(const std::size_t size, std::size_t index) noexcept
    {
        return field_size[index] = size;
    }
    constexpr field_type add_other(const field_type v, const std::size_t index) noexcept
    {
        return other_vals[index] = v;
    }
    constexpr bool is_param(const std::size_t index) const noexcept
    {
        return not const_vals[index] && not other_vals[index];
    }
    constexpr std::size_t params_count() const noexcept
    {
        std::size_t count = 0;
        for (std::size_t index = 0; index < N; index++)
            if (is_param(index))
                count++;
        return count;
    }
    constexpr std::size_t update_params() noexcept
    {
        std::size_t index = 0;
        for (std::size_t i = 0; i < N; i++)
        {
            if (is_param(i))
                nonconst_flds[index++] = i;
        }
        return index;
    }
    constexpr std::size_t total_size() const noexcept
    {
        size_t total = 0;
        for (size_t i = 0; i < N; ++i)
            total += field_size[i];
        return total;
    }
};
template
<
    typename Command,
    std::size_t N = std::tuple_size<Command>::value,
    std::size_t...I
>
constexpr
auto collect_cmd_field_info(std::index_sequence<I...>, cmd_info<N> info = {})
noexcept -> decltype(info)
{
    using cmd_info_n = decltype(info);
    unpack_variadic
    (
        (
        info.add_const_val(is_integral_const<tuple_element_field_t<I, Command>>::value , I),
        info.add_other(is_integral_const<tuple_element_field_t<I, Command>>::is_const == false &&
                       std::tuple_element<I, Command>::type::is_function == true?
                        cmd_info_n::field_type::CRC : cmd_info_n::field_type::OTHER ,I),
        info.add_field_size(field_size<tuple_element_field_t<I, Command>>(), I)
        )...
    );
    info.update_params();
    return info;
}
template
<
    typename Cmd,
    std::size_t FieldCount = std::tuple_size<Cmd>::value
>
constexpr
auto get_cmd_field_info()
{
    return collect_cmd_field_info<Cmd, FieldCount>
            (std::make_index_sequence<FieldCount>());
}

template<typename Tuple>
constexpr size_t datagram_size() noexcept
{
    constexpr auto info = get_cmd_field_info<Tuple>();
    return info.total_size();
}

template<typename Tuple, size_t N = datagram_size<Tuple>()>
struct datagram
{
    using uchar = unsigned char;
    uchar bytes[N]{};
    size_t offset = 0;
    template<typename Input>
    uint append(Input value)
    {
        for (uint i = 0; i < sizeof(Input); i++, offset++)
            bytes[offset] = uchar((value >> i*8) & 0xFF);
        return sizeof(Input);
    }
    void print_seq()
    {
        for (uint i = 0; i < offset; i++)
            printf(" %02x", bytes[i]);
    }
};

template <size_t N, typename Tuple,
          typename... Args>
constexpr auto append(Args&&... args)
{
    datagram<Tuple, N> cmd{};
    cmd.offset = 0;
    size_t sz[] = { cmd.append(args)... };(void)sz;
    return cmd;
}

template <size_t N, typename... Flds, size_t... I>
constexpr auto tuple_to_datagram_impl(const std::tuple<Flds...> & tpl,
                            std::index_sequence<I...>)
{
    using tuple_type = std::tuple<Flds...>;
    return append<N, tuple_type>( std::get<I>(tpl)... );
}

template <size_t N, typename... Flds>
constexpr auto  tuple_to_datagram_impl(const std::tuple<Flds...> & tpl)
{
    return tuple_to_datagram_impl<N>(tpl,
                        std::make_index_sequence<sizeof...(Flds)>{});
}

template<size_t N, typename... Flds>
constexpr auto tuple_to_datagram(const std::tuple<Flds...>& tuple)
//-> decltype(datagram<Tuple, N>{})
{
    return tuple_to_datagram_impl<N>(tuple,
                        std::make_index_sequence<sizeof...(Flds)>{});
}
