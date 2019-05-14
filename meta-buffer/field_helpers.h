#pragma once
#include <iostream>
#include <type_traits>

#include "field_type_utils.h"

template <typename Tuple>
struct tuple_holder
{
    using TypeHolder = Tuple;
protected:
    TypeHolder _tuple;
    virtual TypeHolder get_tuple() { return _tuple; }
};

template <size_t N, typename FldType, typename Tuple>
struct field_accessors : virtual public tuple_holder<Tuple>
{
    using tuple_holder<Tuple>::get_tuple;
    //using tuple_holder<Tuple>::tuple_holder;
    enable_if_t<is_integral_const<typename FldType::type>::is_const, void>
    set_field(FldType & value)
    {
        auto & field = std::get<N>(get_tuple());
        field = value;
    }
    FldType get_field()
    {
        return std::get<N>(get_tuple());
    }
};
template <typename... FldTypes>
struct command_call;

template <typename... FldTypes>
struct command_call<std::tuple<FldTypes...>>
{
    void operator()(FldTypes... args)
    {
        call(args...);
    }
    void call(FldTypes...)
    {
        std::cout << __PRETTY_FUNCTION__;
    }
};

template <typename... FldTypes>
struct command_calls;

template <typename FldType, typename... FldTypes>
struct command_calls<FldType, FldTypes...> : FldType, command_calls<FldTypes...>
{
    //using FldType::operator();
    using FldType::call;
    //using command_calls<FldTypes...>::operator();
    using command_calls<FldTypes...>::call;
};
template <typename FldType>
struct command_calls<FldType> : FldType
{
    //using FldType::operator();
    using FldType::call;
};
