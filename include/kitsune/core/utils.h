#pragma once

#include <kitsune/core/bit.h>
#include <stddef.h>
#include <stdint.h>

#define ksUnused __attribute__((unused))
#define ksPacked __attribute__((packed))
#define ksNoDiscard __attribute__((warn_unused_result))
#define ksDeprecated(msg) __attribute__((deprecated(msg)))
#define ksAligned(x) __attribute__(aligned((x)))
#define ksLikely(x) __builtin_expect(!!(x), 1)
#define ksUnlikely(x) __builtin_expect(!!(x), 0)
#define ksFalltrough __attribute__((falltrough))
#define ksMax(a, b)         \
    ({                      \
        typeof(a) _a = (a); \
        typeof(b) _b = (b); \
        _a > _b ? _a : _b;  \
    })
#define ksMin(a, b)         \
    ({                      \
        typeof(a) _a = (a); \
        typeof(b) _b = (b); \
        _a > _b ? _b : _a;  \
    })
#define ksClamp(x, a, b)                    \
    ({                                      \
        typeof(a) _a = (a);                 \
        typeof(b) _b = (b);                 \
        typeof(x) _x = (x);                 \
        _x < _a ? _a : (_x > _b ? _b : _x); \
    })
#define ksAbs(x)            \
    ({                      \
        typeof(x) _x = (x); \
        _x < 0 ? -_x : _x;  \
    })
#define ksSwap(a, b)       \
    do {                   \
        typeof(a) tmp = a; \
        (a) = (b);         \
        (b) = tmp;         \
    } while (0);
#define ksIsPow2(x) ((x) != 0 && ((((x) - 1) & (x)) == 0))
#define ksAlignDown(x, a) ((x) & ~((typeof(x))(a) - 1))
#define ksAlignUp(x, a) ksAlignDown((x) + (a) - 1, (a))
#define ksNextPow2(x) ((x <= 1) ? 1ULL : (ksBit(sizeof(x) * 8 - (size_t)ksClz((x) - 1))))
#define ksPtrOffset(ptr, off) ((uint8_t*)(ptr) + (off))
#define ksPtrDiff(ptr1, ptr2) ((ptrdiff_t)((ptr1) - (ptr2)))
#define ksPtrAbsDiff(ptr1, ptr2) ((size_t)((ptr1) - (ptr2)))
