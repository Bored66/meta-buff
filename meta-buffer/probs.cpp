#include "field_types.h"
#include "field_type_utils.h"
#include "extract_cmd_params_types.h"
#include "cmd_field_info.h"
#include "forward_cmd_params.h"
#include "field_test.h"
#include "field_helpers.h"
#include "string_utils.h"
#include "byte_stream.h"

void basic_test();
void string_utils_test();
void cmd_prepare_test();
void crc_test();
void cmd_tests();

void probs()
{
    //basic_test();
    cmd_tests();
    crc_test();

    cmd_prepare_test();
    //string_utils_test();
    //byte_stream_test();
}
void cmd_tests()
{
    constexpr auto firstCmdInfo = get_cmd_field_info<Cmd1st>();
    size_t arr[firstCmdInfo.const_vals[0]];(void)arr;
    static_assert(firstCmdInfo.const_vals[0] == 170, "firstCmdInfo.const_vals[0] != 10");
    static_assert(firstCmdInfo.const_vals[1] == 1, "firstCmdInfo.const_vals[0] != 10");
    static_assert(firstCmdInfo.const_vals[2] == 0, "firstCmdInfo.const_vals[0] != 10");
    for (uint32_t i = 0; i < firstCmdInfo.size(); i++)
    {
        std::cout << std::hex << std::uppercase << "0x" << uint32_t(firstCmdInfo.const_vals[i]) << ":"
                  << std::dec << firstCmdInfo.field_size[i] << "=";
    }
}
void crc_test()
{
    std::tuple_element<4, Cmd1st>::type::proc_crc(nullptr, 1);
    std::tuple_element<4, Cmd1st>::type::proc_type tt = std::tuple_element<4, Cmd1st>::type::proc_crc;
    tt(nullptr,3);
}

using src = std::tuple<byte_i_c<0x0>,
uint8_t, int16_t, byte_i_c<0x01>,  int64_t, byte_i_c<0x02>,  float, byte_i_c<0x03>>;

class Cmd1class : public tuple_holder<src>
{
    //Cmd1class(tuple_holder<src>::TypeHolder & tuple):tuple_holder<src>(tuple){}
};
using cmd1args = command_call<//uint8_t, uint32_t>;//
    decltype(get_cmd_field_types(Cmd1st{}))>;
using cmd2args = command_call<//command_call<uint16_t, uint32_t>;
    decltype(get_cmd_field_types(Cmd2nd{}))>;
using cmd3args = command_call<decltype(get_cmd_field_types(Cmd3rd{}))>;

void cmd_prepare_test()
{
    std::make_tuple<byte_i_c<170>, byte_i_c<1>, uint8_t, uint16_t,uint8_t>
        (byte_i_c<170>{},byte_i_c<1>{}, uint8_t(7), uint16_t(32),
                uint8_t(12));
    std::cout << std::endl;
    using target = std::tuple<uint8_t, int16_t, int64_t, float>;

    constexpr auto tuple3 = forward_params<Cmd1st>(5.2,'3');
    print_flat_tuple(tuple3);
    using res = extract_cmd_params_types_result<src>;
    using srcNoParams = std::tuple<byte_i_c<0x0>,
    byte_i_c<0x01>,  byte_i_c<0x02>,  byte_i_c<0x03>>;
    using target0 = void;
    using res2 = extract_cmd_params_types_result<srcNoParams>;
    static_assert(std::is_same<target0,res2>::value,"");
    static_assert(std::is_same<target,res>::value,"");
    static_assert(std::is_same<select_type_t<byte_i_c<0x01>::value,uint8_t,int8_t>, uint8_t>::value, "");
    static_assert(std::is_same<select_type_t<not byte_i_c<0x01>::value,uint8_t,int8_t>, int8_t>::value, "");
    src src1;
    Cmd1class cmd1;
}
void basic_test()
{
    constexpr MarkerField mf;
    //constexpr CrcField crcField;

    constexpr bool f = field_type<typename std::tuple_element<4, Cmd1st>::type>::is_function;
    static_assert (f == false, "T");
    static_assert (mf.name == markerFldName, "mf.name == marker");
    static_assert (std::is_same<CrcField::type, uint8_t>::value, "is_same<CrcField::type");
    //static_assert (std::is_same<CrcField_14::type, uint8_t>::value, "is_same<CrcField::type");
    static_assert (std::is_same<CrcField, std::tuple_element<4, Cmd1st>::type
                   >::value, "is_same Crcfield");
    static_assert(is_integral_const<std::integral_constant<uint8_t, 11>>::is_const, "not integral const");
    static_assert(not is_integral_const<CrcField>::is_const, "integral const?");
    using t1 = MarkerField::type;
    static_assert(is_integral_const<t1>::is_const, "not integral const");
    auto val = t1::value;
    val++;
    using t2 = CrcField;
    static_assert(not is_integral_const<t2>::is_const, "integral const?");
    print_field_typeinfo<tuple_element_t<0, Cmd1st>>();
    print_field_typeinfo<CrcField>();
    using t3 = tuple_element_t<0, Cmd1st>;
    t3 t3v;(void)t3v;
    static_assert(is_integral_const<t3::type>::is_const, "not integral const");
    print_cmd_field_types(Cmd1st{});
    print_cmd_field_types(Cmd2nd{});
    std::cout << std::endl;
    type_name<decltype(get_cmd_field_types(Cmd1st{}))>();
    std::cout << std::endl;
    type_name<decltype(get_cmd_field_types(Cmd2nd{}))>();
    std::cout << std::endl;
}
void string_utils_test()
{
    constexpr char func_name[] = "0123456789";//  __PRETTY_FUNCTION__ ;
    type_name<decltype(func_name)>();
    //constexpr auto str_sz = str_utils_ce::strlen_ce(func_name);
    //constexpr auto str2 = str_utils_ce::create_from_ce(func_name);
    //std::cout << " " << func_name << ":" << std::dec << str_sz;
}
void byte_stream_test()
{
    byte_stream bs1;
    std::vector<uint8_t> bs;
    uint8_t bytes[] = {11,12,13,14,15};
    for (uint32_t i = 0; i < 6; i++)
        bs.insert(bs.begin(), bytes, bytes+i);
    for (uint32_t i = 0; i < 6; i++)
        bs.insert(bs.end(), bytes, bytes+i);
    for (uint32_t i = 1; i <= bs1.size(); i++)
        bs1.append(bytes, i);
    for (uint32_t i = 0; i < bs1.size(); i++)
        std::cout << std::hex << int(bs1[i]) << " ";
}
extern inline uint8_t crc8(uint8_t*seq, size_t size)
{
    std::cout << __PRETTY_FUNCTION__ << ":" << size << std::endl;
    return uint8_t(size);
}
