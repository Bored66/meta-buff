#pragma once
#include <cstdint>
#include <vector>

class byte_stream
{
    std::vector<uint8_t> _storage;
public:
    byte_stream();
    byte_stream(size_t size);
    byte_stream(uint8_t* data, size_t size);
    void append(uint8_t* data, size_t size);
    uint8_t &operator[](const std::size_t t_pos)
    {
      return _storage[t_pos];
    }
    size_t size()
    { return _storage.size(); }
};
