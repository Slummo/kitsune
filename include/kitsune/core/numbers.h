#pragma once

#include <kitsune/core/def.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Unsigned integers

ksUsing(uint8_t, u8);
ksUsing(uint8_t, byte);
ksUsing(uint16_t, u16);
ksUsing(uint32_t, u32);
ksUsing(uint64_t, u64);
ksUsing(uint_fast8_t, uf8);
ksUsing(uint_fast16_t, uf16);
ksUsing(uint_fast32_t, uf32);
ksUsing(uint_fast64_t, uf64);
ksUsing(uint_least8_t, ul8);
ksUsing(uint_least16_t, ul16);
ksUsing(uint_least32_t, ul32);
ksUsing(uint_least64_t, ul64);
ksUsing(size_t, size);

// Signed integers

ksUsing(int8_t, i8);
ksUsing(int16_t, i16);
ksUsing(int32_t, i32);
ksUsing(int64_t, i64);
ksUsing(int_fast8_t, if8);
ksUsing(int_fast16_t, if16);
ksUsing(int_fast32_t, if32);
ksUsing(int_fast64_t, if64);
ksUsing(int_least8_t, il8);
ksUsing(int_least16_t, il16);
ksUsing(int_least32_t, il32);
ksUsing(int_least64_t, il64);
ksUsing(ptrdiff_t, ptrdiff);

// Floats

ksUsing(float, f32);
ksUsing(double, f64);
