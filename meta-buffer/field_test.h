#pragma once

#include <tuple>
#include "field_types.h"

constexpr char markerFldName[] {"Marker"};
constexpr char commandFldName[] {"Command"};
constexpr char dataFldName[] {"Data"};
constexpr char data16FldName[] {"Data16"};
constexpr char data32FldName[] {"Data32"};
constexpr char crcFldName[] {"CRC"};
constexpr char nobFldName[] {"NOB"};
constexpr char podFldName[] {"POD"};
extern inline uint8_t crc8(uint8_t*seq, size_t size);
template<uint8_t I>
using byte_i_c = std::integral_constant<uint8_t, I>;
const auto marker_v = 0xAA;
const auto cmd1_v = 0x01;
const auto cmd2_v = 0x02;
const auto cmd3_v = 0x03;
const auto cmd4_v = 0x04;

using MarkerField = field<byte_i_c<marker_v>, markerFldName>;
using CommandField = field<uint8_t, commandFldName>;
using CommandField1 = field<byte_i_c<cmd1_v>, commandFldName>;
using CommandField2 = field<byte_i_c<cmd2_v>, commandFldName>;
using CommandField3 = field<byte_i_c<cmd3_v>, commandFldName>;
using CommandField4 = field<byte_i_c<cmd4_v>, commandFldName>;

using DataField = field<uint8_t, dataFldName>;
using DataField16 = field<uint16_t, data16FldName>;
using DataField32 = field<uint32_t, data32FldName>;
using CrcField = crc_field<uint8_t, crcFldName, &crc8>;

using Cmd1st = std::tuple<MarkerField, CommandField1,
    DataField, DataField16, CrcField>;

using Cmd2nd = std::tuple<MarkerField, CommandField2,
    DataField, DataField32, CrcField>;

using Cmd3rd = std::tuple<MarkerField, CommandField3,
    DataField, DataField32, CrcField>;

using NobField = field<uint8_t, nobFldName>;
using VarField_uint16 = varsized_field<uint16_t, data16FldName, nobFldName>;

struct pod
{
    uint16_t i16;
    uint8_t i8;
};
using VarField_pod = varsized_field<pod, podFldName, nobFldName>;

using Cmd4th = std::tuple<MarkerField, CommandField4,
    DataField, NobField, VarField_uint16, CrcField>;

using Cmd4thA = std::tuple<MarkerField, CommandField4,
    DataField, NobField, VarField_pod, CrcField>;
