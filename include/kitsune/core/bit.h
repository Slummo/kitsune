#pragma once

#include <stdint.h>
#include <stdbool.h>

static inline int kit_ctz32(uint32_t x) {
    if (x == 0) {
        return 32;
    }

    return __builtin_ctz(x);
}

static inline int kit_ctz64(uint64_t x) {
    if (x == 0) {
        return 64;
    }

    return __builtin_ctzll(x);
}

static inline int kit_popcount32(uint32_t x) {
    return __builtin_popcount(x);
}

static inline int kit_popcount64(uint64_t x) {
    return __builtin_popcountll(x);
}

static inline uint32_t kit_bswap32(uint32_t x) {
    return __builtin_bswap32(x);
}

static inline uint64_t kit_bswap64(uint64_t x) {
    return __builtin_bswap64(x);
}

#define kit_ctz(x) _Generic((x), uint32_t: kit_ctz32, int32_t: kit_ctz32, uint64_t: kit_ctz64, int64_t: kit_ctz64)(x)

#define kit_popcount(x)           \
    _Generic((x),                 \
        uint32_t: kit_popcount32, \
        int32_t: kit_popcount32,  \
        uint64_t: kit_popcount64, \
        int64_t: kit_popcount64)(x)

#define kit_bswap(x) _Generic((x), uint32_t: kit_bswap32, uint64_t: kit_bswap64)(x)

#define kit_bit(n) (1ULL << (n))
#define kit_bitcheck(x, n) (!!((x) & kit_bit(n)))
#define kit_bitset(x, n) ((x) | kit_bit(n))
#define kit_bitclear(x, n) ((x) & ~kit_bit(n))
#define kit_bitflip(x, n) ((x) ^ kit_bit(n))

#define kit_bitmask(len) ((len) >= 64 ? ~0ULL : (kit_bit(len) - 1))

#define kit_foreach_bit(i, mask) for (uint64_t _m = (mask); _m && ((i) = kit_ctz(_m), 1); _m &= (_m - 1))