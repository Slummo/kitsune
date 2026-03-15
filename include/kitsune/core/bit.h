#pragma once

#include <stdint.h>
#include <stdbool.h>

static inline int32_t KS_CTZ32(uint32_t x) {
    if (x == 0) {
        return 32;
    }

    return __builtin_ctz(x);
}

static inline int32_t KS_CTZ64(uint64_t x) {
    if (x == 0) {
        return 64;
    }

    return __builtin_ctzll(x);
}

static inline int32_t KS_CLZ32(uint32_t x) {
    if (x == 0) {
        return 32;
    }

    return __builtin_clz(x);
}

static inline int32_t KS_CLZ64(uint64_t x) {
    if (x == 0) {
        return 64;
    }

    return __builtin_clzll(x);
}

static inline int32_t KS_POPCOUNT32(uint32_t x) {
    return __builtin_popcount(x);
}

static inline int32_t KS_POPCOUNT64(uint64_t x) {
    return __builtin_popcountll(x);
}

static inline uint32_t KS_BSWAP32(uint32_t x) {
    return __builtin_bswap32(x);
}

static inline uint64_t KS_BSWAP64(uint64_t x) {
    return __builtin_bswap64(x);
}

#define KS_CTZ(x) _Generic((x), uint32_t: KS_CTZ32, int32_t: KS_CTZ32, uint64_t: KS_CTZ64, int64_t: KS_CTZ64)(x)
#define KS_CLZ(x) _Generic((x), uint32_t: KS_CLZ32, int32_t: KS_CLZ32, uint64_t: KS_CLZ64, int64_t: KS_CLZ64)(x)

#define KS_POPCOUNT(x) \
    _Generic((x), uint32_t: KS_POPCOUNT32, int32_t: KS_POPCOUNT32, uint64_t: KS_POPCOUNT64, int64_t: KS_POPCOUNT64)(x)

#define KS_BSWAP(x) _Generic((x), uint32_t: KS_BSWAP32, uint64_t: KS_BSWAP64)(x)

#define KS_BIT(n) (1ULL << (n))
#define KS_BIT_CHECK(x, n) (!!((x) & KS_BIT(n)))
#define KS_BIT_SET(x, n) ((x) | KS_BIT(n))
#define KS_BIT_CLEAR(x, n) ((x) & ~KS_BIT(n))
#define KS_BIT_FLIP(x, n) ((x) ^ KS_BIT(n))

#define KS_BIT_MASK(len) ((len) >= 64 ? ~0ULL : (KS_BIT(len) - 1))

#define KS_BIT_FOREACH(i, mask) for (uint64_t _m = (mask); _m && ((i) = KS_CTZ(_m), 1); _m &= (_m - 1))
