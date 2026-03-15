#pragma once

#include <kitsune/core/bit.h>
#include <stddef.h>
#include <stdint.h>

#define KS_UNUSED __attribute__((unused))
#define KS_PACKED __attribute__((packed))
#define KS_NODISCARD __attribute__((warn_unused_result))
#define KS_DEPRECATED(msg) __attribute__((deprecated(msg)))
#define KS_ALIGNED(x) __attribute__(aligned((x)))
#define KS_LIKELY(x) __builtin_expect(!!(x), 1)
#define KS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define KS_FALLTROUGH __attribute__((falltrough))
#define KS_MAX(a, b)        \
    ({                      \
        typeof(a) _a = (a); \
        typeof(b) _b = (b); \
        _a > _b ? _a : _b;  \
    })
#define KS_MIN(a, b)        \
    ({                      \
        typeof(a) _a = (a); \
        typeof(b) _b = (b); \
        _a > _b ? _b : _a;  \
    })
#define KS_CLAMP(x, a, b)                   \
    ({                                      \
        typeof(a) _a = (a);                 \
        typeof(b) _b = (b);                 \
        typeof(x) _x = (x);                 \
        _x < _a ? _a : (_x > _b ? _b : _x); \
    })
#define KS_ABS(x)           \
    ({                      \
        typeof(x) _x = (x); \
        _x < 0 ? -_x : _x;  \
    })
#define KS_SWAP(a, b)      \
    do {                   \
        typeof(a) tmp = a; \
        (a) = (b);         \
        (b) = tmp;         \
    } while (0);
#define KS_ISPOW2(x) ((x) != 0 && ((((x) - 1) & (x)) == 0))
#define KS_ALIGN_DOWN(x, a) ((x) & ~((typeof(x))(a) - 1))
#define KS_ALIGN_UP(x, a) KS_ALIGN_DOWN((x) + (a) - 1, (a))
#define KS_NEXTPOW2(x) ((x <= 1) ? 1ULL : (KS_BIT(sizeof(x) * 8 - (size_t)KS_CLZ((x) - 1))))
#define KS_PTROFF(ptr, off) ((uint8_t*)(ptr) + (off))
#define KS_PTRDIFF(ptr1, ptr2) ((ptrdiff_t)((ptr1) - (ptr2)))
#define KS_PTRDIFF_ABS(ptr1, ptr2) ((size_t)((ptr1) - (ptr2)))
