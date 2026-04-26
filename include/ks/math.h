#pragma once

#include <ks/core.h>
#include <math.h>

KS_UNION(vec2, {
    struct {
        float x;
        float y;
    };
    float data[2];
});

KS_UNION(vec3, {
    struct {
        float x;
        float y;
        float z;
    };
    float data[3];
});

KS_UNION(vec4, {
    struct {
        float x;
        float y;
        float z;
        float w;
    };
    float data[4];
});

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

static inline ks_string _ks_vec_tostr_internal(const char* type_name, const float* data, int n) {
    ks_string s = ks_string_format("%s(", type_name);

    for (int i = 0; i < n; ++i) {
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

static inline ks_string _ks_mat_tostr_internal(const char* type_name, const float* data, int n) {
    ks_string s = ks_string_format("%s(\n", type_name);

    int m = n * n;
    for (int i = 0; i < m; ++i) {
        // Add indentation
        if (i == 0 || (i - n) % n == 0) {
            ks_string_append_raw2(&s, "\t", 1);
        }

        ks_string num = ks_string_format("%f", data[i]);
        ks_string_append(&s, &num);
        ks_string_free(&num);

        // Add a comma unless it's the last element
        if (i < m - 1) {
            ks_string_append_raw2(&s, ", ", 2);
        }

        // Add newlines
        if (i < m - 1 && (i + 1) % n == 0) {
            ks_string_append_raw2(&s, "\n", 1);
        }
    }

    ks_string_append_raw2(&s, "\n)", 2);
    return s;
}

#define KS_VEC_FUNCTIONS(type, n, params, args)                                                       \
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
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v->data[i] = num;                                                                         \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sum)(type * out, const type* v1, const type* v2) {           \
        KS_ASSERT(out && v1 && v2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            out->data[i] = v1->data[i] + v2->data[i];                                                 \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sumi)(type * v1, const type* v2) {                           \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v1->data[i] += v2->data[i];                                                               \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _neg)(type * out, const type* v) {                            \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            out->data[i] = -v->data[i];                                                               \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _negi)(type * v) {                                            \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v->data[i] = -v->data[i];                                                                 \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sub)(type * out, const type* v1, const type* v2) {           \
        KS_ASSERT(out && v1 && v2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            out->data[i] = v1->data[i] - v2->data[i];                                                 \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _subi)(type * v1, const type* v2) {                           \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v1->data[i] -= v2->data[i];                                                               \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _smul)(type * out, const type* v, float num) {                \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            out->data[i] = v->data[i] * num;                                                          \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _smuli)(type * v, float num) {                                \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v->data[i] *= num;                                                                        \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _div)(type * out, const type* v, float num) {                 \
        KS_ASSERT(out && v, "Null arguments");                                                        \
        KS_ASSERT(num != 0.0f, "Zero division");                                                      \
        float inv = 1.0f / num;                                                                       \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            out->data[i] = v->data[i] * inv;                                                          \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _divi)(type * v, float num) {                                 \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_ASSERT(num != 0.0f, "Zero division");                                                      \
        float inv = 1.0f / num;                                                                       \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v->data[i] *= inv;                                                                        \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _dot)(const type* v1, const type* v2) {                      \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        float res = 0;                                                                                \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
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
        KS_CONCAT2(type, _div)(out, v, len);                                                          \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _normi)(type * v) {                                           \
        float len = KS_CONCAT2(type, _length)(v);                                                     \
        KS_ASSERT(len != 0.0f, "Normalizing zero vector");                                            \
        KS_CONCAT2(type, _divi)(v, len);                                                              \
    }                                                                                                 \
                                                                                                      \
    static inline float KS_CONCAT2(type, _dist_sq)(const type* v1, const type* v2) {                  \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        float res = 0;                                                                                \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
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
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            out->data[i] = (1 - t) * v1->data[i] + t * v2->data[i];                                   \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _lerpi)(type * v1, const type* v2, float t) {                 \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int i = 0; i < n; ++i) {                                                    \
            v1->data[i] = (1 - t) * v1->data[i] + t * v2->data[i];                                    \
        }                                                                                             \
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

/* cross product */

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

#define KS_MAT_FUNCTIONS(type, n)                                                           \
    static inline type KS_CONCAT2(type, _id)(void) {                                        \
        type m = {0};                                                                       \
        for (int i = 0; i < n; ++i) {                                                       \
            m.data[i * n + i] = 1.0f;                                                       \
        }                                                                                   \
                                                                                            \
        return m;                                                                           \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _fill)(type * m, float num) {                       \
        KS_ASSERT(m, "Null arguments");                                                     \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            m->data[i] = num;                                                               \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _sum)(type * out, const type* m1, const type* m2) { \
        KS_ASSERT(out && m1 && m2, "Null arguments");                                       \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            out->data[i] = m1->data[i] + m2->data[i];                                       \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _sumi)(type * m1, const type* m2) {                 \
        KS_ASSERT(m1 && m2, "Null arguments");                                              \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            m1->data[i] += m2->data[i];                                                     \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _neg)(type * out, const type* m) {                  \
        KS_ASSERT(out && m, "Null arguments");                                              \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            out->data[i] = -m->data[i];                                                     \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _negi)(type * m) {                                  \
        KS_ASSERT(m, "Null arguments");                                                     \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            m->data[i] = -m->data[i];                                                       \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _sub)(type * out, const type* m1, const type* m2) { \
        KS_ASSERT(out && m1 && m2, "Null arguments");                                       \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            out->data[i] = m1->data[i] - m2->data[i];                                       \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _subi)(type * m1, const type* m2) {                 \
        KS_ASSERT(m1 && m2, "Null arguments");                                              \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            m1->data[i] -= m2->data[i];                                                     \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _smul)(type * out, const type* m, float num) {      \
        KS_ASSERT(out && m, "Null arguments");                                              \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            out->data[i] = m->data[i] * num;                                                \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _smuli)(type * m, float num) {                      \
        KS_ASSERT(m, "Null arguments");                                                     \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            m->data[i] *= num;                                                              \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _div)(type * out, const type* m, float num) {       \
        KS_ASSERT(out && m, "Null arguments");                                              \
        KS_ASSERT(num != 0.0f, "Zero division");                                            \
        float inv = 1.0f / num;                                                             \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            out->data[i] = m->data[i] * inv;                                                \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _divi)(type * m, float num) {                       \
        KS_ASSERT(m, "Null arguments");                                                     \
        KS_ASSERT(num != 0.0f, "Zero division");                                            \
        float inv = 1.0f / num;                                                             \
        KS_SIMD_HINT for (int i = 0; i < n * n; ++i) {                                      \
            m->data[i] *= inv;                                                              \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline ks_string KS_CONCAT2(type, _tostr)(const type* m) {                       \
        KS_ASSERT(m, "Null arguments");                                                     \
        return _ks_mat_tostr_internal(#type, m->data, n);                                   \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _print)(const type* m, FILE* stream) {              \
        KS_ASSERT(m && stream, "Null arguments");                                           \
        ks_string s = KS_CONCAT2(type, _tostr)(m);                                          \
        fprintf(stream, "%s", ks_string_as_raw(&s));                                        \
        ks_string_free(&s);                                                                 \
    }                                                                                       \
                                                                                            \
    static inline void KS_CONCAT2(type, _println)(const type* m, FILE* stream) {            \
        KS_CONCAT2(type, _print)(m, stream);                                                \
        fprintf(stream, "\n");                                                              \
    }

KS_MAT_FUNCTIONS(ks_mat2, 2);
KS_MAT_FUNCTIONS(ks_mat3, 3);
KS_MAT_FUNCTIONS(ks_mat4, 4);

#ifdef KS_MATH_IMPL

#endif