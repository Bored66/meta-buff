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
void find_field_test();

void probs()
{
    //basic_test();
    //cmd_tests();
    crc_test();

    cmd_prepare_test();
    string_utils_test();
    //byte_stream_test();
}
void cmd_tests()
{
    using test_cmd = Cmd1st;
    constexpr auto firstCmdInfo = get_cmd_field_info<test_cmd>();
    size_t arr[firstCmdInfo.const_vals[0]];(void)arr;
    static_assert(firstCmdInfo.const_vals[0] == 170, "firstCmdInfo.const_vals[0] != 10");
    static_assert(firstCmdInfo.const_vals[1] == 1, "firstCmdInfo.const_vals[0] != 1");
    static_assert(firstCmdInfo.const_vals[2] == 0, "firstCmdInfo.const_vals[0] != 0");
    for (uint32_t i = 0; i < firstCmdInfo.size(); i++)
    {
        std::cout << std::hex << std::uppercase << "0x" << uint32_t(firstCmdInfo.const_vals[i]) << ":"
                  << std::dec << firstCmdInfo.field_size[i] << "=";
    }
}
void crc_test()
{
    std::tuple_element<4, Cmd1st>::type::proc_crc(nullptr, 1);
    std::tuple_element<4, Cmd1st>::type::proc_type tt =
            std::tuple_element<4, Cmd1st>::type::proc_crc;
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

    constexpr auto tupleCmd1st = forward_params<Cmd1st>(75,'3');
    //print_item_value(std::get<0>(tupleCmd1st));
    print_flat_tuple(tupleCmd1st);
    std::cout << "total_size(): " << datagram_size<Cmd1st>();
    std::cout << std::endl;
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
    constexpr auto tupleCmd4 = forward_params<Cmd5th>(75,pod{115,'3',345});
    print_flat_tuple(tupleCmd4);
    std::cout << "total_size(): " << datagram_size<Cmd5th>();
}
void basic_test()
{
    constexpr MarkerField mf;

    constexpr bool f = field_type<typename std::tuple_element<4, Cmd1st>::type>::is_function;
    static_assert (f == false, "T");
    static_assert (mf.name == markerFldName, "mf.name == marker");
    static_assert (std::is_same<CrcField::type, uint8_t>::value, "is_same<CrcField::type");
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
    //print_field_typeinfo<tuple_element_t<0, Cmd1st>>();
    //print_field_typeinfo<CrcField>();
    using t3 = tuple_element_t<0, Cmd1st>;
    t3 t3v;(void)t3v;
    static_assert(is_integral_const<t3::type>::is_const, "not integral const");
    print_cmd_field_types(Cmd1st{});
    print_cmd_field_types(Cmd2nd{});
    print_cmd_field_types(Cmd5th{});
    std::cout << std::endl;
    print_type_name<decltype(get_cmd_field_types(Cmd1st{}))>();
    std::cout << std::endl;
    print_type_name<decltype(get_cmd_field_types(Cmd2nd{}))>();
    std::cout << std::endl;
    print_type_name<decltype(get_cmd_field_types(Cmd5th{}))>();
    std::cout << std::endl;
    print_field_typeinfo<tuple_element_t<4, Cmd5th>>();
}

void string_utils_test()
{
    using namespace str_utils_ce;
    print_type_name<cmd2args>();//test for str_copy_ce
    static_assert(str_equal_ce(DataField16().name,
                               DataField16().name), "Not Equal");
    static_assert(not str_equal_ce(DataField().name,
                               DataField16().name), "Equal");
    static_assert(not str_equal_ce(MarkerField().name,
                               DataField16().name), "Equal");
    static_assert(find_char_offset("  [][][][][]",'[')==2, "[");
    static_assert(find_char_offset("[][][][][]",'{')==-1, "{");
    find_field_test();
    static_assert(str_utils_ce::strlen_ce("aaaa"), "aaaa!=4");
}
void find_field_test()
{
    print_field_names(Cmd1st{});
    static_assert(find_field_name_offset(Cmd1st{},
                              DataField16{}.name) == 3, "3");
    static_assert(find_field_name_offset(Cmd1st{},
                              DataField16{}.name) != 2, "Not 2");
    static_assert(find_field_name_offset(Cmd1st{},
                              DataField{}.name) == 2, "2");
    static_assert(find_field_name_offset(Cmd1st{},
                              DataField32{}.name) == -1, "-1");
    print_field_names(Cmd4th{});
    std::cout << NobField{}.name << " offset: "
         << find_field_name_offset(Cmd4th{}, VarField_uint16().refFieldName)
         << " " << find_field_name_offset(Cmd4th{}, VarField_uint16().name) << " "
         << VarField_uint16().refFieldName;
}
extern inline uint8_t crc8(uint8_t*seq, size_t size)
{
    (void)seq;
    std::cout << __PRETTY_FUNCTION__ << ":" << size << std::endl;
    return uint8_t(size);
}
