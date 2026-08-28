#ifndef KS_CORE_H
#define KS_CORE_H

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

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

#define KS_EXPAND(x) x
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

/* Platform, language and compiler detection */

// Platform

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

#if KS_PLATFORM_WINDOWS
#define KS_PLATFORM_NAME "Windows"
#elif KS_PLATFORM_MACOS
#define KS_PLATFORM_NAME "macOS"
#elif KS_PLATFORM_LINUX
#define KS_PLATFORM_NAME "Linux"
#else
#define KS_PLATFORM_NAME "Unknown OS"
#endif

// Language

#if defined(__cplusplus)
#define KS_LANGUAGE_CXX 1
#define KS_LANGUAGE_C 0
#define KS_CXX_VERSION __cplusplus
#define KS_C_VERSION 0
#else
#define KS_LANGUAGE_CXX 0
#define KS_LANGUAGE_C 1
#define KS_CXX_VERSION 0
#if defined(__STDC_VERSION__)
#define KS_C_VERSION __STDC_VERSION__
#else
#define KS_C_VERSION 198900L  // C89/C90 fallback
#endif
#endif

#if KS_LANGUAGE_CXX
#define KS_EXTERN_C_BEGIN extern "C" {
#define KS_EXTERN_C_END }
#else
#define KS_EXTERN_C_BEGIN
#define KS_EXTERN_C_END
#endif

#if KS_LANGUAGE_CXX
#define KS_LANGUAGE_NAME "C++"
#define KS_LANGUAGE_STD KS_CXX_VERSION
#else
#define KS_LANGUAGE_NAME "C"
#define KS_LANGUAGE_STD KS_C_VERSION
#endif

// Compiler and version

#if defined(__clang__)
#define KS_COMPILER_CLANG 1
#define KS_COMPILER_GCC 0
#define KS_COMPILER_MSVC 0
#define KS_COMPILER_NAME "Clang"
#define KS_COMPILER_VERSION_MAJOR __clang_major__
#define KS_COMPILER_VERSION_MINOR __clang_minor__
#define KS_COMPILER_VERSION_PATCH __clang_patchlevel__
#elif defined(__GNUC__) || defined(__GNUG__)
#define KS_COMPILER_CLANG 0
#define KS_COMPILER_GCC 1
#define KS_COMPILER_MSVC 0
#define KS_COMPILER_NAME "GCC"
#define KS_COMPILER_VERSION_MAJOR __GNUC__
#define KS_COMPILER_VERSION_MINOR __GNUC_MINOR__
#define KS_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#define KS_COMPILER_CLANG 0
#define KS_COMPILER_GCC 0
#define KS_COMPILER_MSVC 1
#define KS_COMPILER_NAME "MSVC"
#define KS_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
#define KS_COMPILER_VERSION_MINOR (_MSC_VER % 100)
#define KS_COMPILER_VERSION_PATCH 0
#else
#define KS_COMPILER_CLANG 0
#define KS_COMPILER_GCC 0
#define KS_COMPILER_MSVC 0
#define KS_COMPILER_NAME "Unknown"
#define KS_COMPILER_VERSION_MAJOR 0
#define KS_COMPILER_VERSION_MINOR 0
#define KS_COMPILER_VERSION_PATCH 0
#endif

/* Shared library import/export */

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

/* TLS Thread-local storage */

#ifndef KS_THREAD_LOCAL
#if KS_LANGUAGE_CXX && KS_CXX_VERSION >= 201103L
#define KS_THREAD_LOCAL thread_local
#elif KS_LANGUAGE_C && KS_C_VERSION >= 201112L && !defined(__STDC_NO_THREADS__)
#define KS_THREAD_LOCAL _Thread_local
#elif KS_COMPILER_GCC || KS_COMPILER_CLANG
#define KS_THREAD_LOCAL __thread
#elif KS_COMPILER_MSVC
#define KS_THREAD_LOCAL __declspec(thread)
#else
#error "Your compiler does not support thread-local storage"
#endif
#endif

/* Atomics */

#ifndef KS_ATOMIC_H
#define KS_ATOMIC_H

#if KS_LANGUAGE_CXX && KS_CXX_VERSION >= 201103L
#include <atomic>
#define KS_ATOMIC_INT std::atomic<int>
#define KS_ATOMIC_LOAD(ptr) (ptr)->load(std::memory_order_relaxed)
#define KS_ATOMIC_STORE(ptr, val) (ptr)->store((val), std::memory_order_relaxed)
#elif KS_LANGUAGE_C && KS_C_VERSION >= 201112L && !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#define KS_ATOMIC_INT _Atomic int
#define KS_ATOMIC_LOAD(ptr) atomic_load_explicit((ptr), memory_order_relaxed)
#define KS_ATOMIC_STORE(ptr, val) atomic_store_explicit((ptr), (val), memory_order_relaxed)
#elif KS_COMPILER_GCC || KS_COMPILER_CLANG
#define KS_ATOMIC_INT volatile int
#define KS_ATOMIC_LOAD(ptr) __atomic_load_n((ptr), __ATOMIC_RELAXED)
#define KS_ATOMIC_STORE(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_RELAXED)
#elif KS_COMPILER_MSVC
#define KS_ATOMIC_INT volatile long
long _InterlockedExchange(long volatile* Target, long Value);
#pragma intrinsic(_InterlockedExchange)
#define KS_ATOMIC_LOAD(ptr) (*(ptr))
#define KS_ATOMIC_STORE(ptr, val) _InterlockedExchange((long volatile*)(ptr), (long)(val))
#else
#error "Compiler does not support atomics!"
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

// Float epsilons

#define KS_FEPS_MACHINE FLT_EPSILON
#define KS_FEPS_MATH 1e-5f
#define KS_FEPS_PHYS 1e-4f
#define KS_FEPS_RENDER 1e-3f

#define KS_FZERO(a, eps) (fabsf(a) <= (eps))
#define KS_FNZERO(a, eps) (fabsf(a) > (eps))
#define KS_FEQ(a, b, eps) (fabsf((a) - (b)) <= (eps))
#define KS_FNEQ(a, b, eps) (fabsf((a) - (b)) > (eps))
#define KS_FLESS(a, b, eps) ((a) < ((b) - (eps)))
#define KS_FLESSEQ(a, b, eps) ((a) <= ((b) + (eps)))
#define KS_FGREATER(a, b, eps) ((a) > ((b) + (eps)))
#define KS_FGREATEREQ(a, b, eps) ((a) >= ((b) - (eps)))

// Double epsilons

#define KS_DEPS_MACHINE DBL_EPSILON
#define KS_DEPS_MATH 1e-10
#define KS_DEPS_PHYS 1e-7
#define KS_DEPS_RENDER 1e-3

#define KS_DZERO(a, eps) (fabs(a) <= (eps))
#define KS_DNZERO(a, eps) (fabs(a) > (eps))
#define KS_DEQ(a, b, eps) (fabs((a) - (b)) <= (eps))
#define KS_DNEQ(a, b, eps) (fabs((a) - (b)) > (eps))
#define KS_DLESS(a, b, eps) ((a) < ((b) - (eps)))
#define KS_DLESSEQ(a, b, eps) ((a) <= ((b) + (eps)))
#define KS_DGREATER(a, b, eps) ((a) > ((b) + (eps)))
#define KS_DGREATEREQ(a, b, eps) ((a) >= ((b) - (eps)))

/* Utility memory macros */

#define KS_ISPOW2(x) ((x) != 0 && ((((x) - 1) & (x)) == 0))
#define KS_ALIGN_DOWN(x, a) ((x) & ~((size_t)(a) - 1))
#define KS_ALIGN_UP(x, a) KS_ALIGN_DOWN((x) + (a) - 1, (a))
#define KS_NEXTPOW2(x) ((x <= 1) ? 1ULL : (1ULL << (sizeof(x) * 8 - (size_t)KS_CLZ((x) - 1))))

#define KS_PTROFF(ptr, off) ((uint8_t*)(ptr) + (size_t)(off))
#define KS_PTROFF_UNCAST(ptr, off) ((ptr) + (size_t)(off))
#define KS_PTRDIFF(ptr1, ptr2) ((ptrdiff_t)((intptr_t)(ptr1) - (intptr_t)(ptr2)))
#define KS_PTRDIFF_ABS(ptr1, ptr2) ((size_t)((uintptr_t)(ptr1) - (uintptr_t)(ptr2)))

/* Return codes and status */

KS_ENUM(code, {KS_OK = 0, KS_ERR_GENERIC = -1, KS_ERR_INVALID = -2, KS_ERR_OOM = -3, KS_ERR_NOT_FOUND = -4,
               KS_ERR_DUPLICATE = -5, KS_ERR_EMPTY = -6, KS_ERR_FULL = -7, KS_ERR_BOUNDS = -8});

KS_STRUCT(status, {
    ks_code code;
    const char* msg;
});

#define _ks_status_1(code) ((ks_status){code, ""})
#define _ks_status_2(code, msg) ((ks_status){code, msg})

#define _KS_STATUS_GET_MACRO(_1, _2, NAME, ...) NAME

#define KS_STATUS(...) KS_EXPAND(_KS_STATUS_GET_MACRO(__VA_ARGS__, _ks_status_2, _ks_status_1)(__VA_ARGS__))

KS_EXTERN_C_BEGIN

KS_API const char* ks_code_to_str(ks_code code);
KS_API const char* ks_status_to_str(ks_status status);

KS_EXTERN_C_END

static inline void ks_status_fmt(ks_status status, char* out_buf, size_t buf_size) {
    if (!out_buf || buf_size == 0) {
        return;
    }

    const char* code_str = ks_code_to_str(status.code);

    if (status.msg && status.msg[0] != '\0') {
        snprintf(out_buf, buf_size, "%s: %s", code_str, status.msg);
    } else {
        snprintf(out_buf, buf_size, "%s", code_str);
    }
}

/* Logging */

KS_ENUM(log_level_enum, {
                            KSERR = 0,
                            KSWARN = 1,
                            KSINFO = 2,
                            KSDEBUG = 3,
                        });

KS_EXTERN_C_BEGIN

KS_API extern KS_ATOMIC_INT ks_log_level;
KS_API void ks_log_init(void);
KS_API void ks_log_impl(int type, const char* file, int line, const char* fmt, ...);

KS_EXTERN_C_END

/**
 * @brief Print a message to stdout
 *
 */
#define KS_PRINT_OUT(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/**
 * @brief Print a message and eventually errno to stderr
 *
 */
#define KS_PRINT_ERR(fmt, ...)                              \
    do {                                                    \
        int _err = errno;                                   \
        fprintf(stderr, fmt, ##__VA_ARGS__);                \
        if (_err) {                                         \
            fprintf(stderr, ". ERR: %s\n", strerror(_err)); \
        } else {                                            \
            fprintf(stderr, "\n");                          \
        }                                                   \
        errno = _err;                                       \
    } while (0)

#define ks_log(type, fmt, ...)                                             \
    do {                                                                   \
        if (KS_ATOMIC_LOAD(&ks_log_level) >= (type)) {                     \
            ks_log_impl((type), __FILE__, __LINE__, (fmt), ##__VA_ARGS__); \
        }                                                                  \
    } while (0)

#define _ks_log_stat_1(stat)                                                                      \
    do {                                                                                          \
        ks_status _s = (stat);                                                                    \
        int _lvl = (_s.code == KS_OK) ? KSINFO : KSERR;                                           \
        if (KS_ATOMIC_LOAD(&ks_log_level) >= _lvl) {                                              \
            if (_s.msg && _s.msg[0] != '\0') {                                                    \
                ks_log_impl(_lvl, __FILE__, __LINE__, "%s: %s", ks_code_to_str(_s.code), _s.msg); \
            } else {                                                                              \
                ks_log_impl(_lvl, __FILE__, __LINE__, "%s", ks_code_to_str(_s.code));             \
            }                                                                                     \
        }                                                                                         \
    } while (0)
#define _ks_log_stat_2(stat, msg)                                                                             \
    do {                                                                                                      \
        ks_status _s = (stat);                                                                                \
        int _lvl = (_s.code == KS_OK) ? KSINFO : KSERR;                                                       \
        if (KS_ATOMIC_LOAD(&ks_log_level) >= _lvl) {                                                          \
            if (_s.msg && _s.msg[0] != '\0') {                                                                \
                ks_log_impl(_lvl, __FILE__, __LINE__, "%s - %s: %s", (msg), ks_code_to_str(_s.code), _s.msg); \
            } else {                                                                                          \
                ks_log_impl(_lvl, __FILE__, __LINE__, "%s - %s", (msg), ks_code_to_str(_s.code));             \
            }                                                                                                 \
        }                                                                                                     \
    } while (0)

#define _KS_LOG_STAT_GET_MACRO(_1, _2, NAME, ...) NAME

#define ks_log_stat(...) KS_EXPAND(_KS_LOG_STAT_GET_MACRO(__VA_ARGS__, _ks_log_stat_2, _ks_log_stat_1)(__VA_ARGS__))

#define ks_print(fmt, ...) KS_PRINT_OUT(fmt, ##__VA_ARGS__)

#define ks_print_sys_info()                                                                                          \
    do {                                                                                                             \
        ks_print("==============================");                                                                  \
        ks_print("OS:       %s", KS_PLATFORM_NAME);                                                                  \
        ks_print("Compiler: %s (v%d.%d.%d)", KS_COMPILER_NAME, KS_COMPILER_VERSION_MAJOR, KS_COMPILER_VERSION_MINOR, \
                 KS_COMPILER_VERSION_PATCH);                                                                         \
        ks_print("Language: %s (Standard: %ld)", KS_LANGUAGE_NAME, (long)KS_LANGUAGE_STD);                           \
        ks_print("==============================");                                                                  \
    } while (0)

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

#if KS_C_VERSION >= 201112L
// ISO C11
#define KS_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
// ISO C89/C99 creates an array of negative size if condition is false,
// forcing a compile error
#define KS_STATIC_ASSERT(cond, msg) typedef char KS_CONCAT(static_assertion_failed_, __LINE__)[(cond) ? 1 : -1]
#endif

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
        KS_ASSERT(_KS_OPT_FN(is_some, T)(self), "self is None");                   \
        return self->value;                                                        \
    }

#define ks_opt(T) _KS_OPT(T)
#define ks_opt_is_some(ptr) ((ptr)->has_value)
#define ks_opt_is_none(ptr) (!(ptr)->has_value)

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

#define ks_res(ok_t, err_t) _KS_RES(ok_t, err_t)
#define ks_res_is_ok(ptr) ((ptr)->is_ok)
#define ks_res_is_err(ptr) (!(ptr)->is_ok)
#define ks_res_get_ok(ptr) ((ptr)->ok)
#define ks_res_get_err(ptr) ((ptr)->err)

#endif  // KS_CORE_H

#if defined(KS_CORE_IMPL) && !defined(KS_CORE_IMPL_DONE)
#define KS_CORE_IMPL_DONE

KS_API const char* ks_code_to_str(ks_code code) {
    switch (code) {
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

KS_API const char* ks_status_to_str(ks_status status) {
    static KS_THREAD_LOCAL char buffer[256];
    ks_status_fmt(status, buffer, sizeof(buffer));
    return buffer;
}

KS_API KS_ATOMIC_INT ks_log_level = KSINFO;

void ks_log_init(void) {
    char* env = getenv("KSLOGLVL");
    if (env) {
        KS_ATOMIC_STORE(&ks_log_level, (int)strtol(env, NULL, 10));
    }
}

void ks_log_impl(int type, const char* file, int line, const char* fmt, ...) {
    static const char* prefixes[] = {
        "{-}",  // KSERR
        "{~}",  // KSWARN
        "{+}",  // KSINFO
        "{*}"   // KSDEBUG
    };

    FILE* stream = (type == KSERR) ? stderr : stdout;
    const char* prefix = (type >= 0 && type <= 3) ? prefixes[type] : "{?}";

    fprintf(stream, "%s[%s:%d] ", prefix, file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);

    fprintf(stream, "\n");

    if (type == KSERR) {
        fflush(stream);
    }
}

#endif  // KS_CORE_IMPL