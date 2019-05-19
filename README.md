## Description
This project intended to be in support of RPPS (Rapid Protocol Prototyping Solution - https://github.com/Bored66/RPPS ).
Instead of defining protocol using XMl files it should be possible to define everything just by using certain set of classes and
very little code generation (thru variadic macros) while compiler should do the rest.

Suppose we defne some structures like the following:
``` c++
constexpr char markerFldName[] {"Marker"};
constexpr char commandFldName[] {"Command"};
constexpr char dataFldName[] {"Data"};
constexpr char data16FldName[] {"Data16"};
constexpr char data32FldName[] {"Data32"};
constexpr char crcFldName[] {"CRC"};
extern inline uint8_t crc8(uint8_t*seq, size_t size);
template<uint8_t I>
  using byte_i_c = std::integral_constant<uint8_t, I>;
const auto marker_v = 0xAA;

using MarkerField = field<byte_i_c<marker_v>, markerFldName>;

using CommandField1 = field<byte_i_c<0x01>, commandFldName>;
using CommandField2 = field<byte_i_c<0x02>, commandFldName>;
// More of the same...

using DataField = field<uint8_t, dataFldName>;
using DataField16 = field<uint16_t, data16FldName>;
using DataField32 = field<uint32_t, data32FldName>;
using CrcField = crc_field<uint8_t, crcFldName, &crc8>;

using Cmd1st = std::tuple<MarkerField, CommandField1,
    DataField, DataField16, CrcField>;

using Cmd2nd = std::tuple<MarkerField, CommandField2,
    DataField, DataField32, CrcField>;

// More of the same...

```

### WARNING: 
This projects involves heavy use of meta-programming (templates and const expressions) and requires C++14 capable compiler.

