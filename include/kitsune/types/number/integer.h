#ifndef KITSUNE_CORE_NUMBER_INTEGER_H
#define KITSUNE_CORE_NUMBER_INTEGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Unsigned types

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed types

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
typedef __int128 i128;
#endif

// Unsigned fast types

typedef uint_fast8_t uf8;
typedef uint_fast16_t uf16;
typedef uint_fast32_t uf32;
typedef uint_fast64_t uf64;

// Signed fast types

typedef int_fast8_t if8;
typedef int_fast16_t if16;
typedef int_fast32_t if32;
typedef int_fast64_t if64;

// Unsigned least types

typedef uint_least8_t ul8;
typedef uint_least16_t ul16;
typedef uint_least32_t ul32;
typedef uint_least64_t ul64;

// Signed least types

typedef int_least8_t il8;
typedef int_least16_t il16;
typedef int_least32_t il32;
typedef int_least64_t il64;

// Size types

typedef size_t usize;
typedef ptrdiff_t isize;

#endif