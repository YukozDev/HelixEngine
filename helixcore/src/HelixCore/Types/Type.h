#pragma once
#include <cstdint>

using hxByte = uint8_t;

using hxInt8 = int8_t;
using hxInt16 = int16_t;
using hxInt32 = int32_t;
using hxInt64 = int64_t;

using hxUInt8 = uint8_t;
using hxUInt16 = uint16_t;
using hxUInt32 = uint32_t;
using hxUInt64 = uint64_t;

using hxBool = bool;

using hxChar = char;

template<size_t Size>
using StaticString = char[Size]; // TODO: MEMORY

#define hxNoDiscard [[nodiscard]]
