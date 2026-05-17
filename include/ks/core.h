#ifndef KS_CORE_H
#define KS_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

/* God implementation macro */

#ifdef KITSUNE_IMPL
#define KS_CORE_IMPL
#define KS_IO_IMPL
#define KS_TIME_IMPL
#define KS_MATH_IMPL
#define KS_MEM_IMPL
#define KS_STRING_IMPL
#define KS_DS_IMPL
#endif

/* Concatenation */
#define _KS_CONCAT2(a, b) a##b
#define KS_CONCAT2(a, b) _KS_CONCAT2(a, b)
#define KS_CONCAT3(a, b, c) KS_CONCAT2(KS_CONCAT2(a, b), c)
#define KS_CONCAT4(a, b, c, d) KS_CONCAT2(KS_CONCAT2(a, b), KS_CONCAT2(c, d))
#define KS_CONCAT5(a, b, c, d, e) KS_CONCAT2(KS_CONCAT4(a, b, c, d), e)
#define KS_UNPACK(...) __VA_ARGS__

/* Namespaces */

// Templates
#define NS_TYPE(ns, name) KS_CONCAT3(ns, _, name)
#define NS_TEMPLATED_TYPE(ns, name, T) KS_CONCAT3(NS_TYPE(ns, name), _, T)

// Structs
#define NS_STRUCT_DECL(ns, name) typedef struct NS_TYPE(ns, name) NS_TYPE(ns, name)
#define NS_STRUCT_DEF(ns, name, ...) struct NS_TYPE(ns, name) __VA_ARGS__
#define NS_STRUCT(ns, name, ...) \
    NS_STRUCT_DECL(ns, name);    \
    NS_STRUCT_DEF(ns, name, __VA_ARGS__)
#define NS_TEMPLATED_STRUCT(ns, name, T, ...) \
    typedef struct NS_TEMPLATED_TYPE(ns, name, T) __VA_ARGS__ NS_TEMPLATED_TYPE(ns, name, T)

// Unions
#define NS_UNION_DECL(ns, name) typedef union NS_TYPE(ns, name) NS_TYPE(ns, name)
#define NS_UNION_DEF(ns, name, ...) union NS_TYPE(ns, name) __VA_ARGS__
#define NS_UNION(ns, name, ...) \
    NS_UNION_DECL(ns, name);    \
    NS_UNION_DEF(ns, name, __VA_ARGS__)
#define NS_TEMPLATED_UNION(ns, name, T, ...) \
    typedef union NS_TEMPLATED_TYPE(ns, name, T) __VA_ARGS__ NS_TEMPLATED_TYPE(ns, name, T)

// Methods and Enums
#define NS_TEMPLATED_METHOD(ns, name, action, T) KS_CONCAT5(NS_TYPE(ns, name), _, action, _, T)

#define NS_ENUM(ns, name, ...) typedef enum NS_TYPE(ns, name) __VA_ARGS__ NS_TYPE(ns, name)

// Functions
#define NS_FUNC(T, ns, name, ...) typedef T (*KS_CONCAT3(ns, _, name))(__VA_ARGS__)

/* Kitsune namespace */

#define KS_TYPE(name) NS_TYPE(ks, name)
#define KS_TEMPLATED_TYPE(name, T) NS_TEMPLATED_TYPE(ks, name, T)

#define KS_STRUCT_DECL(name) NS_STRUCT_DECL(ks, name)
#define KS_STRUCT_DEF(name, ...) NS_STRUCT_DEF(ks, name, __VA_ARGS__)
#define KS_STRUCT(name, ...) NS_STRUCT(ks, name, __VA_ARGS__)
#define KS_TEMPLATED_STRUCT(name, T, ...) NS_TEMPLATED_STRUCT(ks, name, T, __VA_ARGS__)

#define KS_UNION_DECL(name) NS_UNION_DECL(ks, name)
#define KS_UNION_DEF(name, ...) NS_UNION_DEF(ks, name, __VA_ARGS__)
#define KS_UNION(name, ...) NS_UNION(ks, name, __VA_ARGS__)
#define KS_TEMPLATED_UNION(name, T, ...) NS_TEMPLATED_UNION(ks, name, T, __VA_ARGS__)

#define KS_TEMPLATED_METHOD(name, action, T) NS_TEMPLATED_METHOD(ks, name, action, T)

#define KS_ENUM(name, ...) NS_ENUM(ks, name, __VA_ARGS__)

#define KS_FUNC(T, name, ...) NS_FUNC(T, ks, name, __VA_ARGS__)

/* Alias and using */

#define KS_ALIAS(oldT, newT) typedef oldT newT
#define KS_USING(T, usingT) typedef T usingT

/* Platform and compiler detection */

#if defined(__linux__)
#define KS_PLATFORM_LINUX 1
#define KS_PLATFORM_MACOS 0
#define KS_PLATFORM_WINDOWS 0
#elif defined(__APPLE__) && defined(__MACH__)
#define KS_PLATFORM_LINUX 0
#define KS_PLATFORM_MACOS 1
#define KS_PLATFORM_WINDOWS 0
#elif defined(_WIN32) || defined(_WIN64)
#define KS_PLATFORM_LINUX 0
#define KS_PLATFORM_MACOS 0
#define KS_PLATFORM_WINDOWS 1
#else
#error "Unsupported operating system"
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#define KS_COMPILER_GCC 1
#define KS_COMPILER_CLANG 0
#define KS_COMPILER_MSVC 0
#define KS_COMPILER_UNKNOWN 0
#define KS_COMPILER_NAME "GCC"
#elif defined(__clang__)
#define KS_COMPILER_GCC 0
#define KS_COMPILER_CLANG 1
#define KS_COMPILER_MSVC 0
#define KS_COMPILER_UNKNOWN 0
#define KS_COMPILER_NAME "Clang"
#elif defined(_MSC_VER)
#define KS_COMPILER_GCC 0
#define KS_COMPILER_CLANG 0
#define KS_COMPILER_MSVC 1
#define KS_COMPILER_UNKNOWN 0
#define KS_COMPILER_NAME "MSVC"
#else
#define KS_COMPILER_GCC 0
#define KS_COMPILER_CLANG 0
#define KS_COMPILER_MSVC 0
#define KS_COMPILER_UNKNOWN 1
#define KS_COMPILER_NAME "Unknown"
#endif

/* Shader library import/export */

#if KS_PLATFORM_WINDOWS
#if defined(KS_EXPORT_SHARED)
#define KS_API __declspec(dllexport)
#elif defined(KS_IMPORT_SHARED)
#define KS_API __declspec(dllimport)
#else
#define KS_API
#endif
#else
#if defined(KS_EXPORT_SHARED)
#define KS_API __attribute__((visibility("default")))
#else
#define KS_API
#endif
#endif

/* Thread local storage */

#ifndef KS_THREAD_LOCAL
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#define KS_THREAD_LOCAL _Thread_local
#elif KS_COMPILER_GCC || KS_COMPILER_CLANG
#define KS_THREAD_LOCAL __thread
#elif KS_COMPILER_MSVC
#define KS_THREAD_LOCAL __declspec(thread)
#else
#error "Your compiler does not support thread-local storage"
#endif
#endif

/* Attributes and hints */

// Unused & Nodiscard

#if KS_COMPILER_MSVC
#define KS_UNUSED
#else
#define KS_UNUSED __attribute__((unused))
#endif

#define KS_MARK_UNUSED(x) (void)(x)

#if KS_COMPILER_MSVC
#define KS_NODISCARD _Check_return_
#define KS_DEPRECATED(msg) __declspec(deprecated(msg))
#define KS_FALLTHROUGH
#else
#define KS_NODISCARD __attribute__((warn_unused_result))
#define KS_DEPRECATED(msg) __attribute__((deprecated(msg)))
#define KS_FALLTHROUGH __attribute__((fallthrough))
#endif

// Alignment

#if KS_COMPILER_MSVC
#define KS_ALIGNED(x) __declspec(align(x))
#else
#define KS_ALIGNED(x) __attribute__((aligned(x)))
#endif

// Packed structs

#if KS_COMPILER_MSVC
#define KS_PACKED
// Use #pragma pack(push, 1), #pragma pack(pop) on MSVC
#else
#define KS_PACKED __attribute__((packed))
#endif

// Branch prediction

#if KS_COMPILER_GCC || KS_COMPILER_CLANG
#define KS_LIKELY(x) __builtin_expect(!!(x), 1)
#define KS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define KS_SIMD_HINT _Pragma("GCC ivdep")
#else
#define KS_LIKELY(x) (x)
#define KS_UNLIKELY(x) (x)
#define KS_SIMD_HINT __pragma(loop(ivdep))
#endif

// Force/prevent inlining

#if KS_COMPILER_MSVC
#define KS_FORCE_INLINE __forceinline
#define KS_NO_INLINE __declspec(noinline)
#else
#define KS_FORCE_INLINE inline __attribute__((always_inline))
#define KS_NO_INLINE __attribute__((noinline))
#endif

/* Utility math macros */

#if KS_COMPILER_GCC || KS_COMPILER_CLANG
#define KS_TYPEOF __typeof__
#define KS_MAX(a, b)           \
    ({                         \
        KS_TYPEOF(a) _a = (a); \
        KS_TYPEOF(b) _b = (b); \
        _a > _b ? _a : _b;     \
    })
#define KS_MIN(a, b)           \
    ({                         \
        KS_TYPEOF(a) _a = (a); \
        KS_TYPEOF(b) _b = (b); \
        _a > _b ? _b : _a;     \
    })
#define KS_CLAMP(x, a, b)                   \
    ({                                      \
        KS_TYPEOF(a) _a = (a);              \
        KS_TYPEOF(b) _b = (b);              \
        KS_TYPEOF(x) _x = (x);              \
        _x < _a ? _a : (_x > _b ? _b : _x); \
    })
#define KS_ABS(x)              \
    ({                         \
        KS_TYPEOF(x) _x = (x); \
        _x < 0 ? -_x : _x;     \
    })
#define KS_LERP(a, b, t)        \
    ({                          \
        KS_TYPEOF(a) _a = (a);  \
        KS_TYPEOF(b) _b = (b);  \
        KS_TYPEOF(t) _t = (t);  \
        _a * (1 - _t) + _b* _t; \
    })
#define KS_SMOOTHSTEP(edge0, edge1, x)                                       \
    ({                                                                       \
        KS_TYPEOF(edge0) _edge0 = (edge0);                                   \
        KS_TYPEOF(edge1) _edge1 = (edge1);                                   \
        KS_TYPEOF(x) _x = (x);                                               \
        KS_TYPEOF(x) _t = KS_CLAMP((_x - _edge0) / (_edge1 - _edge0), 0, 1); \
        _t * _t * (3 - 2 * _t);                                              \
    })
#define KS_SIGN(x)             \
    ({                         \
        KS_TYPEOF(x) _x = (x); \
        (_x > 0) - (_x < 0);   \
    })
#define KS_STEP(edge, x)                \
    ({                                  \
        KS_TYPEOF(edge) _edge = (edge); \
        KS_TYPEOF(x) _x = (x);          \
        _x < _edge ? 0 : 1;             \
    })
#define KS_MAP(x, omin, omax, nmin, nmax)                         \
    ({                                                            \
        KS_TYPEOF(x) _x = (x);                                    \
        KS_TYPEOF(omin) _omin = (omin);                           \
        KS_TYPEOF(omax) _omax = (omax);                           \
        KS_TYPEOF(nmin) _nmin = (nmin);                           \
        KS_TYPEOF(nmax) _nmax = (nmax);                           \
        _nmin + (_x - _omin) * (_nmax - _nmin) / (_omax - _omin); \
    })
#define KS_SATURATE(x)                  \
    ({                                  \
        KS_TYPEOF(x) _x = (x);          \
        _x < 0 ? 0 : (_x > 1 ? 1 : _x); \
    })
#define KS_FLOOR(x) floor(x)
#define KS_CEIL(x) ceil(x)
#define KS_FRACT(x)            \
    ({                         \
        KS_TYPEOF(x) _x = (x); \
        _x - KS_FLOOR(_x);     \
    })
#define KS_WRAP(x, y)               \
    ({                              \
        KS_TYPEOF(x) _x = (x);      \
        KS_TYPEOF(y) _y = (y);      \
        _x - _y* KS_FLOOR(_x / _y); \
    })
#define KS_SWAP(a, b)         \
    do {                      \
        KS_TYPEOF(a) tmp = a; \
        (a) = (b);            \
        (b) = tmp;            \
    } while (0)
#else
#define KS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define KS_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define KS_CLAMP(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#define KS_ABS(x) (((x) < 0) ? -(x) : (x))
#define KS_LERP(a, b, t) ((a) * (1 - (t)) + (b) * (t))
#define KS_SIGN(x) (((x) > 0) - ((x) < 0))
#define KS_MAP(x, omin, omax, nmin, nmax) ((nmin) + ((x) - (omin)) * ((nmax) - (nmin)) / ((omax) - (omin)))
#define KS_SATURATE(x) (((x) < 0) ? 0 : (((x) > 1) ? 1 : (x)))
#define KS_FLOOR(x) floor(x)
#define KS_CEIL(x) ceil(x)
#define KS_FRACT(x) ((x) - KS_FLOOR(x))
#define KS_WRAP(x, y) ((x) - (y) * KS_FLOOR((x) / (y)))
#define KS_SWAP_TYPED(type, a, b) \
    do {                          \
        type tmp = (a);           \
        (a) = (b);                \
        (b) = tmp;                \
    } while (0)
#endif

/* Utility memory macros */

#define KS_ISPOW2(x) ((x) != 0 && ((((x) - 1) & (x)) == 0))
#define KS_ALIGN_DOWN(x, a) ((x) & ~((size_t)(a) - 1))
#define KS_ALIGN_UP(x, a) KS_ALIGN_DOWN((x) + (a) - 1, (a))
#define KS_NEXTPOW2(x) ((x <= 1) ? 1ULL : (1ULL << (sizeof(x) * 8 - (size_t)KS_CLZ((x) - 1))))

#define KS_PTROFF(ptr, off) ((uint8_t*)(ptr) + (size_t)(off))
#define KS_PTRDIFF(ptr1, ptr2) ((ptrdiff_t)((intptr_t)(ptr1) - (intptr_t)(ptr2)))
#define KS_PTRDIFF_ABS(ptr1, ptr2) ((size_t)((uintptr_t)(ptr1) - (uintptr_t)(ptr2)))

/* Logging */

#define KSERR 0
#define KSWARN 1
#define KSINFO 2
#define KSDEBUG 3

KS_API extern int ks_log_level;

/**
 * @brief Print a message to stdout
 *
 */
#define KS_PRINT_OUT(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/**
 * @brief Print a message and eventually errno to stderr
 *
 */
#define KS_PRINT_ERR(fmt, ...)               \
    do {                                     \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        if (errno) {                         \
            perror(". ERR");                 \
            errno = 0;                       \
        } else {                             \
            fprintf(stderr, "\n");           \
        }                                    \
    } while (0)

static inline void ks_log_init(void) {
    char* env = getenv("KSLOGLVL");
    if (env) {
        ks_log_level = strtol(env, NULL, 10);
    }
}

#define ks_log(type, fmt, ...)                                                          \
    do {                                                                                \
        if (ks_log_level >= type) {                                                     \
            switch (type) {                                                             \
                case KSERR:                                                             \
                    KS_PRINT_ERR("{-}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                case KSWARN:                                                            \
                    KS_PRINT_OUT("{~}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                case KSINFO:                                                            \
                    KS_PRINT_OUT("{+}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                case KSDEBUG:                                                           \
                    KS_PRINT_OUT("{*}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                default:                                                                \
                    break;                                                              \
            }                                                                           \
        }                                                                               \
    } while (0)

#define ks_print(fmt, ...) KS_PRINT_OUT(fmt, ##__VA_ARGS__)

/* Debug and assert */

#if KS_COMPILER_MSVC
#define KS_DEBUGBREAK() __debugbreak()
#elif KS_COMPILER_GCC || KS_COMPILER_CLANG
#if defined(__x86_64__) || defined(__i386__)
#define KS_DEBUGBREAK() __asm__ volatile("int $3")
#elif defined(__arm__) || defined(__aarch64__)
#define KS_DEBUGBREAK() __asm__ volatile(".inst 0xd4200000")
#else
#include <signal.h>
#define KS_DEBUGBREAK() raise(SIGTRAP)
#endif
#else
#define KS_DEBUGBREAK()
#endif

#define KS_PANIC(fmt, ...)                                                                                     \
    do {                                                                                                       \
        KS_PRINT_ERR("Program panicked at %s:%d in %s():\n\t" fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        KS_DEBUGBREAK();                                                                                       \
        abort();                                                                                               \
    } while (0);

#ifdef NDEBUG
#define KS_ASSERT(condition, message) ((void)0)
#else
#define KS_ASSERT(condition, message)                                   \
    do {                                                                \
        if (KS_UNLIKELY(!(condition))) {                                \
            KS_PANIC("Assertion failed (" #condition "): %s", message); \
        }                                                               \
    } while (0)
#endif

#define KS_ASSERT_NONNULL_ARGS(x) KS_ASSERT(x, "Null arguments");

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
// ISO C11
#define KS_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
// ISO C89/C99 creates an array of negative size if condition is false,
// forcing a compile error
#define KS_STATIC_ASSERT(cond, msg) typedef char KS_CONCAT(static_assertion_failed_, __LINE__)[(cond) ? 1 : -1]
#endif

/* Return codes */

KS_ENUM(code, {KS_OK = 0, KS_ERR_GENERIC = -1, KS_ERR_INVALID = -2, KS_ERR_OOM = -3, KS_ERR_NOT_FOUND = -4,
               KS_ERR_DUPLICATE = -5, KS_ERR_EMPTY = -6, KS_ERR_FULL = -7, KS_ERR_BOUNDS = -8});

static inline const char* ks_res_str(int res) {
    switch (res) {
        case KS_OK:
            return "Success";
        case KS_ERR_GENERIC:
            return "Generic error";
        case KS_ERR_INVALID:
            return "Invalid argument";
        case KS_ERR_OOM:
            return "Out of memory";
        case KS_ERR_NOT_FOUND:
            return "Not found";
        case KS_ERR_DUPLICATE:
            return "Duplicate entry";
        case KS_ERR_EMPTY:
            return "Container empty";
        case KS_ERR_FULL:
            return "Container full";
        case KS_ERR_BOUNDS:
            return "Index out of bounds";
        default:
            return "Unknown error";
    }
}

/* Bit manipulation */

#if KS_COMPILER_MSVC
#include <intrin.h>

static KS_FORCE_INLINE int32_t KS_CTZ32(uint32_t x) {
    if (x == 0) {
        return 32;
    }
    unsigned long index;
    _BitScanForward(&index, x);
    return (int32_t)index;
}

static KS_FORCE_INLINE int32_t KS_CTZ64(uint64_t x) {
    if (x == 0) {
        return 64;
    }
    unsigned long index;
    _BitScanForward64(&index, x);
    return (int32_t)index;
}

static KS_FORCE_INLINE int32_t KS_CLZ32(uint32_t x) {
    if (x == 0) {
        return 32;
    }
    unsigned long index;
    _BitScanReverse(&index, x);
    return (int32_t)(31 - index);
}

static KS_FORCE_INLINE int32_t KS_CLZ64(uint64_t x) {
    if (x == 0) {
        return 64;
    }
    unsigned long index;
    _BitScanReverse64(&index, x);
    return (int32_t)(63 - index);
}

static KS_FORCE_INLINE int32_t KS_POPCOUNT32(uint32_t x) {
    return (int32_t)__popcnt(x);
}

static KS_FORCE_INLINE int32_t KS_POPCOUNT64(uint64_t x) {
    return (int32_t)__popcnt64(x);
}

static KS_FORCE_INLINE uint32_t KS_BSWAP32(uint32_t x) {
    return _byteswap_ulong(x);
}

static KS_FORCE_INLINE uint64_t KS_BSWAP64(uint64_t x) {
    return _byteswap_uint64(x);
}
#else
static KS_FORCE_INLINE int32_t KS_CTZ32(uint32_t x) {
    if (x == 0) {
        return 32;
    }
    return __builtin_ctz(x);
}

static KS_FORCE_INLINE int32_t KS_CTZ64(uint64_t x) {
    if (x == 0) {
        return 64;
    }
    return __builtin_ctzll(x);
}

static KS_FORCE_INLINE int32_t KS_CLZ32(uint32_t x) {
    if (x == 0) {
        return 32;
    }
    return __builtin_clz(x);
}

static KS_FORCE_INLINE int32_t KS_CLZ64(uint64_t x) {
    if (x == 0) {
        return 64;
    }
    return __builtin_clzll(x);
}

static KS_FORCE_INLINE int32_t KS_POPCOUNT32(uint32_t x) {
    return __builtin_popcount(x);
}

static KS_FORCE_INLINE int32_t KS_POPCOUNT64(uint64_t x) {
    return __builtin_popcountll(x);
}

static KS_FORCE_INLINE uint32_t KS_BSWAP32(uint32_t x) {
    return __builtin_bswap32(x);
}

static KS_FORCE_INLINE uint64_t KS_BSWAP64(uint64_t x) {
    return __builtin_bswap64(x);
}
#endif

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

/* Optional */

#define _KS_OPT(T) KS_TEMPLATED_TYPE(opt, T)
#define _KS_OPT_FN(action, T) KS_TEMPLATED_METHOD(opt, action, T)

#define KS_OPTIONAL(T)                                                             \
    KS_TEMPLATED_STRUCT(opt, T, {                                                  \
        bool has_value;                                                            \
        T value;                                                                   \
    });                                                                            \
                                                                                   \
    KS_UNUSED static inline _KS_OPT(T) _KS_OPT_FN(some, T)(T value) {              \
        return (_KS_OPT(T)){.has_value = true, .value = value};                    \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline _KS_OPT(T) _KS_OPT_FN(none, T)(void) {                 \
        return (_KS_OPT(T)){.has_value = false};                                   \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline bool _KS_OPT_FN(is_some, T)(_KS_OPT(T) * self) {       \
        KS_ASSERT(self, "self is NULL");                                           \
        return self->has_value;                                                    \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline bool _KS_OPT_FN(is_none, T)(_KS_OPT(T) * self) {       \
        KS_ASSERT(self, "self is NULL");                                           \
        return !self->has_value;                                                   \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline T* _KS_OPT_FN(ptr, T)(_KS_OPT(T) * self) {             \
        KS_ASSERT(self, "self is NULL");                                           \
        return _KS_OPT_FN(is_some, T)(self) ? &self->value : NULL;                 \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline T _KS_OPT_FN(unwrap_or, T)(_KS_OPT(T) * self, T def) { \
        KS_ASSERT(self, "self is NULL");                                           \
        return _KS_OPT_FN(is_some, T)(self) ? self->value : def;                   \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline T _KS_OPT_FN(unwrap, T)(_KS_OPT(T) * self) {           \
        KS_ASSERT(self, "self is NULL");                                           \
        KS_ASSERT(_KS_OPT_FN(is_some, T)(self), "self is NONE");                   \
        return self->value;                                                        \
    }

/* Result */

#define _KS_RES_T(ok_t, err_t) KS_CONCAT3(ok_t, _, err_t)
#define _KS_RES(ok_t, err_t) KS_TEMPLATED_TYPE(res, _KS_RES_T(ok_t, err_t))
#define _KS_RES_FN(action, ok_t, err_t) KS_TEMPLATED_METHOD(res, action, _KS_RES_T(ok_t, err_t))

#define KS_RESULT(ok_t, err_t)                                                                    \
    KS_TEMPLATED_STRUCT(res, _KS_RES_T(ok_t, err_t), {                                            \
        bool is_ok;                                                                               \
        union {                                                                                   \
            ok_t ok;                                                                              \
            err_t err;                                                                            \
        };                                                                                        \
    });                                                                                           \
                                                                                                  \
    KS_UNUSED static inline _KS_RES(ok_t, err_t) _KS_RES_FN(ok, ok_t, err_t)(ok_t value) {        \
        return (_KS_RES(ok_t, err_t)){.is_ok = true, .ok = value};                                \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline _KS_RES(ok_t, err_t) _KS_RES_FN(err, ok_t, err_t)(err_t value) {      \
        return (_KS_RES(ok_t, err_t)){.is_ok = false, .err = value};                              \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline bool _KS_RES_FN(is_ok, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) {    \
        KS_ASSERT(self, "self is NULL");                                                          \
        return self->is_ok;                                                                       \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline bool _KS_RES_FN(is_err, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) {   \
        KS_ASSERT(self, "self is NULL");                                                          \
        return !self->is_ok;                                                                      \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline ok_t _KS_RES_FN(get_ok, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) {   \
        KS_ASSERT(self, "self is NULL");                                                          \
        KS_ASSERT(self->is_ok, "self is ERR");                                                    \
        return self->ok;                                                                          \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline err_t _KS_RES_FN(get_err, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) { \
        KS_ASSERT(self, "self is NULL");                                                          \
        KS_ASSERT(!self->is_ok, "self is OK");                                                    \
        return self->err;                                                                         \
    }

#endif  // KS_CORE_H

#if defined(KS_CORE_IMPL) && !defined(KS_CORE_IMPL_DONE)
#define KS_CORE_IMPL_DONE

KS_API int ks_log_level = KSINFO;

#endif  // KS_CORE_IMPL