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

/* kitsune namespace */

#define KS_TYPE(name)                           NS_TYPE(ks, name)
#define KS_TEMPLATED_TYPE(name, T)              NS_TEMPLATED_TYPE(ks, name, T)

#define KS_STRUCT_DECL(name)                    NS_STRUCT_DECL(ks, name)
#define KS_STRUCT_DEF(name, ...)                NS_STRUCT_DEF(ks, name, __VA_ARGS__)
#define KS_STRUCT(name, ...)                    NS_STRUCT(ks, name, __VA_ARGS__)
#define KS_TEMPLATED_STRUCT(name, T, ...)       NS_TEMPLATED_STRUCT(ks, name, T, __VA_ARGS__)

#define KS_UNION_DECL(name)                     NS_UNION_DECL(ks, name)
#define KS_UNION_DEF(name, ...)                 NS_UNION_DEF(ks, name, __VA_ARGS__)
#define KS_UNION(name, ...)                     NS_UNION(ks, name, __VA_ARGS__)
#define KS_TEMPLATED_UNION(name, T, ...)        NS_TEMPLATED_UNION(ks, name, T, __VA_ARGS__)

#define KS_TEMPLATED_METHOD(name, action, T)    NS_TEMPLATED_METHOD(ks, name, action, T)

#define KS_ENUM(name, ...)                      NS_ENUM(ks, name, __VA_ARGS__)

#define KS_FUNC(T, name, ...)                   NS_FUNC(T,ks, name, __VA_ARGS__)

/* Alias and using */
#define KS_ALIAS(oldT, newT) typedef oldT newT
#define KS_USING(T, usingT) typedef T usingT

/* Multiple platforms */
#if defined(_WIN32) || defined(_WIN64)
#define KS_PLATFORM_WINDOWS 1
#define KS_PLATFORM_MACOS 0
#define KS_PLATFORM_LINUX 0
#elif defined(__APPLE__) && defined(__MACH__)
#define KS_PLATFORM_WINDOWS 0
#define KS_PLATFORM_MACOS 1
#define KS_PLATFORM_LINUX 0
#elif defined(__linux__)
#define KS_PLATFORM_WINDOWS 0
#define KS_PLATFORM_MACOS 0
#define KS_PLATFORM_LINUX 1
#else
#error "Unsupported operating system"
#endif

#if KS_PLATFORM_WINDOWS
#if defined(KS_EXPORT_SHARED)
#define KS_API __declspec(dllexport)
#elif defined(KS_IMPORT_SHARED)
#define KS_API __declspec(dllimport)
#else
#define KS_API
#endif
#else
#define KS_API
#endif

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

/* Assert */

#define KS_PANIC(fmt, ...)                                                                                     \
    do {                                                                                                       \
        KS_PRINT_ERR("Program panicked at %s:%d in %s():\n\t" fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        abort();                                                                                               \
    } while (0);

#ifdef NDEBUG
#define KS_ASSERT(condition, message) ((void)0)
#else
#define KS_ASSERT(condition, message)                                   \
    do {                                                                \
        if (!(condition)) {                                             \
            KS_PANIC("Assertion failed (" #condition "): %s", message); \
        }                                                               \
    } while (0)
#endif

#define KS_ASSERT_NONNULL_ARGS(x) KS_ASSERT(x, "Null arguments");

/* Utility macros */

#define KS_UNUSED __attribute__((unused))
#define KS_PACKED __attribute__((packed))
#define KS_NODISCARD __attribute__((warn_unused_result))
#define KS_DEPRECATED(msg) __attribute__((deprecated(msg)))
#define KS_ALIGNED(x) __attribute__(aligned((x)))
#define KS_LIKELY(x) __builtin_expect(!!(x), 1)
#define KS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define KS_FALLTROUGH __attribute__((falltrough))
#define KS_SIMD_HINT _Pragma("GCC ivdep")
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
#define KS_SWAP(a, b)         \
    do {                      \
        KS_TYPEOF(a) tmp = a; \
        (a) = (b);            \
        (b) = tmp;            \
    } while (0)
#define KS_ISPOW2(x) ((x) != 0 && ((((x) - 1) & (x)) == 0))
#define KS_ALIGN_DOWN(x, a) ((x) & ~((KS_TYPEOF(x))(a) - 1))
#define KS_ALIGN_UP(x, a) KS_ALIGN_DOWN((x) + (a) - 1, (a))
#define KS_NEXTPOW2(x) ((x <= 1) ? 1ULL : (KS_BIT(sizeof(x) * 8 - (size_t)KS_CLZ((x) - 1))))
#define KS_PTROFF(ptr, off) ((uint8_t*)(ptr) + (size_t)(off))
#define KS_PTRDIFF(ptr1, ptr2) ((ptrdiff_t)((intptr_t)(ptr1) - (intptr_t)(ptr2)))
#define KS_PTRDIFF_ABS(ptr1, ptr2) ((size_t)((uintptr_t)(ptr1) - (uintptr_t)(ptr2)))

/* Bit manipulation */

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

/* Strings */
#define KS_SSO_CAP (sizeof(size_t) * 3 - 2)

KS_STRUCT(string, {
    union {
        struct {
            char data[KS_SSO_CAP + 1];
            uint8_t meta;
        } s;
        struct {
            size_t cap;
            size_t len;
            char* data;
        } l;
    };
});

KS_STRUCT(str, {
    const char* ptr;
    size_t len;
});

KS_API ks_string ks_string_new(const char* s);
KS_API ks_string ks_string_new2(const char* s, size_t len);
KS_API ks_string ks_string_from(const ks_string* s);
KS_API ks_string ks_string_from_view(ks_str view);
KS_API ks_string ks_string_empty(size_t cap);
KS_API void ks_string_reserve(ks_string* s, size_t newcap);
KS_API void ks_string_upper(ks_string* s);
KS_API void ks_string_lower(ks_string* s);
KS_API int32_t ks_string_to_int64(const ks_string* s, int64_t* out, int32_t base);
KS_API void ks_string_append(ks_string* dst, const ks_string* src);
KS_API void ks_string_append_view(ks_string* dst, ks_str src);
KS_API void ks_string_append_raw(ks_string* dst, const char* src);
KS_API void ks_string_append_raw2(ks_string* dst, const char* src, size_t srclen);
KS_API ks_string ks_string_concat(const ks_string* s1, const ks_string* s2);
KS_API ks_string ks_string_concat_raw(const char* s1, const char* s2);
KS_API ks_string ks_string_concat_raw2(const char* s1, size_t len1, const char* s2, size_t len2);
KS_API void ks_string_push(ks_string* dst, char c);
KS_API void ks_string_cut(ks_string* s, size_t n);
KS_API char ks_string_pop(ks_string* s);
KS_API void ks_string_trim_left(ks_string* s);
KS_API void ks_string_trim_right(ks_string* s);
KS_API void ks_string_trim(ks_string* s);
KS_API ks_str* ks_string_split(const ks_string* s, const char* separator);
KS_API ks_string ks_string_join(const ks_str* parts, const char* separator);
KS_API char* ks_string_find(const ks_string* s, const ks_string* needle);
KS_API char* ks_string_find_raw(const ks_string* s, const char* needle);
KS_API char* ks_string_find_raw2(const ks_string* s, const char* needle, size_t len);
KS_API void ks_string_replace(ks_string* s, const ks_string* a, const ks_string* b);
KS_API void ks_string_replace_raw(ks_string* s, const char* a, const char* b);
KS_API void ks_string_replace_raw2(ks_string* s, const char* a, size_t lena, const char* b, size_t lenb);
KS_API ks_string ks_string_format(const char* fmt, ...);
KS_API bool ks_string_is_ascii(const ks_string* s);
KS_API bool ks_string_is_upper(const ks_string* s);
KS_API bool ks_string_is_lower(const ks_string* s);
KS_API bool ks_string_is_alpha(const ks_string* s);
KS_API bool ks_string_is_num(const ks_string* s);
KS_API bool ks_string_is_alnum(const ks_string* s);
KS_API int32_t ks_string_cmp(const ks_string* s1, const ks_string* s2);
KS_API void ks_string_shrink(ks_string* s);
KS_API void ks_string_clear(ks_string* s);
KS_API void ks_string_free(ks_string* s);

static inline bool ks_string_is_short(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    return s->s.meta & 1;
}

static inline size_t ks_string_len(const ks_string* s) {
    if (!s) {
        return 0;
    }

    return ks_string_is_short(s) ? s->s.meta >> 1 : s->l.len;
}

static inline char* ks_string_as_raw(ks_string* s) {
    if (!s) {
        return NULL;
    }

    return ks_string_is_short(s) ? s->s.data : s->l.data;
}

static inline bool ks_string_is_empty(const ks_string* s) {
    return ks_string_len(s) == 0;
}

static inline size_t ks_string_cap(const ks_string* s) {
    if (!s) {
        return 0;
    }

    return ks_string_is_short(s) ? KS_SSO_CAP : s->l.cap;
}

KS_API ks_str ks_str_new(const char* s, size_t start, size_t end);
KS_API ks_str ks_str_from(ks_str s, size_t start, size_t end);
KS_API const char* ks_str_as_raw(ks_str s);
KS_API void ks_str_trim(ks_str* s);
KS_API int32_t ks_str_cmp(ks_str s1, ks_str s2);
KS_API bool ks_str_starts_with(ks_str s, const char* prefix);
KS_API bool ks_str_ends_with(ks_str s, const char* suffix);

static inline ks_str ks_string_view(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    return ks_str_new(ks_string_as_raw((ks_string*)s), 0, ks_string_len(s));
}

static inline size_t ks_str_len(ks_str s) {
    return s.len;
}

static inline bool ks_str_is_empty(ks_str s) {
    return ks_str_len(s) == 0;
}

#endif  // KS_CORE_H

#if defined(KS_CORE_IMPL) && !defined(KS_CORE_IMPL_DONE)
#define KS_CORE_IMPL_DONE

KS_API int ks_log_level = KSINFO;

/* Strings */

#define ks_string_foreach(it, s) \
    for (char *it = ks_string_as_raw((ks_string*)s), *_end = it + ks_string_len(s); it < _end; ++it)
#define ks_str_foreach(it, s) \
    for (const char *it = (s).ptr, *_end = (const char*)KS_PTROFF((s).ptr, (s).len); it < _end; ++it)

static inline void _ks_string_resize(ks_string* s, size_t newcap) {
    size_t len = ks_string_len(s);
    size_t oldcap = ks_string_cap(s);

    if (ks_string_is_short(s)) {
        if (newcap <= KS_SSO_CAP) {
            return;
        }

        // upgrade to a long string
        char* newdata = malloc(newcap + 1);
        KS_ASSERT(newdata, "OOM");
        memcpy(newdata, s->s.data, len);
        newdata[len] = '\0';
        memset(s, 0, sizeof(ks_string));

        s->l.cap = newcap;
        s->l.len = len;
        s->l.data = newdata;
    } else if (newcap <= KS_SSO_CAP) {
        // downgrade to a short string
        char* oldata = s->l.data;
        size_t newlen = KS_MIN(len, newcap);
        memcpy(s->s.data, oldata, newlen);
        s->s.data[newlen] = '\0';
        s->s.meta = newlen << 1 | 1;
        free(oldata);
    } else {
        if (newcap == oldcap) {
            return;
        }

        // create a shorter/longer long string
        char* newdata = realloc(s->l.data, newcap + 1);
        KS_ASSERT(newdata, "OOM");
        s->l.data = newdata;
        s->l.cap = newcap;
        if (newcap < len) {
            s->l.len = newcap;
            s->l.data[newcap] = '\0';
        }
    }
}

static inline void _ks_string_set_len(ks_string* str, size_t newlen) {
    if (ks_string_is_short(str)) {
        str->s.meta = newlen << 1 | 1;
    } else {
        str->l.len = newlen;
    }
}

static inline void _ks_skip_white_spaces(const char** str_ptr) {
    const char* str = *str_ptr;
    while (isspace((unsigned char)*str)) {
        ++str;
    }
    *str_ptr = str;
}

KS_API ks_string ks_string_new(const char* s) {
    return ks_string_new2(s, strlen(s));
}

KS_API ks_string ks_string_new2(const char* s, size_t len) {
    KS_ASSERT(s, "s is NULL");

    ks_string string = {0};
    if (len <= KS_SSO_CAP) {
        memcpy(string.s.data, s, len);
        string.s.data[len] = '\0';
        string.s.meta = len << 1 | 1;
    } else {
        size_t cap = KS_NEXTPOW2(len + 1);
        char* data = malloc(cap);
        KS_ASSERT(data, "OOM");
        string.l.cap = cap - 1;
        string.l.len = len;
        string.l.data = data;
        memcpy(string.l.data, s, len);
        string.l.data[len] = '\0';
    }

    return string;
}

KS_API ks_string ks_string_from(const ks_string* s) {
    if (!s) {
        return ks_string_empty(0);
    }

    char* str = ks_string_as_raw((ks_string*)s);
    size_t len = ks_string_len(s);
    return ks_string_new2(str, len);
}

KS_API ks_string ks_string_from_view(ks_str view) {
    return ks_string_new2(view.ptr, view.len);
}

KS_API ks_string ks_string_empty(size_t cap) {
    ks_string s = {0};
    s.s.data[0] = '\0';
    s.s.meta = 0 << 1 | 1;

    if (cap > KS_SSO_CAP) {
        ks_string_reserve(&s, cap);
    }
    return s;
}

KS_API void ks_string_reserve(ks_string* s, size_t newcap) {
    KS_ASSERT(s, "s is NULL");

    size_t oldcap = ks_string_cap(s);
    if (newcap <= oldcap) {
        return;
    }

    _ks_string_resize(s, newcap);
}

KS_API void ks_string_upper(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    ks_string_foreach(c, s) {
        *c = toupper(*c);
    }
}

KS_API void ks_string_lower(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    ks_string_foreach(c, s) {
        *c = tolower(*c);
    }
}

KS_API int32_t ks_string_to_int64(const ks_string* s, int64_t* out, int32_t base) {
    KS_ASSERT(s && out, "NULL parameters");

    const char* str = ks_string_as_raw((ks_string*)s);
    _ks_skip_white_spaces(&str);

    if (*str == '\0') {
        return -1;
    }

    errno = 0;
    char* endptr = NULL;
    long long val = strtoll(str, &endptr, base);

    if (endptr == str || errno == ERANGE) {
        return -1;
    }

    const char* cend = endptr;
    _ks_skip_white_spaces(&cend);

    if (*cend != '\0') {
        return -1;
    }

    *out = (int64_t)val;

    return 0;
}

KS_API void ks_string_append(ks_string* dst, const ks_string* src) {
    ks_string_append_raw2(dst, ks_string_as_raw((ks_string*)src), ks_string_len(src));
}

KS_API void ks_string_append_view(ks_string* dst, ks_str src) {
    KS_ASSERT(dst, "dst is NULL");
    if (src.len == 0 || !src.ptr) {
        return;
    }

    ks_string_append_raw2(dst, src.ptr, src.len);
}

KS_API void ks_string_append_raw(ks_string* dst, const char* src) {
    ks_string_append_raw2(dst, src, strlen(src));
}

KS_API void ks_string_append_raw2(ks_string* dst, const char* src, size_t srclen) {
    KS_ASSERT(dst && src, "dst or src is NULL");

    if (srclen == 0) {
        return;
    }

    size_t dstlen = ks_string_len(dst);
    size_t newlen = srclen + dstlen;

    char* dstraw = ks_string_as_raw(dst);
    bool is_overlapping = src >= dstraw && src < dstraw + dstlen;
    size_t srcoff = is_overlapping ? (size_t)(src - dstraw) : 0;

    if (ks_string_cap(dst) < newlen) {
        size_t newcap = newlen <= KS_SSO_CAP ? KS_SSO_CAP : KS_NEXTPOW2(newlen + 1);
        _ks_string_resize(dst, newcap);

        dstraw = ks_string_as_raw(dst);
        if (is_overlapping) {
            src = dstraw + srcoff;
        }
    }

    memcpy(KS_PTROFF(dstraw, dstlen), src, srclen);
    dstraw[newlen] = '\0';

    _ks_string_set_len(dst, newlen);
}

KS_API ks_string ks_string_concat(const ks_string* s1, const ks_string* s2) {
    return ks_string_concat_raw2(ks_string_as_raw((ks_string*)s1), ks_string_len(s1), ks_string_as_raw((ks_string*)s2),
                                 ks_string_len(s2));
}

KS_API ks_string ks_string_concat_raw(const char* s1, const char* s2) {
    return ks_string_concat_raw2(s1, strlen(s1), s2, strlen(s2));
}

KS_API ks_string ks_string_concat_raw2(const char* s1, size_t len1, const char* s2, size_t len2) {
    ks_string str = ks_string_new2(s1, len1);
    ks_string_append_raw2(&str, s2, len2);
    return str;
}

KS_API void ks_string_push(ks_string* dst, char c) {
    KS_ASSERT(dst, "dst is NULL");

    ks_string_append_raw2(dst, &c, 1);
}

KS_API void ks_string_cut(ks_string* s, size_t n) {
    KS_ASSERT(s, "s is NULL");
    if (n == 0) {
        return;
    }

    size_t len = ks_string_len(s);
    KS_ASSERT(n <= len, "n is greater than the current length");

    size_t newlen = len - n;
    char* raw = ks_string_as_raw(s);
    raw[newlen] = '\0';

    _ks_string_set_len(s, newlen);

    size_t cap = ks_string_cap(s);
    if (!ks_string_is_short(s) && newlen < cap >> 2) {
        _ks_string_resize(s, cap >> 1);
    }
}

KS_API char ks_string_pop(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    size_t len = ks_string_len(s);
    if (len == 0) {
        return '\0';
    }

    char* raw = ks_string_as_raw(s);
    char last = raw[len - 1];

    ks_string_cut(s, 1);

    return last;
}

KS_API void ks_string_trim_left(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    size_t len = ks_string_len(s);
    if (len == 0) {
        return;
    }

    char* raw = ks_string_as_raw(s);
    size_t start = 0;

    while (start < len && isspace(raw[start])) {
        ++start;
    }

    if (start > 0) {
        size_t newlen = len - start;
        memmove(raw, raw + start, newlen);
        raw[newlen] = '\0';
        _ks_string_set_len(s, newlen);
    }
}

KS_API void ks_string_trim_right(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    size_t len = ks_string_len(s);
    if (len == 0) {
        return;
    }

    char* raw = ks_string_as_raw(s);
    size_t end = len;

    while (end > 0 && isspace((unsigned char)raw[end - 1])) {
        --end;
    }

    if (end < len) {
        raw[end] = '\0';
        _ks_string_set_len(s, end);
    }
}

KS_API void ks_string_trim(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    size_t len = ks_string_len(s);
    if (len == 0) {
        return;
    }

    char* raw = ks_string_as_raw(s);
    size_t start = 0;
    while (start < len && isspace((unsigned char)raw[start])) {
        ++start;
    }

    if (start == len) {
        ks_string_clear(s);
        return;
    }

    size_t end = len;
    while (end > start && isspace(raw[end - 1])) {
        --end;
    }

    size_t newlen = end - start;
    if (start > 0) {
        memmove(raw, raw + start, newlen);
    }

    raw[newlen] = '\0';
    _ks_string_set_len(s, newlen);
}

KS_API ks_str* ks_string_split(const ks_string* s, const char* separator) {
    KS_ASSERT(s && separator, "s or separator is NULL");

    size_t seplen = strlen(separator);
    if (seplen == 0) {
        return NULL;
    }

    char* raw = ks_string_as_raw((ks_string*)s);
    size_t slen = ks_string_len(s);

    size_t count = 1;
    char* temp = raw;
    while ((temp = strstr(temp, separator))) {
        ++count;
        temp += seplen;
    }

    ks_str* result = malloc((count + 1) * sizeof(ks_str));
    KS_ASSERT(result, "OOM");

    size_t i = 0;
    char* startptr = raw;
    char* matchptr;

    while ((matchptr = strstr(startptr, separator))) {
        size_t startidx = KS_PTRDIFF_ABS(startptr, raw);
        size_t endidx = KS_PTRDIFF_ABS(matchptr, raw);

        result[i++] = ks_str_new(raw, startidx, endidx);
        startptr = matchptr + seplen;
    }

    result[i++] = ks_str_new(raw, (size_t)(startptr - raw), slen);

    result[i].ptr = NULL;
    result[i].len = 0;

    return result;
}

KS_API ks_string ks_string_join(const ks_str* parts, const char* separator) {
    if (!parts) {
        return ks_string_empty(0);
    }
    size_t seplen = separator ? strlen(separator) : 0;

    size_t totlen = 0;
    size_t count = 0;

    while (parts[count].ptr) {
        totlen += parts[count].len;
        ++count;
    }

    if (count == 0) {
        return ks_string_empty(0);
    }

    if (count > 1) {
        totlen += (count - 1) * seplen;
    }

    ks_string result = ks_string_empty(totlen);
    for (size_t i = 0; i < count; ++i) {
        ks_string_append_raw2(&result, parts[i].ptr, parts[i].len);
        if (i < count - 1 && seplen > 0) {
            ks_string_append_raw2(&result, separator, seplen);
        }
    }

    return result;
}

KS_API char* ks_string_find(const ks_string* s, const ks_string* needle) {
    return ks_string_find_raw2(s, ks_string_as_raw((ks_string*)needle), ks_string_len(needle));
}

KS_API char* ks_string_find_raw(const ks_string* s, const char* needle) {
    return ks_string_find_raw2(s, needle, strlen(needle));
}

KS_API char* ks_string_find_raw2(const ks_string* s, const char* needle, size_t len) {
    char* s1 = ks_string_as_raw((ks_string*)s);
    const char* p = s1;

    if (!len) {
        return s1;
    }

    for (; (p = strchr(p, *needle)); ++p) {
        if (strncmp(p, needle, len) == 0) {
            return (char*)p;
        }
    }

    return NULL;
}

KS_API void ks_string_replace(ks_string* s, const ks_string* a, const ks_string* b) {
    ks_string_replace_raw2(s, ks_string_as_raw((ks_string*)a), ks_string_len(a), ks_string_as_raw((ks_string*)b),
                           ks_string_len(b));
}

KS_API void ks_string_replace_raw(ks_string* s, const char* a, const char* b) {
    ks_string_replace_raw2(s, a, strlen(a), b, strlen(b));
}

KS_API void ks_string_replace_raw2(ks_string* s, const char* a, size_t lena, const char* b, size_t lenb) {
    KS_ASSERT(s && a && b, "Some parameters are NULL");

    char* pos = ks_string_find_raw2(s, a, lena);
    if (!pos) {
        return;
    }

    char* haystack = ks_string_as_raw(s);
    size_t prelen = KS_PTRDIFF_ABS(pos, haystack);
    size_t suflen = ks_string_len(s) - prelen - lena;
    size_t totlen = prelen + lenb + suflen;

    ks_string tmp = ks_string_empty(totlen);

    if (prelen > 0) {
        ks_string_append_raw2(&tmp, haystack, prelen);
    }

    if (lenb > 0) {
        ks_string_append_raw2(&tmp, b, lenb);
    }

    if (suflen > 0) {
        ks_string_append_raw2(&tmp, pos + lena, suflen);
    }

    ks_string_free(s);

    *s = tmp;
}

KS_API ks_string ks_string_format(const char* fmt, ...) {
    KS_ASSERT(fmt, "fmt is NULL");

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        va_end(args);
        return ks_string_empty(0);
    }

    ks_string result = ks_string_empty((size_t)len);

    char* raw = ks_string_as_raw(&result);
    vsnprintf(raw, (size_t)len + 1, fmt, args);
    va_end(args);

    _ks_string_set_len(&result, (size_t)len);

    return result;
}

KS_API bool ks_string_is_ascii(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!__isascii(*c)) {
            return false;
        }
    }

    return true;
}

KS_API bool ks_string_is_upper(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isupper(*c)) {
            return false;
        }
    }

    return true;
}

KS_API bool ks_string_is_lower(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!islower(*c)) {
            return false;
        }
    }

    return true;
}

KS_API bool ks_string_is_alpha(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isalpha(*c)) {
            return false;
        }
    }

    return true;
}

KS_API bool ks_string_is_num(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (*c < '0' || *c > '9') {
            return false;
        }
    }

    return true;
}

KS_API bool ks_string_is_alnum(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isalnum(*c)) {
            return false;
        }
    }

    return true;
}

KS_API int32_t ks_string_cmp(const ks_string* s1, const ks_string* s2) {
    KS_ASSERT(s1 && s2, "s1 or s2 is NULL");
    return strcmp(ks_string_as_raw((ks_string*)s1), ks_string_as_raw((ks_string*)s2));
}

KS_API void ks_string_shrink(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    size_t len = ks_string_len(s);
    _ks_string_resize(s, len);
}

KS_API void ks_string_clear(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    char* raw = ks_string_as_raw(s);
    raw[0] = '\0';
    _ks_string_set_len(s, 0);
}

KS_API void ks_string_free(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    if (!ks_string_is_short(s)) {
        free(s->l.data);
    }

    memset(s, 0, sizeof(ks_string));
    s->s.meta = 1;
}

KS_API ks_str ks_str_new(const char* s, size_t start, size_t end) {
    KS_ASSERT(s, "s is NULL");
    KS_ASSERT(end >= start, "end greater or equal to start");
    return (ks_str){.ptr = (const char*)KS_PTROFF(s, start), .len = end - start};
}

KS_API ks_str ks_str_from(ks_str s, size_t start, size_t end) {
    KS_ASSERT(end >= start && end <= s.len, "Invalid bounds");
    return (ks_str){.ptr = (const char*)KS_PTROFF(s.ptr, start), .len = end - start};
}

KS_API const char* ks_str_as_raw(ks_str s) {
    return s.ptr;
}

KS_API void ks_str_trim(ks_str* s) {
    KS_ASSERT(s, "s is NULL");

    while (s->len > 0 && isspace((unsigned char)s->ptr[0])) {
        ++s->ptr;
        --s->len;
    }

    while (s->len > 0 && isspace((unsigned char)s->ptr[s->len - 1])) {
        --s->len;
    }
}

KS_API int32_t ks_str_cmp(ks_str s1, ks_str s2) {
    size_t minlen = KS_MIN(s1.len, s2.len);
    int32_t cmp = strncmp(s1.ptr, s2.ptr, minlen);

    if (cmp == 0) {
        if (s1.len < s2.len) {
            return -1;
        }

        if (s1.len > s2.len) {
            return 1;
        }

        return 0;
    }

    return cmp;
}

KS_API bool ks_str_starts_with(ks_str s, const char* prefix) {
    KS_ASSERT(prefix, "prefix is NULL");
    size_t prelen = strlen(prefix);

    if (prelen > s.len) {
        return false;
    }

    return strncmp(s.ptr, prefix, prelen) == 0;
}

KS_API bool ks_str_ends_with(ks_str s, const char* suffix) {
    KS_ASSERT(suffix, "suffix is NULL");
    size_t suflen = strlen(suffix);

    if (suflen > s.len) {
        return false;
    }

    return strncmp(s.ptr + (s.len - suflen), suffix, suflen) == 0;
}

#endif  // KS_CORE_IMPL