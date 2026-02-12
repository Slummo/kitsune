#pragma once

#include <kitsune/core/def.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Unsigned integers

ksAlias(uint8_t, U8);
ksAlias(uint8_t, Byte);
ksAlias(uint16_t, U16);
ksAlias(uint32_t, U32);
ksAlias(uint64_t, U64);
ksAlias(uint_fast8_t, UF8);
ksAlias(uint_fast16_t, UF16);
ksAlias(uint_fast32_t, UF32);
ksAlias(uint_fast64_t, UF64);
ksAlias(uint_least8_t, UL8);
ksAlias(uint_least16_t, UL16);
ksAlias(uint_least32_t, UL32);
ksAlias(uint_least64_t, UL64);
ksAlias(size_t, Size);

// Signed integers

ksAlias(int8_t, I8);
ksAlias(int16_t, I16);
ksAlias(int32_t, I32);
ksAlias(int64_t, I64);
ksAlias(int_fast8_t, IF8);
ksAlias(int_fast16_t, IF16);
ksAlias(int_fast32_t, IF32);
ksAlias(int_fast64_t, IF64);
ksAlias(int_least8_t, IL8);
ksAlias(int_least16_t, IL16);
ksAlias(int_least32_t, IL32);
ksAlias(int_least64_t, IL64);
ksAlias(ptrdiff_t, PtrDiff);

// Floats

ksAlias(float, F32);
ksAlias(double, F64);