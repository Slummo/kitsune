#pragma once

#include <stdint.h>
#include <stdbool.h>

static inline int ksCtz32(uint32_t x) {
    if (x == 0) {
        return 32;
    }

    return __builtin_ctz(x);
}

static inline int ksCtz64(uint64_t x) {
    if (x == 0) {
        return 64;
    }

    return __builtin_ctzll(x);
}

static inline int ksPopcount32(uint32_t x) {
    return __builtin_popcount(x);
}

static inline int ksPopcount64(uint64_t x) {
    return __builtin_popcountll(x);
}

static inline uint32_t ksBswap32(uint32_t x) {
    return __builtin_bswap32(x);
}

static inline uint64_t ksBswap64(uint64_t x) {
    return __builtin_bswap64(x);
}

#define ksCtz(x) _Generic((x), uint32_t: ksCtz32, int32_t: ksCtz32, uint64_t: ksCtz64, int64_t: ksCtz64)(x)

#define ksPopcount(x) \
    _Generic((x), uint32_t: ksPopcount32, int32_t: ksPopcount32, uint64_t: ksPopcount64, int64_t: ksPopcount64)(x)

#define ksBswap(x) _Generic((x), uint32_t: ksBswap32, uint64_t: ksBswap64)(x)

#define ksBit(n) (1ULL << (n))
#define ksBitCheck(x, n) (!!((x) & ksBit(n)))
#define ksBitSet(x, n) ((x) | ksBit(n))
#define ksBitClear(x, n) ((x) & ~ksBit(n))
#define ksBitFlip(x, n) ((x) ^ ksBit(n))

#define ksBitMask(len) ((len) >= 64 ? ~0ULL : (ksBit(len) - 1))

#define ksBitForeach(i, mask) for (uint64_t _m = (mask); _m && ((i) = ksCtz(_m), 1); _m &= (_m - 1))