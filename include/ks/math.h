#ifndef KS_MATH_H
#define KS_MATH_H

#include <ks/core.h>
#include <math.h>
#include <complex.h>
#include <omp.h>

#define KS_E 2.7182818284590452354          // e
#define KS_LOG2E 1.4426950408889634074      // log2 e
#define KS_LOG10E 0.43429448190325182765    // log10 e
#define KS_LN2 0.69314718055994530942       // loge 2
#define KS_LN10 2.30258509299404568402      // loge 10
#define KS_PI 3.14159265358979323846        // pi
#define KS_2PI 6.28318530718                // 2pi
#define KS_PI_2 1.57079632679489661923      // pi/2
#define KS_PI_4 0.78539816339744830962      // pi/4
#define KS_1_PI 0.31830988618379067154      // 1/pi
#define KS_2_PI 0.63661977236758134308      // 2/pi
#define KS_SQRTPI 1.77245385091             // sqrt(pi)
#define KS_1_SQRTPI 0.564189583548          // 1/sqrt(pi)
#define KS_1_SQRT2PI 0.398942280401         // 1/sqrt(2pi)
#define KS_2_SQRTPI 1.12837916709551257390  // 2/sqrt(pi)
#define KS_2_SQRT2PI 0.797884560803         // 2/sqrt(2pi)
#define KS_SQRT2 1.41421356237309504880     // sqrt(2)
#define KS_1_SQRT2 0.70710678118654752440   // 1/sqrt(2)
#define KS_SQRT3 1.73205080757              // sqrt(3)
#define KS_1_SQRT3 0.57735026919            // 1/sqrt(3)

#define KS_DEG2RAD (KS_PI / 180.0)
#define KS_RAD2DEG (180.0 / KS_PI)

/* Utilities */

static inline size_t log2ld(size_t n) {
    size_t i = 0;
    for (i = 0; n; ++i) {
        n >>= 1;
    }
    return i - 1;
}

/* Linear algebra */

KS_UNION(vec2, {
    struct {
        float x;
        float y;
    };
    float data[2];
});

#define KS_VEC2(x, y) \
    (ks_vec2) {       \
        {             \
            x, y      \
        }             \
    }

KS_UNION(vec3, {
    struct {
        float x;
        float y;
        float z;
    };
    float data[3];
});

#define KS_VEC3(x, y, z) \
    (ks_vec3) {          \
        {                \
            x, y, z      \
        }                \
    }

KS_UNION(vec4, {
    struct {
        float x;
        float y;
        float z;
        float w;
    };
    float data[4];
});

#define KS_VEC4(x, y, z, w) \
    (ks_vec4) {             \
        {                   \
            x, y, z, w      \
        }                   \
    }

KS_UNION(mat2, {
    float m[2][2];
    float data[4];
    ks_vec2 cols[2];
    struct {
        float e00, e10;
        float e01, e11;
    };
});

KS_UNION(mat3, {
    float m[3][3];
    float data[9];
    ks_vec3 cols[3];
    struct {
        float e00, e10, e20;
        float e01, e11, e21;
        float e02, e12, e22;
    };
});

KS_UNION(mat4, {
    float m[4][4];
    float data[16];
    ks_vec4 cols[4];
    struct {
        float e00, e10, e20, e30;
        float e01, e11, e21, e31;
        float e02, e12, e22, e32;
        float e03, e13, e23, e33;
    };
});

static inline ks_string _ks_vec_tostr_internal(const char* type_name, const float* data, int32_t n) {
    ks_string s = ks_string_format("%s(", type_name);

    for (int32_t i = 0; i < n; ++i) {
        ks_string num = ks_string_format("%f", data[i]);
        ks_string_append(&s, &num);
        ks_string_free(&num);

        // Add a comma unless it's the last element
        if (i < n - 1) {
            ks_string_append_raw(&s, ", ");
        }
    }

    ks_string_append_raw(&s, ")");
    return s;
}

static inline ks_string _ks_mat_tostr_internal(const char* type_name, const float* data, int32_t n) {
    ks_string s = ks_string_format("%s(\n", type_name);

    for (int32_t r = 0; r < n; ++r) {
        ks_string_append_raw2(&s, "    ", 4);
        for (int32_t c = 0; c < n; ++c) {
            ks_string num = ks_string_format("%f%s", data[c * n + r], (c == n - 1 ? "" : ", "));
            ks_string_append(&s, &num);
            ks_string_free(&num);
        }

        ks_string_append_raw(&s, (r == n - 1 ? "\n" : ",\n"));
    }

    ks_string_append_raw2(&s, ")", 1);
    return s;
}

#define KS_VEC_FUNCTIONS(type, n, params, args)                                                       \
    static inline void KS_CONCAT2(type, _zeroinit)(type * v) {                                        \
        KS_ASSERT(v, "Null arguments");                                                               \
        memset(v->data, 0, sizeof(type));                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline type KS_CONCAT2(type, _zeronew)(void) {                                             \
        type v;                                                                                       \
        KS_CONCAT2(type, _zeroinit)(&v);                                                              \
        return v;                                                                                     \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _init)(type * v, KS_UNPACK params) {                          \
        KS_ASSERT(v, "Null arguments");                                                               \
        *v = (type){{KS_UNPACK args}};                                                                \
    }                                                                                                 \
                                                                                                      \
    static inline type KS_CONCAT2(type, _new)(KS_UNPACK params) {                                     \
        return (type){{KS_UNPACK args}};                                                              \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _fill)(type * v, float num) {                                 \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v->data[i] = num;                                                                         \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _add)(type * out, const type* v1, const type* v2) {           \
        KS_ASSERT(out && v1 && v2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = v1->data[i] + v2->data[i];                                                 \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _addi)(type * v1, const type* v2) {                           \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v1->data[i] += v2->data[i];                                                               \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _neg)(type * out, const type* v) {                            \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = -v->data[i];                                                               \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _negi)(type * v) {                                            \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v->data[i] = -v->data[i];                                                                 \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sub)(type * out, const type* v1, const type* v2) {           \
        KS_ASSERT(out && v1 && v2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = v1->data[i] - v2->data[i];                                                 \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _subi)(type * v1, const type* v2) {                           \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v1->data[i] -= v2->data[i];                                                               \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _smul)(type * out, const type* v, float num) {                \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = v->data[i] * num;                                                          \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _smuli)(type * v, float num) {                                \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v->data[i] *= num;                                                                        \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sdiv)(type * out, const type* v, float num) {                \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_ASSERT(num != 0.0f, "Zero division");                                                      \
        float inv = 1.0f / num;                                                                       \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = v->data[i] * inv;                                                          \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sdivi)(type * v, float num) {                                \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_ASSERT(num != 0.0f, "Zero division");                                                      \
        float inv = 1.0f / num;                                                                       \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v->data[i] *= inv;                                                                        \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _dot)(const type* v1, const type* v2) {                      \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        float res = 0;                                                                                \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            res += v1->data[i] * v2->data[i];                                                         \
        }                                                                                             \
        return res;                                                                                   \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _length_sq)(const type* v) {                                 \
        return KS_CONCAT2(type, _dot)(v, v);                                                          \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _length)(const type* v) {                                    \
        return sqrtf(KS_CONCAT2(type, _dot)(v, v));                                                   \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _norm)(type * out, const type* v) {                           \
        float len = KS_CONCAT2(type, _length)(v);                                                     \
        KS_ASSERT(len != 0.0f, "Normalizing zero vector");                                            \
        KS_CONCAT2(type, _sdiv)(out, v, len);                                                         \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _normi)(type * v) {                                           \
        float len = KS_CONCAT2(type, _length)(v);                                                     \
        KS_ASSERT(len != 0.0f, "Normalizing zero vector");                                            \
        KS_CONCAT2(type, _sdivi)(v, len);                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _dist_sq)(const type* v1, const type* v2) {                  \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        float res = 0;                                                                                \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            float diff = v1->data[i] - v2->data[i];                                                   \
            res += diff * diff;                                                                       \
        }                                                                                             \
        return res;                                                                                   \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _dist)(const type* v1, const type* v2) {                     \
        return sqrtf(KS_CONCAT2(type, _dist_sq)(v1, v2));                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _lerp)(type * out, const type* v1, const type* v2, float t) { \
        KS_ASSERT(out && v1 && v2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = (1 - t) * v1->data[i] + t * v2->data[i];                                   \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _lerpi)(type * v1, const type* v2, float t) {                 \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            v1->data[i] = (1 - t) * v1->data[i] + t * v2->data[i];                                    \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _get)(const type* v, int32_t i) {                            \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_ASSERT(i < n, "Index out of bounds");                                                      \
        return v->data[i];                                                                            \
    }                                                                                                 \
                                                                                                      \
    static inline float* KS_CONCAT2(type, _ptr)(type * v) {                                           \
        KS_ASSERT(v, "Null arguments");                                                               \
        return v->data;                                                                               \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _set)(type * out, const type* v, int32_t i, float num) {      \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_ASSERT(i < n, "Index out of bounds");                                                      \
        KS_SIMD_HINT for (int32_t j = 0; j < n; ++j) {                                                \
            if (i == j) {                                                                             \
                out->data[i] = num;                                                                   \
            } else {                                                                                  \
                out->data[j] = v->data[j];                                                            \
            }                                                                                         \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _seti)(type * v, int32_t i, float num) {                      \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_ASSERT(i < n, "Index out of bounds");                                                      \
        v->data[i] = num;                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline ks_string KS_CONCAT2(type, _tostr)(const type* v) {                                 \
        KS_ASSERT(v, "Null arguments");                                                               \
        return _ks_vec_tostr_internal(#type, v->data, n);                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _print)(const type* v, FILE* stream) {                        \
        KS_ASSERT(v && stream, "Null arguments");                                                     \
        ks_string s = KS_CONCAT2(type, _tostr)(v);                                                    \
        fprintf(stream, "%s", ks_string_as_raw(&s));                                                  \
        ks_string_free(&s);                                                                           \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _println)(const type* v, FILE* stream) {                      \
        KS_CONCAT2(type, _print)(v, stream);                                                          \
        fprintf(stream, "\n");                                                                        \
    }

KS_VEC_FUNCTIONS(ks_vec2, 2, (float x, float y), (x, y));
KS_VEC_FUNCTIONS(ks_vec3, 3, (float x, float y, float z), (x, y, z));
KS_VEC_FUNCTIONS(ks_vec4, 4, (float x, float y, float z, float w), (x, y, z, w));

/* vec3 */

static inline void ks_vec3_cross(ks_vec3* out, const ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(out && v1 && v2, "Null arguments");
    out->x = v1->y * v2->z - v1->z * v2->y;
    out->y = v1->z * v2->x - v1->x * v2->z;
    out->z = v1->x * v2->y - v1->y * v2->x;
}

static inline void ks_vec3_crossi(ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    v1->x = v1->y * v2->z - v1->z * v2->y;
    v1->y = v1->z * v2->x - v1->x * v2->z;
    v1->z = v1->x * v2->y - v1->y * v2->x;
}

#define KS_MAT_FUNCTIONS(type, vtype, n)                                                       \
    static inline void KS_CONCAT2(type, _zeroinit)(type * m) {                                 \
        KS_ASSERT(m, "Null arguments");                                                        \
        memset(m->data, 0, sizeof(type));                                                      \
    }                                                                                          \
                                                                                               \
    static inline type KS_CONCAT2(type, _zeronew)(void) {                                      \
        type m;                                                                                \
        KS_CONCAT2(type, _zeroinit)(&m);                                                       \
        return m;                                                                              \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _idinit)(type * m) {                                   \
        KS_CONCAT2(type, _zeroinit)(m);                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                         \
            m->data[i * n + i] = 1.0f;                                                         \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline type KS_CONCAT2(type, _idnew)(void) {                                        \
        type m;                                                                                \
        KS_CONCAT2(type, _idinit)(&m);                                                         \
        return m;                                                                              \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _fill)(type * m, float num) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m->data[i] = num;                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _add)(type * out, const type* m1, const type* m2) {    \
        KS_ASSERT(out && m1 && m2, "Null arguments");                                          \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            out->data[i] = m1->data[i] + m2->data[i];                                          \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _addi)(type * m1, const type* m2) {                    \
        KS_ASSERT(m1 && m2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m1->data[i] += m2->data[i];                                                        \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _neg)(type * out, const type* m) {                     \
        KS_ASSERT(out && m, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            out->data[i] = -m->data[i];                                                        \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _negi)(type * m) {                                     \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m->data[i] = -m->data[i];                                                          \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _sub)(type * out, const type* m1, const type* m2) {    \
        KS_ASSERT(out && m1 && m2, "Null arguments");                                          \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            out->data[i] = m1->data[i] - m2->data[i];                                          \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _subi)(type * m1, const type* m2) {                    \
        KS_ASSERT(m1 && m2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m1->data[i] -= m2->data[i];                                                        \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _smul)(type * out, const type* m, float num) {         \
        KS_ASSERT(out && m, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            out->data[i] = m->data[i] * num;                                                   \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _smuli)(type * m, float num) {                         \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m->data[i] *= num;                                                                 \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _sdiv)(type * out, const type* m, float num) {         \
        KS_ASSERT(out && m, "Null arguments");                                                 \
        KS_ASSERT(num != 0.0f, "Zero division");                                               \
        float inv = 1.0f / num;                                                                \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            out->data[i] = m->data[i] * inv;                                                   \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _sdivi)(type * m, float num) {                         \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_ASSERT(num != 0.0f, "Zero division");                                               \
        float inv = 1.0f / num;                                                                \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m->data[i] *= inv;                                                                 \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _trans)(type * out, const type* m) {                   \
        KS_ASSERT(out && m, "Null arguments");                                                 \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < n; ++r) {                                                  \
                out->data[c * n + r] = m->data[r * n + c];                                     \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _transi)(type * m) {                                   \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < n; ++r) {                                                  \
                m->data[c * n + r] = m->data[r * n + c];                                       \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _mul)(type * out, const type* m1, const type* m2) {    \
        KS_ASSERT(out && m1 && m2, "Null arguments");                                          \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < n; ++r) {                                                  \
                float sum = 0.0f;                                                              \
                for (int32_t k = 0; k < n; ++k) {                                              \
                    sum += m1->data[k * n + r] * m2->data[c * n + k];                          \
                }                                                                              \
                                                                                               \
                out->data[c * n + r] = sum;                                                    \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _muli)(type * m1, const type* m2) {                    \
        type tmp;                                                                              \
        KS_CONCAT2(type, _mul)(&tmp, m1, m2);                                                  \
        *m1 = tmp;                                                                             \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _mulv)(vtype * out, const type* m, const vtype* v) {   \
        KS_ASSERT(out && m && v, "Null arguments");                                            \
        for (int32_t r = 0; r < n; ++r) {                                                      \
            float sum = 0.0f;                                                                  \
            for (int32_t c = 0; c < n; ++c) {                                                  \
                sum += m->data[c * n + r] * v->data[c];                                        \
            }                                                                                  \
            out->data[r] = sum;                                                                \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _mulvi)(const type* m, vtype* v) {                     \
        vtype tmp;                                                                             \
        KS_CONCAT2(type, _mulv)(&tmp, m, v);                                                   \
        *v = tmp;                                                                              \
    }                                                                                          \
                                                                                               \
    static inline type KS_CONCAT2(type, _model)(const type* t, const type* r, const type* s) { \
        KS_ASSERT_NONNULL_ARGS(t && r && s);                                                   \
        type m;                                                                                \
        KS_CONCAT2(type, _mul)(&m, t, r);                                                      \
        KS_CONCAT2(type, _muli)(&m, s);                                                        \
        return m;                                                                              \
    }                                                                                          \
                                                                                               \
    static inline float KS_CONCAT2(type, _get)(const type* m, int32_t c, int32_t r) {          \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_ASSERT(c < n && r < n, "Index out of bounds");                                      \
        return m->data[c * n + r];                                                             \
    }                                                                                          \
                                                                                               \
    static inline vtype KS_CONCAT2(type, _getc)(const type* m, int32_t c) {                    \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_ASSERT(c < n, "Index out of bounds");                                               \
        vtype res;                                                                             \
        for (int32_t i = 0; i < n; ++i) {                                                      \
            res.data[i] = m->data[c * n + i];                                                  \
        }                                                                                      \
        return res;                                                                            \
    }                                                                                          \
                                                                                               \
    static inline vtype KS_CONCAT2(type, _getr)(const type* m, int32_t r) {                    \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_ASSERT(r < n, "Index out of bounds");                                               \
        vtype res;                                                                             \
        for (int32_t i = 0; i < n; ++i) {                                                      \
            res.data[i] = m->data[i * n + r];                                                  \
        }                                                                                      \
        return res;                                                                            \
    }                                                                                          \
                                                                                               \
    static inline float* KS_CONCAT2(type, _ptr)(type * m) {                                    \
        KS_ASSERT(m, "Null arguments");                                                        \
        return m->data;                                                                        \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _set)(type * m, int32_t c, int32_t r, float num) {     \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_ASSERT(c < n && r < n, "Index out of bounds");                                      \
        m->data[c * n + r] = num;                                                              \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _setc)(type * m, int32_t c, const vtype* v) {          \
        KS_ASSERT(m && v, "Null arguments");                                                   \
        KS_ASSERT(c < n, "Column index out of bounds");                                        \
        KS_SIMD_HINT for (int32_t r = 0; r < n; ++r) {                                         \
            m->data[c * n + r] = v->data[r];                                                   \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _setr)(type * m, int32_t r, const vtype* v) {          \
        KS_ASSERT(m && v, "Null arguments");                                                   \
        KS_ASSERT(r < n, "Row index out of bounds");                                           \
        KS_SIMD_HINT for (int32_t c = 0; c < n; ++c) {                                         \
            m->data[c * n + r] = v->data[c];                                                   \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline ks_string KS_CONCAT2(type, _tostr)(const type* m) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        return _ks_mat_tostr_internal(#type, m->data, n);                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _print)(const type* m, FILE* stream) {                 \
        KS_ASSERT(m && stream, "Null arguments");                                              \
        ks_string s = KS_CONCAT2(type, _tostr)(m);                                             \
        fprintf(stream, "%s", ks_string_as_raw(&s));                                           \
        ks_string_free(&s);                                                                    \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _println)(const type* m, FILE* stream) {               \
        KS_CONCAT2(type, _print)(m, stream);                                                   \
        fprintf(stream, "\n");                                                                 \
    }

KS_MAT_FUNCTIONS(ks_mat2, ks_vec2, 2);
KS_MAT_FUNCTIONS(ks_mat3, ks_vec3, 3);
KS_MAT_FUNCTIONS(ks_mat4, ks_vec4, 4);

/* mat2 */

static inline float ks_mat2_det(const ks_mat2* m) {
    KS_ASSERT(m, "Null arguments");
    const float* d = m->data;
    return d[0] * d[3] - d[1] * d[2];
}

static inline void ks_mat2_inv(ks_mat2* out, const ks_mat2* m) {
    KS_ASSERT(out && m, "Null arguments");
    float det = ks_mat2_det(m);
    KS_ASSERT(det != 0.0f, "Matrix is not invertible");
    float inv = 1.0f / det;
    const float* d = m->data;
    float* o = out->data;

    o[0] = d[3] * inv;
    o[1] = -d[1] * inv;
    o[2] = -d[2] * inv;
    o[3] = d[0] * inv;
}

static inline void ks_mat2_invi(ks_mat2* m) {
    ks_mat2 tmp;
    ks_mat2_inv(&tmp, m);
    *m = tmp;
}

static inline void ks_mat2_rotate(ks_mat2* m, float angle) {
    KS_ASSERT(m, "Null arguments");
    float c = cosf(angle), s = sinf(angle);
    ks_mat2 R = ks_mat2_idnew();
    float* d = R.data;
    d[0] = c;
    d[1] = s;
    d[2] = -s;
    d[3] = c;
    ks_mat2_muli(m, &R);
}

static inline void ks_mat2_scale(ks_mat2* m, const ks_vec2* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    d[0] *= v->x;
    d[1] *= v->x;
    d[2] *= v->y;
    d[3] *= v->y;
}

/* mat3 */

static inline float ks_mat3_det(const ks_mat3* m) {
    KS_ASSERT(m, "Null arguments");
    const float* d = m->data;
    return d[0] * (d[4] * d[8] - d[5] * d[7]) - d[1] * (d[3] * d[8] - d[5] * d[6]) + d[2] * (d[3] * d[7] - d[4] * d[6]);
}

static inline void ks_mat3_inv(ks_mat3* out, const ks_mat3* m) {
    KS_ASSERT(out && m, "Null arguments");
    float det = ks_mat3_det(m);
    KS_ASSERT(det != 0.0f, "Matrix is not invertible");
    float inv = 1.0f / det;
    const float* d = m->data;
    float* o = out->data;

    o[0] = (d[4] * d[8] - d[5] * d[7]) * inv;
    o[1] = (d[2] * d[7] - d[1] * d[8]) * inv;
    o[2] = (d[1] * d[5] - d[2] * d[4]) * inv;
    o[3] = (d[5] * d[6] - d[3] * d[8]) * inv;
    o[4] = (d[0] * d[8] - d[2] * d[6]) * inv;
    o[5] = (d[2] * d[3] - d[0] * d[5]) * inv;
    o[6] = (d[3] * d[7] - d[4] * d[6]) * inv;
    o[7] = (d[1] * d[6] - d[0] * d[7]) * inv;
    o[8] = (d[0] * d[4] - d[1] * d[3]) * inv;
}

static inline void ks_mat3_invi(ks_mat3* m) {
    ks_mat3 tmp;
    ks_mat3_inv(&tmp, m);
    *m = tmp;
}

static inline void ks_mat3_translate(ks_mat3* m, const ks_vec2* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    d[6] += d[0] * v->x + d[3] * v->y;
    d[7] += d[1] * v->x + d[4] * v->y;
}

static inline void ks_mat3_rotate(ks_mat3* m, float angle) {
    KS_ASSERT(m, "Null arguments");
    float c = cosf(angle), s = sinf(angle);
    ks_mat3 R = ks_mat3_idnew();
    float* d = R.data;
    d[0] = c;
    d[1] = s;
    d[3] = -s;
    d[4] = c;
    ks_mat3_muli(m, &R);
}

static inline void ks_mat3_scale(ks_mat3* m, const ks_vec2* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    d[0] *= v->x;
    d[3] *= v->y;
}

static inline void ks_mat3_lookat(ks_mat3* out, const ks_vec2* eye, const ks_vec2* target) {
    KS_ASSERT(out && eye && target, "Null arguments");
    ks_vec2 f;  // front
    ks_vec2_sub(&f, target, eye);
    ks_vec2_normi(&f);
    ks_vec2 r = ks_vec2_new(-f.y, f.x);  // right

    ks_mat3_idinit(out);
    float* d = out->data;
    d[0] = r.x;
    d[3] = r.y;
    d[1] = f.x;
    d[4] = f.y;
    d[6] = -ks_vec2_dot(&r, eye);
    d[7] = -ks_vec2_dot(&f, eye);
}

static inline void ks_mat3_ortho(ks_mat3* out, float left, float right, float bottom, float top) {
    KS_ASSERT(out && right != left && top != bottom, "Invalid bounds");
    ks_mat3_idinit(out);
    float* d = out->data;
    d[0] = 2.0f / (right - left);
    d[4] = 2.0f / (top - bottom);
    d[6] = -(right + left) / (right - left);
    d[7] = -(top + bottom) / (top - bottom);
}

/* mat4 */

static inline float ks_mat4_det(const ks_mat4* m) {
    KS_ASSERT(m, "Null arguments");
    const float* d = m->data;

    float s0 = d[0] * d[5] - d[1] * d[4];
    float s1 = d[0] * d[6] - d[2] * d[4];
    float s2 = d[0] * d[7] - d[3] * d[4];
    float s3 = d[1] * d[6] - d[2] * d[5];
    float s4 = d[1] * d[7] - d[3] * d[5];
    float s5 = d[2] * d[7] - d[3] * d[6];

    float c5 = d[10] * d[15] - d[11] * d[14];
    float c4 = d[9] * d[15] - d[11] * d[13];
    float c3 = d[9] * d[14] - d[10] * d[13];
    float c2 = d[8] * d[15] - d[11] * d[12];
    float c1 = d[8] * d[14] - d[10] * d[12];
    float c0 = d[8] * d[13] - d[9] * d[12];

    return (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
}

static inline void ks_mat4_inv(ks_mat4* out, const ks_mat4* m) {
    KS_ASSERT(out && m, "Null arguments");
    float det = ks_mat4_det(m);
    KS_ASSERT(det != 0.0f, "Matrix is not invertible");
    float inv = 1.0f / det;
    const float* d = m->data;

    float c00 = d[10] * d[15] - d[11] * d[14], c01 = d[9] * d[15] - d[11] * d[13];
    float c02 = d[9] * d[14] - d[10] * d[13], c03 = d[8] * d[15] - d[11] * d[12];
    float c04 = d[8] * d[14] - d[10] * d[12], c05 = d[8] * d[13] - d[9] * d[12];
    float c12 = d[6] * d[11] - d[7] * d[10], c13 = d[5] * d[11] - d[7] * d[9];
    float c14 = d[5] * d[10] - d[6] * d[9], c15 = d[4] * d[11] - d[7] * d[8];
    float c16 = d[4] * d[10] - d[6] * d[8], c17 = d[4] * d[9] - d[5] * d[8];

    out->data[0] = (d[5] * c00 - d[6] * c01 + d[7] * c02) * inv;
    out->data[1] = (-d[1] * c00 + d[2] * c01 - d[3] * c02) * inv;
    out->data[2] = (d[13] * c12 - d[14] * c13 + d[15] * c14) * inv;
    out->data[3] = (-d[9] * c12 + d[10] * c13 - d[11] * c14) * inv;
    out->data[4] = (-d[4] * c00 + d[6] * c03 - d[7] * c04) * inv;
    out->data[5] = (d[0] * c00 - d[2] * c03 + d[3] * c04) * inv;
    out->data[6] = (-d[12] * c12 + d[14] * c15 - d[15] * c16) * inv;
    out->data[7] = (d[8] * c12 - d[10] * c15 + d[11] * c16) * inv;
    out->data[8] = (d[4] * c01 - d[5] * c03 + d[7] * c05) * inv;
    out->data[9] = (-d[0] * c01 + d[1] * c03 - d[3] * c05) * inv;
    out->data[10] = (d[12] * c13 - d[13] * c15 + d[15] * c17) * inv;
    out->data[11] = (-d[8] * c13 + d[9] * c15 - d[11] * c17) * inv;
    out->data[12] = (-d[4] * c02 + d[5] * c04 - d[6] * c05) * inv;
    out->data[13] = (d[0] * c02 - d[1] * c04 + d[2] * c05) * inv;
    out->data[14] = (-d[12] * c14 + d[13] * c16 - d[14] * c17) * inv;
    out->data[15] = (d[8] * c14 - d[9] * c16 + d[10] * c17) * inv;
}

static inline void ks_mat4_invi(ks_mat4* m) {
    ks_mat4 tmp;
    ks_mat4_inv(&tmp, m);
    *m = tmp;
}

static inline void ks_mat4_translate(ks_mat4* m, const ks_vec3* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    d[12] += d[0] * v->x + d[4] * v->y + d[8] * v->z;
    d[13] += d[1] * v->x + d[5] * v->y + d[9] * v->z;
    d[14] += d[2] * v->x + d[6] * v->y + d[10] * v->z;
    d[15] += d[3] * v->x + d[7] * v->y + d[11] * v->z;
}

// Rodrigues formula
static inline void ks_mat4_rotate(ks_mat4* m, float angle, const ks_vec3* axis) {
    KS_ASSERT(m && axis, "Null arguments");
    float c = cosf(angle), s = sinf(angle), t = 1.0f - c;
    float x = axis->x, y = axis->y, z = axis->z;

    ks_mat4 R = ks_mat4_idnew();
    float* d = R.data;
    d[0] = c + x * x * t;
    d[4] = x * y * t - z * s;
    d[8] = x * z * t + y * s;
    d[1] = y * x * t + z * s;
    d[5] = c + y * y * t;
    d[9] = y * z * t - x * s;
    d[2] = z * x * t - y * s;
    d[6] = z * y * t + x * s;
    d[10] = c + z * z * t;

    ks_mat4_muli(m, &R);
}

static inline void ks_mat4_scale(ks_mat4* m, const ks_vec3* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    d[0] *= v->x;
    d[1] *= v->x;
    d[2] *= v->x;
    d[3] *= v->x;
    d[4] *= v->y;
    d[5] *= v->y;
    d[6] *= v->y;
    d[7] *= v->y;
    d[8] *= v->z;
    d[9] *= v->z;
    d[10] *= v->z;
    d[11] *= v->z;
}

static inline void ks_mat4_lookat(ks_mat4* out, const ks_vec3* eye, const ks_vec3* center, const ks_vec3* up) {
    KS_ASSERT(out && eye && center && up, "Null arguments");
    ks_vec3 f, s, u;

    ks_vec3_sub(&f, center, eye);
    ks_vec3_normi(&f);

    ks_vec3_cross(&s, &f, up);
    ks_vec3_normi(&s);

    ks_vec3_cross(&u, &s, &f);

    ks_mat4_idinit(out);
    float* d = out->data;
    d[0] = s.x;
    d[4] = s.y;
    d[8] = s.z;
    d[1] = u.x;
    d[5] = u.y;
    d[9] = u.z;
    d[2] = -f.x;
    d[6] = -f.y;
    d[10] = -f.z;
    d[12] = -ks_vec3_dot(&s, eye);
    d[13] = -ks_vec3_dot(&u, eye);
    d[14] = ks_vec3_dot(&f, eye);
}

static inline void ks_mat4_perspective(ks_mat4* out, float fovy_rad, float aspect, float nearz, float farz) {
    KS_ASSERT(out && aspect != 0.0f && nearz != farz, "Invalid bounds");
    float t = tanf(fovy_rad / 2.0f);

    ks_mat4_zeroinit(out);
    float* d = out->data;
    d[0] = 1.0f / (aspect * t);
    d[5] = 1.0f / (t);
    d[10] = -(farz + nearz) / (farz - nearz);
    d[11] = -1.0f;
    d[14] = -(2.0f * farz * nearz) / (farz - nearz);
}

static inline void ks_mat4_ortho(ks_mat4* out, float left, float right, float bottom, float top, float nearz,
                                 float farz) {
    KS_ASSERT(out && left != right && bottom != top && nearz != farz, "Invalid bounds");
    ks_mat4_idinit(out);
    float* d = out->data;
    d[0] = 2.0f / (right - left);
    d[5] = 2.0f / (top - bottom);
    d[10] = -2.0f / (farz - nearz);
    d[12] = -(right + left) / (right - left);
    d[13] = -(top + bottom) / (top - bottom);
    d[14] = -(farz + nearz) / (farz - nearz);
}

/* Derivatives and integrals */

KS_FUNC(double, realf_1d, double x, void* args);
KS_FUNC(double, realf_2d, double x, double y, void* args);
KS_FUNC(double, realf_nd, const double* vars, int32_t dims, void* args);
KS_FUNC(ks_vec2, odef_2d, double t, ks_vec2 state, void* args);
KS_FUNC(ks_vec3, odef_3d, double t, ks_vec3 state, void* args);

static inline double ks_deriv_1d(ks_realf_1d f, double x, double h, void* args) {
    return (f(x + h, args) - f(x - h, args)) / (2.0 * h);
}

static inline double ks_deriv_2d_x(ks_realf_2d f, double x, double y, double h, void* args) {
    return (f(x + h, y, args) - f(x - h, y, args)) / (2.0 * h);
}

static inline double ks_deriv_2d_y(ks_realf_2d f, double x, double y, double h, void* args) {
    return (f(x, y + h, args) - f(x, y - h, args)) / (2.0 * h);
}

static inline double ks_integ_1d(ks_realf_1d f, double a, double b, int32_t steps, void* args) {
    if (steps % 2 != 0) {
        ++steps;
    }

    double dx = (b - a) / steps;
    double sum = f(a, args) + f(b, args);

    for (int32_t i = 1; i < steps; ++i) {
        double x = a + i * dx;
        sum += f(x, args) * ((i % 2 == 0) ? 2.0 : 4.0);
    }

    return sum * (dx / 3.0);
}

static inline double ks_integ_2d(ks_realf_2d f, double xa, double xb, int32_t stepsx, double ya, double yb,
                                 int32_t stepsy, void* args) {
    if (stepsx % 2 != 0) {
        ++stepsx;
    }
    if (stepsy % 2 != 0) {
        ++stepsy;
    }

    double dx = (xb - xa) / stepsx;
    double dy = (yb - ya) / stepsy;
    double sum = 0.0;

    for (int32_t i = 0; i <= stepsx; ++i) {
        double x = xa + i * dx;
        double wx = (i == 0 || i == stepsx) ? 1.0 : ((i % 2 == 0) ? 2.0 : 4.0);

        for (int32_t j = 0; j <= stepsy; ++j) {
            double y = ya + j * dy;
            double wy = (j == 0 || j == stepsy) ? 1.0 : ((j % 2 == 0) ? 2.0 : 4.0);
            sum += wx * wy * f(x, y, args);
        }
    }

    return sum * ((dx * dy) / 9.0);
}

static inline double ks_integ_nd(ks_realf_nd f, int32_t dims, const double* min_bounds, const double* max_bounds,
                                 int32_t samples, void* args) {
    double volume = 1.0;
    for (int32_t i = 0; i < dims; ++i) {
        volume *= (max_bounds[i] - min_bounds[i]);
    }

    double sum = 0.0;
    double pt[dims];

    for (int32_t s = 0; s < samples; ++s) {
        for (int32_t d = 0; d < dims; ++d) {
            double r = (double)rand() / RAND_MAX;
            pt[d] = min_bounds[d] + r * (max_bounds[d] - min_bounds[d]);
        }

        sum += f(pt, dims, args);
    }

    return volume * (sum / (double)samples);
}

/* ODE solvers */

static inline void ks_euler(double* val, double rate, double dt) {
    *val += rate * dt;
}

static inline void ks_semi_euler_2d(ks_vec2* pos, ks_vec2* vel, ks_vec2 accel, double dt) {
    vel->x += accel.x * dt;
    vel->y += accel.y * dt;

    pos->x += vel->x * dt;
    pos->y += vel->y * dt;
}

static inline void ks_semi_euler_3d(ks_vec3* pos, ks_vec3* vel, ks_vec3 accel, double dt) {
    vel->x += accel.x * dt;
    vel->y += accel.y * dt;
    vel->z += accel.z * dt;

    pos->x += vel->x * dt;
    pos->y += vel->y * dt;
    pos->z += vel->z * dt;
}

static inline ks_vec2 ks_rk4_2d(ks_odef_2d f, double t, ks_vec2 state, double dt, void* args) {
    double dt_2 = dt / 2.0;

    ks_vec2 k1 = f(t, state, args);

    ks_vec2 state_k2 = KS_VEC2(state.x + k1.x * dt_2, state.y + k1.y * dt_2);
    ks_vec2 k2 = f(t + dt_2, state_k2, args);

    ks_vec2 state_k3 = KS_VEC2(state.x + k2.x * dt_2, state.y + k2.y * dt_2);
    ks_vec2 k3 = f(t + dt_2, state_k3, args);

    ks_vec2 state_k4 = KS_VEC2(state.x + k3.x * dt, state.y + k3.y * dt);
    ks_vec2 k4 = f(t + dt, state_k4, args);

    ks_vec2 next_state;
    next_state.x = state.x + (dt / 6.0) * (k1.x + 2.0 * k2.x + 2.0 * k3.x + k4.x);
    next_state.y = state.y + (dt / 6.0) * (k1.y + 2.0 * k2.y + 2.0 * k3.y + k4.y);

    return next_state;
}

static inline ks_vec3 ks_rk4_3d(ks_odef_3d f, double t, ks_vec3 state, double dt, void* args) {
    double dt_2 = dt / 2.0;

    ks_vec3 k1 = f(t, state, args);

    ks_vec3 state_k2 = KS_VEC3(state.x + k1.x * dt_2, state.y + k1.y * dt_2, state.z + k1.z * dt_2);
    ks_vec3 k2 = f(t + dt_2, state_k2, args);

    ks_vec3 state_k3 = KS_VEC3(state.x + k2.x * dt_2, state.y + k2.y * dt_2, state.z + k2.z * dt_2);
    ks_vec3 k3 = f(t + dt_2, state_k3, args);

    ks_vec3 state_k4 = KS_VEC3(state.x + k3.x * dt, state.y + k3.y * dt, state.z + k3.z * dt);
    ks_vec3 k4 = f(t + dt, state_k4, args);

    ks_vec3 next_state;
    next_state.x = state.x + (dt / 6.0) * (k1.x + 2.0 * k2.x + 2.0 * k3.x + k4.x);
    next_state.y = state.y + (dt / 6.0) * (k1.y + 2.0 * k2.y + 2.0 * k3.y + k4.y);
    next_state.z = state.z + (dt / 6.0) * (k1.z + 2.0 * k2.z + 2.0 * k3.z + k4.z);

    return next_state;
}

/* Scalar and vector fields */

KS_STRUCT(field, {
    int32_t width, height, depth;
    size_t typesize;
    double cellsize;
    void* data;
});

KS_STRUCT(field_ctx, {
    double x, y, z, t;
    const ks_field* dst;
    const ks_field* src;
});

KS_FUNC(void, field_cb, ks_field_ctx* ctx, void* out);

KS_API ks_field ks_field_create(int32_t w, int32_t h, int32_t d, size_t typesize, double cellsize);
KS_API void ks_field_sample(ks_field* dst, const ks_field* src, double t, ks_field_cb cb);
KS_API void ks_field_destroy(ks_field* field);

static inline int32_t _ks_f2_idx_internal(int32_t x, int32_t y, int32_t w) {
    return y * w + x;
}

static inline int32_t _ks_f3_idx_internal(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h) {
    return z * w * h + y * w + x;
}

static inline ks_vec2 ks_sf2_grad(const ks_field* f, int32_t x, int32_t y) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    double h2 = 2.0 * f->cellsize;

    int32_t xi = _ks_f2_idx_internal(x1, y, f->width);
    int32_t xj = _ks_f2_idx_internal(x0, y, f->width);
    int32_t yi = _ks_f2_idx_internal(x, y1, f->width);
    int32_t yj = _ks_f2_idx_internal(x, y0, f->width);

    double* data = f->data;

    double dx = (data[xi] - data[xj]) / h2;
    double dy = (data[yi] - data[yj]) / h2;

    return KS_VEC2(dx, dy);
}

static inline double ks_vf2_div(const ks_field* f, int32_t x, int32_t y) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    double h2 = 2.0 * f->cellsize;

    int32_t xi = _ks_f2_idx_internal(x1, y, f->width);
    int32_t xj = _ks_f2_idx_internal(x0, y, f->width);
    int32_t yi = _ks_f2_idx_internal(x, y1, f->width);
    int32_t yj = _ks_f2_idx_internal(x, y0, f->width);

    ks_vec2* data = f->data;

    double dfx_dx = (data[xi].x - data[xj].x) / h2;
    double dfy_dy = (data[yi].y - data[yj].y) / h2;
    return dfx_dx + dfy_dy;
}

static inline double ks_vf2_curl(const ks_field* f, int32_t x, int32_t y) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    double h2 = 2.0 * f->cellsize;

    int32_t xi = _ks_f2_idx_internal(x1, y, f->width);
    int32_t xj = _ks_f2_idx_internal(x0, y, f->width);
    int32_t yi = _ks_f2_idx_internal(x, y1, f->width);
    int32_t yj = _ks_f2_idx_internal(x, y0, f->width);

    ks_vec2* data = f->data;

    double dfy_dx = (data[xi].y - data[xj].y) / h2;
    double dfx_dy = (data[yi].x - data[yj].x) / h2;
    return dfy_dx - dfx_dy;
}

static inline double ks_sf2_lap(const ks_field* f, int32_t x, int32_t y) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    double hsq = f->cellsize * f->cellsize;

    int32_t ci = _ks_f2_idx_internal(x, y, f->width);
    int32_t li = _ks_f2_idx_internal(x0, y, f->width);
    int32_t ri = _ks_f2_idx_internal(x1, y, f->width);
    int32_t di = _ks_f2_idx_internal(x, y0, f->width);
    int32_t ui = _ks_f2_idx_internal(x, y1, f->width);

    double* data = f->data;

    double center = data[ci];
    double left = data[li];
    double right = data[ri];
    double down = data[di];
    double up = data[ui];

    return (left + right + up + down - 4.0 * center) / hsq;
}

static inline ks_vec3 ks_sf3_grad(const ks_field* f, int32_t x, int32_t y, int32_t z) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    int32_t z0 = KS_CLAMP(z - 1, 0, f->depth - 1);
    int32_t z1 = KS_CLAMP(z + 1, 0, f->depth - 1);
    double h2 = 2.0 * f->cellsize;

    int32_t xi = _ks_f3_idx_internal(x1, y, z, f->width, f->height);
    int32_t xj = _ks_f3_idx_internal(x0, y, z, f->width, f->height);
    int32_t yi = _ks_f3_idx_internal(x, y1, z, f->width, f->height);
    int32_t yj = _ks_f3_idx_internal(x, y0, z, f->width, f->height);
    int32_t zi = _ks_f3_idx_internal(x, y, z1, f->width, f->height);
    int32_t zj = _ks_f3_idx_internal(x, y, z0, f->width, f->height);

    double* data = f->data;

    double dx = (data[xi] - data[xj]) / h2;
    double dy = (data[yi] - data[yj]) / h2;
    double dz = (data[zi] - data[zj]) / h2;

    return KS_VEC3(dx, dy, dz);
}

static inline double ks_vf3_div(const ks_field* f, int32_t x, int32_t y, int32_t z) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    int32_t z0 = KS_CLAMP(z - 1, 0, f->depth - 1);
    int32_t z1 = KS_CLAMP(z + 1, 0, f->depth - 1);
    double h2 = 2.0 * f->cellsize;

    int32_t xi = _ks_f3_idx_internal(x1, y, z, f->width, f->height);
    int32_t xj = _ks_f3_idx_internal(x0, y, z, f->width, f->height);
    int32_t yi = _ks_f3_idx_internal(x, y1, z, f->width, f->height);
    int32_t yj = _ks_f3_idx_internal(x, y0, z, f->width, f->height);
    int32_t zi = _ks_f3_idx_internal(x, y, z1, f->width, f->height);
    int32_t zj = _ks_f3_idx_internal(x, y, z0, f->width, f->height);

    ks_vec3* data = f->data;

    double dfx_dx = (data[xi].x - data[xj].x) / h2;
    double dfy_dy = (data[yi].y - data[yj].y) / h2;
    double dfz_dz = (data[zi].z - data[zj].z) / h2;

    return dfx_dx + dfy_dy + dfz_dz;
}

static inline ks_vec3 ks_vf3_curl(const ks_field* f, int32_t x, int32_t y, int32_t z) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    int32_t z0 = KS_CLAMP(z - 1, 0, f->depth - 1);
    int32_t z1 = KS_CLAMP(z + 1, 0, f->depth - 1);
    double h2 = 2.0 * f->cellsize;

    int32_t xi = _ks_f3_idx_internal(x1, y, z, f->width, f->height);
    int32_t xj = _ks_f3_idx_internal(x0, y, z, f->width, f->height);
    int32_t yi = _ks_f3_idx_internal(x, y1, z, f->width, f->height);
    int32_t yj = _ks_f3_idx_internal(x, y0, z, f->width, f->height);
    int32_t zi = _ks_f3_idx_internal(x, y, z1, f->width, f->height);
    int32_t zj = _ks_f3_idx_internal(x, y, z0, f->width, f->height);

    ks_vec3* data = f->data;

    double dfz_dy = (data[yi].z - data[yj].z) / h2;
    double dfy_dz = (data[zi].y - data[zj].y) / h2;

    double dfx_dz = (data[zi].x - data[zj].x) / h2;
    double dfz_dx = (data[xi].z - data[xj].z) / h2;

    double dfy_dx = (data[xi].y - data[xj].y) / h2;
    double dfx_dy = (data[yi].x - data[yj].x) / h2;

    return KS_VEC3(dfz_dy - dfy_dz, dfx_dz - dfz_dx, dfy_dx - dfx_dy);
}

static inline double ks_sf3_lap(const ks_field* f, int32_t x, int32_t y, int32_t z) {
    int32_t x0 = KS_CLAMP(x - 1, 0, f->width - 1);
    int32_t x1 = KS_CLAMP(x + 1, 0, f->width - 1);
    int32_t y0 = KS_CLAMP(y - 1, 0, f->height - 1);
    int32_t y1 = KS_CLAMP(y + 1, 0, f->height - 1);
    int32_t z0 = KS_CLAMP(z - 1, 0, f->depth - 1);
    int32_t z1 = KS_CLAMP(z + 1, 0, f->depth - 1);
    double hsq = f->cellsize * f->cellsize;

    int32_t ci = _ks_f3_idx_internal(x, y, z, f->width, f->height);
    int32_t li = _ks_f3_idx_internal(x0, y, z, f->width, f->height);
    int32_t ri = _ks_f3_idx_internal(x1, y, z, f->width, f->height);
    int32_t di = _ks_f3_idx_internal(x, y0, z, f->width, f->height);
    int32_t ui = _ks_f3_idx_internal(x, y1, z, f->width, f->height);
    int32_t bi = _ks_f3_idx_internal(x, y, z0, f->width, f->height);
    int32_t fi = _ks_f3_idx_internal(x, y, z1, f->width, f->height);

    double* data = f->data;

    double center = data[ci];
    double left = data[li];
    double right = data[ri];
    double down = data[di];
    double up = data[ui];
    double back = data[bi];
    double front = data[fi];

    return (left + right + up + down + back + front - 6.0 * center) / hsq;
}

/* FFT */

static inline size_t bit_reverse(size_t n, size_t m) {
    size_t r = 0;
    size_t l = log2ld(m);
    for (size_t i = 0; i < l; ++i) {
        if ((n >> i) & 1) {
            r |= (1ul << (l - 1 - i));
        }
    }
    return r;
}

KS_API int fft(size_t n, double complex samples[static n]);

#endif  // KS_MATH_H

#if defined(KS_MATH_IMPL) && !defined(KS_MATH_IMPL_DONE)
#define KS_MATH_IMPL_DONE

KS_API ks_field ks_field_create(int32_t w, int32_t h, int32_t d, size_t typesize, double cellsize) {
    void* data = malloc(typesize * (size_t)(w * h));
    KS_ASSERT(data, "OOM");

    return (ks_field){w, h, d, typesize, cellsize, data};
}

KS_API void ks_field_sample(ks_field* dst, const ks_field* src, double t, ks_field_cb cb) {
    KS_ASSERT_NONNULL_ARGS(dst && cb);

    ks_field_ctx ctx = {0};
    ctx.dst = dst;
    ctx.src = src;
    ctx.t = t;

    for (int32_t z = 0; z < dst->depth; ++z) {
        for (int32_t y = 0; y < dst->height; ++y) {
            for (int32_t x = 0; x < dst->width; ++x) {
                ctx.x = (double)x;
                ctx.y = (double)y;
                ctx.z = (double)z;

                int32_t idx = _ks_f3_idx_internal(x, y, z, dst->width, dst->height);
                void* target = KS_PTROFF(dst->data, (size_t)idx * dst->typesize);
                cb(&ctx, target);
            }
        }
    }
}

KS_API void ks_field_destroy(ks_field* f) {
    KS_ASSERT_NONNULL_ARGS(f);
    free(f->data);
    memset(f, 0, sizeof(ks_field));
}

KS_API int fft(size_t n, double complex samples[static n]) {
    if (!KS_ISPOW2(n)) {
        return 1;
    }

// Reorder the array using bitwise-reversed indices
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        size_t j = bit_reverse(i, n);
        if (i < j) {
            double complex temp = samples[i];
            samples[i] = samples[j];
            samples[j] = temp;
        }
    }

    const size_t l = log2ld(n);
    const double complex f = -KS_PI_2 * I;
    for (size_t i = 1; i <= l; ++i) {
        size_t m = 1ull << i;  // 2^i
        double complex wm = cexpf(f / m);

#pragma omp parallel for
        for (size_t j = 0; j < n; j += m) {
            double complex w = 1;
            for (size_t k = 0; k <= m / 2 - 1; ++k) {
                double complex t = w * samples[j + k + m / 2];
                double complex u = samples[j + k];
                samples[j + k] = u + t;
                samples[j + k + m / 2] = u - t;
                w *= wm;
            }
        }
    }

    return 0;
}

#endif  // KS_MATH_IMPL