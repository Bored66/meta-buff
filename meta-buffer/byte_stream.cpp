#include "byte_stream.h"

byte_stream::byte_stream()
{}

byte_stream::byte_stream(size_t size)
{
    _storage.resize(size);
}

byte_stream::byte_stream(uint8_t *data, size_t size)
{
    append(data, size);
}

void byte_stream::append(uint8_t *data, size_t size)
{
    _storage.insert(_storage.end(), data, data+size);
}
