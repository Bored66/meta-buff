#pragma once

#include <tuple>
#include "field_types.h"

constexpr char markerFldName[] {"Marker"};
constexpr char command[] {"Command"};
constexpr char data[] {"Data"};
constexpr char data16[] {"Data16"};
constexpr char data32[] {"Data32"};
constexpr char crc[] {"CRC"};
extern inline uint8_t crc8(uint8_t*seq, size_t size);
template<uint8_t I>
using byte_i_c = std::integral_constant<uint8_t, I>;
const auto marker_v = 0xAA;

using MarkerField = field<byte_i_c<marker_v>, markerFldName>;
using CommandField = field<uint8_t, command>;
using CommandField1 = field<byte_i_c<0x01>, command>;
using CommandField2 = field<byte_i_c<0x02>, command>;
using CommandField3 = field<byte_i_c<0x03>, command>;
using CommandField4 = field<byte_i_c<0x04>, command>;

using DataField = field<uint8_t, data>;
using DataField16 = field<uint16_t, data16>;
using DataField32 = field<uint32_t, data32>;
using CrcField = crc_field<uint8_t, crc, &crc8>;
//using CrcField = field<crc_proc<&crc8>, crc>;
//using CrcField_14 = field<&crc8, crc>;

using Cmd1st = std::tuple<MarkerField, CommandField1,
    DataField, DataField16, CrcField>;

using Cmd2nd = std::tuple<MarkerField, CommandField2,
    DataField, DataField32, CrcField>;

using Cmd3rd = std::tuple<MarkerField, CommandField3,
    DataField, DataField32, CrcField>;

using Cmd4th = std::tuple<MarkerField, CommandField1,
    DataField, DataField16, CrcField>;
