#ifndef KS_MATH_H
#define KS_MATH_H

#include <ks/ds.h>

#include <complex.h>
#include <omp.h>
#include <time.h>

// clang-format off
#define KS_E            2.7182818284590452354       // e
#define KS_LOG2E        1.4426950408889634074       // log2 e
#define KS_LOG10E       0.43429448190325182765      // log10 e
#define KS_LN2          0.69314718055994530942      // loge 2
#define KS_LN10         2.30258509299404568402      // loge 10
#define KS_PI           3.14159265358979323846      // pi
#define KS_2PI          6.28318530718               // 2pi
#define KS_PI_2         1.57079632679489661923      // pi/2
#define KS_PI_4         0.78539816339744830962      // pi/4
#define KS_1_PI         0.31830988618379067154      // 1/pi
#define KS_2_PI         0.63661977236758134308      // 2/pi
#define KS_SQRTPI       1.77245385091               // sqrt(pi)
#define KS_1_SQRTPI     0.564189583548              // 1/sqrt(pi)
#define KS_1_SQRT2PI    0.398942280401              // 1/sqrt(2pi)
#define KS_2_SQRTPI     1.12837916709551257390      // 2/sqrt(pi)
#define KS_2_SQRT2PI    0.797884560803              // 2/sqrt(2pi)
#define KS_SQRT2        1.41421356237309504880      // sqrt(2)
#define KS_1_SQRT2      0.70710678118654752440      // 1/sqrt(2)
#define KS_SQRT3        1.73205080757               // sqrt(3)
#define KS_1_SQRT3      0.57735026919               // 1/sqrt(3)
// clang-format on

#define KS_DEG2RAD (KS_PI / 180.0)
#define KS_RAD2DEG (180.0 / KS_PI)

#define KS_TENSOR_MAX_DIMS 8

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

#define KS_VEC2(x, y) ((ks_vec2){{x, y}})

KS_UNION(vec3, {
    struct {
        float x;
        float y;
        float z;
    };
    float data[3];
});

#define KS_VEC3(x, y, z) ((ks_vec3){{x, y, z}})

KS_UNION(vec4, {
    struct {
        float x;
        float y;
        float z;
        float w;
    };
    float data[4];
});

#define KS_VEC4(x, y, z, w) ((ks_vec4){{x, y, z, w}})

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

#ifdef KS_TENSOR_USE_64BIT
typedef int64_t ks_tensor_int;
#define KS_TENSOR_INT_FMT "%" PRId64
#else
typedef int32_t ks_tensor_int;
#define KS_TENSOR_INT_FMT "%d"
#endif

KS_STRUCT(tensor, {
    const ks_allocator* allocator;
    int32_t ndims;
    ks_tensor_int shape[KS_TENSOR_MAX_DIMS];
    ks_tensor_int strides[KS_TENSOR_MAX_DIMS];
    float* data;
});

KS_ENUM(euler_order, {KS_EULER_XYZ = 0, KS_EULER_XZY, KS_EULER_YXZ, KS_EULER_YZX, KS_EULER_ZXY, KS_EULER_ZYX});

KS_STRUCT(euler, {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float alpha;
            float beta;
            float gamma;
        };
        struct {
            float phi;
            float theta;
            float psi;
        };
        struct {
            float pitch;
            float yaw;
            float roll;
        };
    };
    ks_euler_order order;
});

#define KS_EULER(x, y, z, o) ((ks_euler){{{x, y, z}}, o})

KS_UNION(quat, {
    struct {
        float w;
        float x;
        float y;
        float z;
    };
    struct {
        float a;
        float b;
        float c;
        float d;
    };
});

#define KS_QUAT(w, x, y, z) ((ks_quat){{w, x, y, z}})

#define KS_VEC2_FMT "Vec2(%.3f, %.3f)"
#define KS_VEC2_ARGS(v) (v).data[0], (v).data[1]

#define KS_VEC3_FMT "Vec3(%.3f, %.3f, %.3f)"
#define KS_VEC3_ARGS(v) (v).data[0], (v).data[1], (v).data[2]

#define KS_VEC4_FMT "Vec4(%.3f, %.3f, %.3f, %.3f)"
#define KS_VEC4_ARGS(v) (v).data[0], (v).data[1], (v).data[2], (v).data[3]

#define KS_MAT2_FMT      \
    "Mat2:"              \
    "\t| %.3f, %.3f |\n" \
    "\t| %.3f, %.3f |"
#define KS_MAT2_ARGS(m) (m).data[0], (m).data[2], (m).data[1], (m).data[3]

#define KS_MAT3_FMT            \
    "Mat3:"                    \
    "\t| %.3f, %.3f, %.3f |\n" \
    "\t| %.3f, %.3f, %.3f |\n" \
    "\t| %.3f, %.3f, %.3f |"
#define KS_MAT3_ARGS(m) \
    (m).data[0], (m).data[3], (m).data[6], (m).data[1], (m).data[4], (m).data[7], (m).data[2], (m).data[5], (m).data[8]

#define KS_MAT4_FMT                  \
    "Mat4: "                         \
    "\t| %.3f, %.3f, %.3f, %.3f |\n" \
    "\t| %.3f, %.3f, %.3f, %.3f |\n" \
    "\t| %.3f, %.3f, %.3f, %.3f |\n" \
    "\t| %.3f, %.3f, %.3f, %.3f |"
#define KS_MAT4_ARGS(m)                                                                                       \
    (m).data[0], (m).data[4], (m).data[8], (m).data[12], (m).data[1], (m).data[5], (m).data[9], (m).data[13], \
        (m).data[2], (m).data[6], (m).data[10], (m).data[14], (m).data[3], (m).data[7], (m).data[11], (m).data[15]

/* Vectors */

#define KS_VEC_FUNCTIONS(type, n, params, args)                                                       \
    static inline void KS_CONCAT2(type, _zeroes)(type * v) {                                          \
        KS_ASSERT(v, "Null arguments");                                                               \
        memset(v, 0, sizeof(type));                                                                   \
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
        KS_ASSERT(KS_FNZERO(num, KS_FEPS_MATH), "Division by zero");                                  \
        float inv = 1.0f / num;                                                                       \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            out->data[i] = v->data[i] * inv;                                                          \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _sdivi)(type * v, float num) {                                \
        KS_ASSERT(v, "Null arguments");                                                               \
        KS_ASSERT(KS_FNZERO(num, KS_FEPS_MATH), "Division by zero");                                  \
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
        KS_ASSERT(KS_FNZERO(len, KS_FEPS_MATH), "Normalizing zero vector");                           \
        KS_CONCAT2(type, _sdiv)(out, v, len);                                                         \
    }                                                                                                 \
                                                                                                      \
    static inline void KS_CONCAT2(type, _normi)(type * v) {                                           \
        float len = KS_CONCAT2(type, _length)(v);                                                     \
        KS_ASSERT(KS_FNZERO(len, KS_FEPS_MATH), "Normalizing zero vector");                           \
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
    static inline bool KS_CONCAT2(type, _equals)(const type* v1, const type* v2) {                    \
        KS_ASSERT(v1 && v2, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            if (KS_FNEQ(v1->data[i], v2->data[i], KS_FEPS_MATH)) {                                    \
                return false;                                                                         \
            }                                                                                         \
        }                                                                                             \
                                                                                                      \
        return true;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    static inline bool KS_CONCAT2(type, _isunitary)(const type* v) {                                  \
        KS_ASSERT(v, "Null arguments");                                                               \
        float l2 = KS_CONCAT2(type, _length_sq)(v);                                                   \
        return KS_FEQ(l2, 1.0f, KS_FEPS_MATH);                                                        \
    }                                                                                                 \
                                                                                                      \
    static inline bool KS_CONCAT2(type, _iscanonic)(const type* v) {                                  \
        KS_ASSERT(v, "Null arguments");                                                               \
        bool onefound = false;                                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                                \
            if (KS_FEQ(v->data[i], 1.0f, KS_FEPS_MATH)) {                                             \
                if (onefound) {                                                                       \
                    return false;                                                                     \
                }                                                                                     \
                onefound = true;                                                                      \
            } else if (KS_FNEQ(v->data[i], 0.0f, KS_FEPS_MATH)) {                                     \
                return false;                                                                         \
            }                                                                                         \
        }                                                                                             \
                                                                                                      \
        return onefound;                                                                              \
    }

KS_VEC_FUNCTIONS(ks_vec2, 2, (float x, float y), (x, y));
KS_VEC_FUNCTIONS(ks_vec3, 3, (float x, float y, float z), (x, y, z));
KS_VEC_FUNCTIONS(ks_vec4, 4, (float x, float y, float z, float w), (x, y, z, w));

static inline void ks_vec3_cross(ks_vec3* out, const ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(out && v1 && v2, "Null arguments");
    float x1 = v1->x, y1 = v1->y, z1 = v1->z;
    float x2 = v2->x, y2 = v2->y, z2 = v2->z;
    out->x = y1 * z2 - z1 * y2;
    out->y = z1 * x2 - x1 * z2;
    out->z = x1 * y2 - y1 * x2;
}

static inline void ks_vec3_crossi(ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    float x1 = v1->x, y1 = v1->y, z1 = v1->z;
    float x2 = v2->x, y2 = v2->y, z2 = v2->z;
    v1->x = y1 * z2 - z1 * y2;
    v1->y = z1 * x2 - x1 * z2;
    v1->z = x1 * y2 - y1 * x2;
}

/* Matrices */

#define KS_MAT_FUNCTIONS(type, vtype, n)                                                       \
    static inline void KS_CONCAT2(type, _zeroes)(type * m) {                                   \
        KS_ASSERT(m, "Null arguments");                                                        \
        memset(m, 0, sizeof(type));                                                            \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _id)(type * m) {                                       \
        KS_CONCAT2(type, _zeroes)(m);                                                          \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                         \
            m->data[i * n + i] = 1.0f;                                                         \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _fill)(type * m, float num) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m->data[i] = num;                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _diag)(type * m, float num) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_SIMD_HINT for (int32_t i = 0; i < n; ++i) {                                         \
            m->data[i * n + i] = num;                                                          \
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
        KS_ASSERT(KS_FNZERO(num, KS_FEPS_MATH), "Division by zero");                           \
        float inv = 1.0f / num;                                                                \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            out->data[i] = m->data[i] * inv;                                                   \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _sdivi)(type * m, float num) {                         \
        KS_ASSERT(m, "Null arguments");                                                        \
        KS_ASSERT(KS_FNZERO(num, KS_FEPS_MATH), "Division by zero");                           \
        float inv = 1.0f / num;                                                                \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            m->data[i] *= inv;                                                                 \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _transp)(type * out, const type* m) {                  \
        KS_ASSERT(out && m, "Null arguments");                                                 \
        KS_ASSERT(out != m, "Destination and source must be different");                       \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < n; ++r) {                                                  \
                out->data[c * n + r] = m->data[r * n + c];                                     \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _transpi)(type * m) {                                  \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = c + 1; r < n; ++r) {                                              \
                float tmp = m->data[c * n + r];                                                \
                m->data[c * n + r] = m->data[r * n + c];                                       \
                m->data[r * n + c] = tmp;                                                      \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    static inline void KS_CONCAT2(type, _mul)(type * out, const type* m1, const type* m2) {    \
        KS_ASSERT(out && m1 && m2, "Null arguments");                                          \
        KS_ASSERT(out != m1 && out != m2, "Destination and source must be different ");        \
        KS_CONCAT2(type, _zeroes)(out);                                                        \
                                                                                               \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t k = 0; k < n; ++k) {                                                  \
                float m2_val = m2->data[c * n + k];                                            \
                for (int32_t r = 0; r < n; ++r) {                                              \
                    out->data[c * n + r] += m1->data[k * n + r] * m2_val;                      \
                }                                                                              \
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
        KS_ASSERT(out != v, "Destination and source must be different ");                      \
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
    static inline type KS_CONCAT2(type, _model)(const type* T, const type* R, const type* S) { \
        KS_ASSERT_NONNULL_ARGS(T && R && S);                                                   \
        type m;                                                                                \
        KS_CONCAT2(type, _mul)(&m, T, R);                                                      \
        KS_CONCAT2(type, _muli)(&m, S);                                                        \
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
    static inline bool KS_CONCAT2(type, _equals)(const type* m1, const type* m2) {             \
        KS_ASSERT(m1 && m2, "Null arguments");                                                 \
        KS_SIMD_HINT for (int32_t i = 0; i < n * n; ++i) {                                     \
            if (KS_FNEQ(m1->data[i], m2->data[i], KS_FEPS_MATH)) {                             \
                return false;                                                                  \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    static inline bool KS_CONCAT2(type, _isidentity)(const type* m) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < n; ++r) {                                                  \
                float val = m->data[c * n + r];                                                \
                if (c == r) {                                                                  \
                    if (KS_FNEQ(val, 1.0f, KS_FEPS_MATH)) {                                    \
                        return false;                                                          \
                    }                                                                          \
                } else {                                                                       \
                    if (KS_FNEQ(val, 0.0f, KS_FEPS_MATH)) {                                    \
                        return false;                                                          \
                    }                                                                          \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    static inline bool KS_CONCAT2(type, _issymmetric)(const type* m) {                         \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = c + 1; r < n; ++r) {                                              \
                if (KS_FNEQ(m->data[c * n + r], m->data[r * n + c], KS_FEPS_MATH)) {           \
                    return false;                                                              \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    static inline bool KS_CONCAT2(type, _isdiagonal)(const type* m) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < n; ++r) {                                                  \
                if (c != r && KS_FNZERO(m->data[c * n + r], KS_FEPS_MATH)) {                   \
                    return false;                                                              \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    static inline bool KS_CONCAT2(type, _isuppertri)(const type* m) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 0; c < n; ++c) {                                                      \
            for (int32_t r = c + 1; r < n; ++r) {                                              \
                if (KS_FNZERO(m->data[c * n + r], KS_FEPS_MATH)) {                             \
                    return false;                                                              \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    static inline bool KS_CONCAT2(type, _islowertri)(const type* m) {                          \
        KS_ASSERT(m, "Null arguments");                                                        \
        for (int32_t c = 1; c < n; ++c) {                                                      \
            for (int32_t r = 0; r < c; ++r) {                                                  \
                if (KS_FNZERO(m->data[c * n + r], KS_FEPS_MATH)) {                             \
                    return false;                                                              \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
        return true;                                                                           \
    }

KS_MAT_FUNCTIONS(ks_mat2, ks_vec2, 2);
KS_MAT_FUNCTIONS(ks_mat3, ks_vec3, 3);
KS_MAT_FUNCTIONS(ks_mat4, ks_vec4, 4);

static inline float ks_mat2_det(const ks_mat2* m) {
    KS_ASSERT(m, "Null arguments");
    const float* d = m->data;
    return d[0] * d[3] - d[1] * d[2];
}

static inline void ks_mat2_inv(ks_mat2* out, const ks_mat2* m) {
    KS_ASSERT(out && m, "Null arguments");
    KS_ASSERT(out != m, "Destination and source must be different");
    float det = ks_mat2_det(m);
    KS_ASSERT(KS_FNZERO(det, KS_FEPS_MATH), "Matrix is not invertible");
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

static inline void ks_mat2_mkrot(ks_mat2* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);

    float c = cosf(angle), s = sinf(angle);
    float* d = out->data;
    d[0] = c;
    d[1] = s;
    d[2] = -s;
    d[3] = c;
}

static inline void ks_mat2_rot(ks_mat2* m, float angle) {
    KS_ASSERT(m, "Null arguments");
    float c = cosf(angle), s = sinf(angle);
    float* d = m->data;
    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3];
    d[0] = d0 * c - d1 * s;
    d[1] = d1 * c - d0 * s;
    d[2] = d2 * c + d3 * s;
    d[3] = d3 * c + d2 * s;
}

static inline void ks_mat2_mkscale(ks_mat2* out, const ks_vec2* v) {
    KS_ASSERT_NONNULL_ARGS(out && v);
    float* d = out->data;
    ks_mat2_zeroes(out);
    d[0] = v->x;
    d[3] = v->y;
}

static inline void ks_mat2_scale(ks_mat2* m, const ks_vec2* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    float x = v->x, y = v->y;
    d[0] *= x;
    d[1] *= y;
    d[2] *= x;
    d[3] *= y;
}

static inline float ks_mat3_det(const ks_mat3* m) {
    KS_ASSERT(m, "Null arguments");
    const float* d = m->data;
    float d3 = d[3], d4 = d[4], d5 = d[5], d7 = d[7], d8 = d[8];
    return d[0] * (d4 * d8 - d5 * d7) - d[1] * (d3 * d8 - d5 * d[6]) + d[2] * (d3 * d7 - d4 * d[6]);
}

static inline void ks_mat3_inv(ks_mat3* out, const ks_mat3* m) {
    KS_ASSERT(out && m, "Null arguments");
    KS_ASSERT(out != m, "Destination and source must be different");
    float det = ks_mat3_det(m);
    KS_ASSERT(KS_FNZERO(det, KS_FEPS_MATH), "Matrix is not invertible");
    float inv = 1.0f / det;
    const float* d = m->data;
    float* o = out->data;

    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3], d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7], d8 = d[8];

    o[0] = (d4 * d8 - d5 * d7) * inv;
    o[1] = (d2 * d7 - d1 * d8) * inv;
    o[2] = (d1 * d5 - d2 * d4) * inv;
    o[3] = (d5 * d6 - d3 * d8) * inv;
    o[4] = (d0 * d8 - d2 * d6) * inv;
    o[5] = (d2 * d3 - d0 * d5) * inv;
    o[6] = (d3 * d7 - d4 * d6) * inv;
    o[7] = (d1 * d6 - d0 * d7) * inv;
    o[8] = (d0 * d4 - d1 * d3) * inv;
}

static inline void ks_mat3_invi(ks_mat3* m) {
    ks_mat3 tmp;
    ks_mat3_inv(&tmp, m);
    *m = tmp;
}

static inline void ks_mat3_mktransl(ks_mat3* out, const ks_vec2* v) {
    KS_ASSERT_NONNULL_ARGS(out && v);
    float* d = out->data;
    ks_mat3_id(out);
    d[6] = v->x;
    d[7] = v->y;
}

static inline void ks_mat3_transl(ks_mat3* m, const ks_vec2* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    float x = v->x, y = v->y;
    d[6] += d[0] * x + d[3] * y;
    d[7] += d[1] * x + d[4] * y;
    d[8] += d[2] * x + d[5] * y;
}

static inline void ks_mat3_mkrotx(ks_mat3* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    float c = cosf(angle), s = sinf(angle);
    float* d = out->data;
    ks_mat3_zeroes(out);

    d[0] = 1.0f;
    d[4] = c;
    d[5] = s;
    d[7] = -s;
    d[8] = c;
}

static inline void ks_mat3_mkroty(ks_mat3* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    float c = cosf(angle), s = sinf(angle);
    float* d = out->data;
    ks_mat3_zeroes(out);

    d[0] = c;
    d[2] = -s;
    d[4] = 1.0f;
    d[6] = s;
    d[8] = c;
}

static inline void ks_mat3_mkrotz(ks_mat3* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    float c = cosf(angle), s = sinf(angle);
    float* d = out->data;
    ks_mat3_zeroes(out);

    d[0] = c;
    d[1] = s;
    d[3] = -s;
    d[4] = c;
    d[8] = 1.0f;
}

static inline void ks_mat3_mkrot(ks_mat3* out, ks_vec3 axis, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    KS_ASSERT(ks_vec3_isunitary(&axis), "Axis vector must be unitary");

    float c = cosf(angle), s = sinf(angle);
    float t = 1.0f - c;

    float x = axis.x, y = axis.y, z = axis.z;
    float xx = x * x, xy = x * y, xz = x * z;
    float yy = y * y, yz = y * z;
    float zz = z * z;

    float xyt = xy * t;
    float xzt = xz * t;
    float yzt = yz * t;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    float* d = out->data;

    d[0] = xx * t + c;
    d[1] = xyt + zs;
    d[2] = xzt - ys;
    d[3] = xyt - zs;
    d[4] = yy * t + c;
    d[5] = yzt + xs;
    d[6] = xzt + ys;
    d[7] = yzt - xs;
    d[8] = zz * t + c;
}

static inline void ks_mat3_rotx(ks_mat3* m, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    float* d = m->data;

    float c = cosf(angle), s = sinf(angle);

    float d3 = d[3], d4 = d[4], d5 = d[5];
    float d6 = d[6], d7 = d[7], d8 = d[8];

    d[3] = d3 * c + d6 * s;
    d[4] = d4 * c + d7 * s;
    d[5] = d5 * c + d8 * s;
    d[6] = d6 * c - d3 * s;
    d[7] = d7 * c - d4 * s;
    d[8] = d8 * c - d5 * s;
}

static inline void ks_mat3_roty(ks_mat3* m, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    float* d = m->data;

    float c = cosf(angle), s = sinf(angle);

    float d0 = d[0], d1 = d[1], d2 = d[2];
    float d6 = d[6], d7 = d[7], d8 = d[8];

    d[0] = d0 * c - d6 * s;
    d[1] = d1 * c - d7 * s;
    d[2] = d2 * c - d8 * s;
    d[6] = d6 * c + d0 * s;
    d[7] = d7 * c + d1 * s;
    d[8] = d8 * c + d2 * s;
}

static inline void ks_mat3_rotz(ks_mat3* m, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    float* d = m->data;

    float c = cosf(angle), s = sinf(angle);

    float d0 = d[0], d1 = d[1], d2 = d[2];
    float d3 = d[3], d4 = d[4], d5 = d[5];

    d[0] = d0 * c + d3 * s;
    d[1] = d1 * c + d4 * s;
    d[2] = d2 * c + d5 * s;
    d[3] = d3 * c - d0 * s;
    d[4] = d4 * c - d1 * s;
    d[5] = d5 * c - d2 * s;
}

static inline void ks_mat3_rot(ks_mat3* m, ks_vec3 axis, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    KS_ASSERT(ks_vec3_isunitary(&axis), "Axis vector must be unitary");
    float* d = m->data;

    float c = cosf(angle), s = sinf(angle);
    float t = 1.0f - c;

    float x = axis.x, y = axis.y, z = axis.z;
    float xx = x * x, xy = x * y, xz = x * z;
    float yy = y * y, yz = y * z;
    float zz = z * z;

    float xyt = xy * t;
    float xzt = xz * t;
    float yzt = yz * t;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    float r0 = c + xx * t;
    float r1 = xyt + zs;
    float r2 = xzt - ys;
    float r3 = xyt - zs;
    float r4 = c + yy * t;
    float r5 = yzt + xs;
    float r6 = xzt + ys;
    float r7 = yzt - xs;
    float r8 = c + zz * t;

    float d0 = d[0], d1 = d[1], d2 = d[2];
    float d3 = d[3], d4 = d[4], d5 = d[5];
    float d6 = d[6], d7 = d[7], d8 = d[8];

    d[0] = d0 * r0 + d3 * r1 + d6 * r2;
    d[1] = d1 * r0 + d4 * r1 + d7 * r2;
    d[2] = d2 * r0 + d5 * r1 + d8 * r2;
    d[3] = d0 * r3 + d3 * r4 + d6 * r5;
    d[4] = d1 * r3 + d4 * r4 + d7 * r5;
    d[5] = d2 * r3 + d5 * r4 + d8 * r5;
    d[6] = d0 * r6 + d3 * r7 + d6 * r8;
    d[7] = d1 * r6 + d4 * r7 + d7 * r8;
    d[8] = d2 * r6 + d5 * r7 + d8 * r8;
}

static inline void ks_mat3_mkscale(ks_mat3* out, const ks_vec3* v) {
    KS_ASSERT_NONNULL_ARGS(out && v);
    float* d = out->data;
    ks_mat3_zeroes(out);
    d[0] = v->x;
    d[4] = v->y;
    d[8] = v->z;
}

static inline void ks_mat3_scale(ks_mat3* m, const ks_vec3* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    float x = v->x, y = v->y, z = v->z;
    d[0] *= x;
    d[1] *= x;
    d[2] *= x;
    d[3] *= y;
    d[4] *= y;
    d[5] *= y;
    d[6] *= z;
    d[7] *= z;
    d[8] *= z;
}

static inline void ks_mat3_lookat(ks_mat3* out, const ks_vec2* eye, const ks_vec2* target) {
    KS_ASSERT(out && eye && target, "Null arguments");

    ks_vec2 f;
    ks_vec2_sub(&f, target, eye);
    ks_vec2_normi(&f);

    ks_vec2 r = KS_VEC2(f.y, -f.x);

    ks_mat3_id(out);
    float* d = out->data;

    d[0] = r.x;
    d[1] = r.y;
    d[2] = 0.0f;
    d[3] = f.x;
    d[4] = f.y;
    d[5] = 0.0f;
    d[6] = -ks_vec2_dot(&r, eye);
    d[7] = -ks_vec2_dot(&f, eye);
    d[8] = 1.0f;
}

static inline void ks_mat3_ortho(ks_mat3* out, float left, float right, float bottom, float top) {
    KS_ASSERT(out && KS_FNEQ(right, left, KS_FEPS_MATH) && KS_FNEQ(top, bottom, KS_FEPS_MATH), "Invalid bounds");
    ks_mat3_id(out);
    float* d = out->data;
    float rl = right - left;
    float tb = top - bottom;
    d[0] = 2.0f / rl;
    d[4] = 2.0f / tb;
    d[6] = -(right + left) / rl;
    d[7] = -(top + bottom) / tb;
}

static inline float ks_mat4_det(const ks_mat4* m) {
    KS_ASSERT(m, "Null arguments");
    const float* d = m->data;

    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3], d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7], d8 = d[8], d9 = d[9],
          d10 = d[10], d11 = d[11], d12 = d[12], d13 = d[13], d14 = d[14], d15 = d[15];

    float s0 = d0 * d5 - d1 * d4;
    float s1 = d0 * d6 - d2 * d4;
    float s2 = d0 * d7 - d3 * d4;
    float s3 = d1 * d6 - d2 * d5;
    float s4 = d1 * d7 - d3 * d5;
    float s5 = d2 * d7 - d3 * d6;

    float c5 = d10 * d15 - d11 * d14;
    float c4 = d9 * d15 - d11 * d13;
    float c3 = d9 * d14 - d10 * d13;
    float c2 = d8 * d15 - d11 * d12;
    float c1 = d8 * d14 - d10 * d12;
    float c0 = d8 * d13 - d9 * d12;

    return (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
}

static inline void ks_mat4_inv(ks_mat4* out, const ks_mat4* m) {
    KS_ASSERT(out && m, "Null arguments");
    KS_ASSERT(out != m, "Destination and source must be different");

    float det = ks_mat4_det(m);
    KS_ASSERT(KS_FNZERO(det, KS_FEPS_MATH), "Matrix is not invertible");
    float inv = 1.0f / det;
    const float* d = m->data;

    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3], d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7], d8 = d[8], d9 = d[9],
          d10 = d[10], d11 = d[11], d12 = d[12], d13 = d[13], d14 = d[14], d15 = d[15];

    float c00 = d10 * d15 - d11 * d14, c01 = d9 * d15 - d11 * d13;
    float c02 = d9 * d14 - d10 * d13, c03 = d8 * d15 - d11 * d12;
    float c04 = d8 * d14 - d10 * d12, c05 = d8 * d13 - d9 * d12;
    float c12 = d6 * d11 - d7 * d10, c13 = d5 * d11 - d7 * d9;
    float c14 = d5 * d10 - d6 * d9, c15 = d4 * d11 - d7 * d8;
    float c16 = d4 * d10 - d6 * d8, c17 = d4 * d9 - d5 * d8;

    out->data[0] = (d5 * c00 - d6 * c01 + d7 * c02) * inv;
    out->data[1] = (-d1 * c00 + d2 * c01 - d3 * c02) * inv;
    out->data[2] = (d13 * c12 - d14 * c13 + d15 * c14) * inv;
    out->data[3] = (-d9 * c12 + d10 * c13 - d11 * c14) * inv;
    out->data[4] = (-d4 * c00 + d6 * c03 - d7 * c04) * inv;
    out->data[5] = (d0 * c00 - d2 * c03 + d3 * c04) * inv;
    out->data[6] = (-d12 * c12 + d14 * c15 - d15 * c16) * inv;
    out->data[7] = (d8 * c12 - d10 * c15 + d11 * c16) * inv;
    out->data[8] = (d4 * c01 - d5 * c03 + d7 * c05) * inv;
    out->data[9] = (-d0 * c01 + d1 * c03 - d3 * c05) * inv;
    out->data[10] = (d12 * c13 - d13 * c15 + d15 * c17) * inv;
    out->data[11] = (-d8 * c13 + d9 * c15 - d11 * c17) * inv;
    out->data[12] = (-d4 * c02 + d5 * c04 - d6 * c05) * inv;
    out->data[13] = (d0 * c02 - d1 * c04 + d2 * c05) * inv;
    out->data[14] = (-d12 * c14 + d13 * c16 - d14 * c17) * inv;
    out->data[15] = (d8 * c14 - d9 * c16 + d10 * c17) * inv;
}

static inline void ks_mat4_invi(ks_mat4* m) {
    ks_mat4 tmp;
    ks_mat4_inv(&tmp, m);
    *m = tmp;
}

static inline void ks_mat4_mktransl(ks_mat4* out, const ks_vec3* v) {
    KS_ASSERT_NONNULL_ARGS(out && v);
    float* d = out->data;
    ks_mat4_id(out);
    d[12] = v->x;
    d[13] = v->y;
    d[14] = v->z;
}

static inline void ks_mat4_transl(ks_mat4* m, const ks_vec3* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    float x = v->x, y = v->y, z = v->z;
    d[12] += d[0] * x + d[4] * y + d[8] * z;
    d[13] += d[1] * x + d[5] * y + d[9] * z;
    d[14] += d[2] * x + d[6] * y + d[10] * z;
    d[15] += d[3] * x + d[7] * y + d[11] * z;
}

static inline void ks_mat4_mkrotx(ks_mat4* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    float c = cosf(angle), s = sinf(angle);
    ks_mat4_zeroes(out);

    float* d = out->data;
    d[0] = 1.0f;
    d[5] = c;
    d[6] = s;
    d[9] = -s;
    d[10] = c;
    d[15] = 1.0f;
}

static inline void ks_mat4_mkroty(ks_mat4* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    float c = cosf(angle), s = sinf(angle);
    ks_mat4_zeroes(out);

    float* d = out->data;
    d[0] = c;
    d[2] = -s;
    d[5] = 1.0f;
    d[8] = s;
    d[10] = c;
    d[15] = 1.0f;
}

static inline void ks_mat4_mkrotz(ks_mat4* out, float angle) {
    KS_ASSERT_NONNULL_ARGS(out);
    float c = cosf(angle), s = sinf(angle);
    ks_mat4_zeroes(out);

    float* d = out->data;
    d[0] = c;
    d[1] = s;
    d[4] = -s;
    d[5] = c;
    d[10] = 1.0f;
    d[15] = 1.0f;
}

static inline void ks_mat4_mkrot(ks_mat4* out, const ks_vec3* axis, float angle) {
    KS_ASSERT_NONNULL_ARGS(out && axis);
    KS_ASSERT(ks_vec3_isunitary(axis), "Axis vector must be unitary");

    float c = cosf(angle), s = sinf(angle);
    float t = 1.0f - c;

    float x = axis->x, y = axis->y, z = axis->z;
    float xx = x * x, xy = x * y, xz = x * z;
    float yy = y * y, yz = y * z;
    float zz = z * z;

    float xyt = xy * t;
    float xzt = xz * t;
    float yzt = yz * t;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    ks_mat4_id(out);
    float* d = out->data;

    d[0] = xx * t + c;
    d[1] = xyt + zs;
    d[2] = xzt - ys;
    d[4] = xyt - zs;
    d[5] = yy * t + c;
    d[6] = yzt + xs;
    d[8] = xzt + ys;
    d[9] = yzt - xs;
    d[10] = zz * t + c;
}

static inline void ks_mat4_rotx(ks_mat4* m, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    float* d = m->data;
    float c = cosf(angle), s = sinf(angle);

    float d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7], d8 = d[8], d9 = d[9], d10 = d[10], d11 = d[11];

    d[4] = d4 * c + d8 * s;
    d[5] = d5 * c + d9 * s;
    d[6] = d6 * c + d10 * s;
    d[7] = d7 * c + d11 * s;
    d[8] = d8 * c - d4 * s;
    d[9] = d9 * c - d5 * s;
    d[10] = d10 * c - d6 * s;
    d[11] = d11 * c - d7 * s;
}

static inline void ks_mat4_roty(ks_mat4* m, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    float* d = m->data;
    float c = cosf(angle), s = sinf(angle);

    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3], d8 = d[8], d9 = d[9], d10 = d[10], d11 = d[11];

    d[0] = d0 * c - d8 * s;
    d[1] = d1 * c - d9 * s;
    d[2] = d2 * c - d10 * s;
    d[3] = d3 * c - d11 * s;
    d[8] = d8 * c + d0 * s;
    d[9] = d9 * c + d1 * s;
    d[10] = d10 * c + d2 * s;
    d[11] = d11 * c + d3 * s;
}

static inline void ks_mat4_rotz(ks_mat4* m, float angle) {
    KS_ASSERT_NONNULL_ARGS(m);
    float* d = m->data;
    float c = cosf(angle), s = sinf(angle);

    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3], d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7];

    d[0] = d0 * c + d4 * s;
    d[1] = d1 * c + d5 * s;
    d[2] = d2 * c + d6 * s;
    d[3] = d3 * c + d7 * s;
    d[4] = d4 * c - d0 * s;
    d[5] = d5 * c - d1 * s;
    d[6] = d6 * c - d2 * s;
    d[7] = d7 * c - d3 * s;
}

static inline void ks_mat4_rot(ks_mat4* m, const ks_vec3* axis, float angle) {
    KS_ASSERT_NONNULL_ARGS(m && axis);
    KS_ASSERT(ks_vec3_isunitary(axis), "Axis vector must be unitary");
    float* d = m->data;

    float c = cosf(angle), s = sinf(angle);
    float t = 1.0f - c;

    float x = axis->x, y = axis->y, z = axis->z;
    float xx = x * x, xy = x * y, xz = x * z;
    float yy = y * y, yz = y * z;
    float zz = z * z;

    float xyt = xy * t;
    float xzt = xz * t;
    float yzt = yz * t;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    float r0 = c + xx * t;
    float r1 = xyt + zs;
    float r2 = xzt - ys;
    float r3 = xyt - zs;
    float r4 = c + yy * t;
    float r5 = yzt + xs;
    float r6 = xzt + ys;
    float r7 = yzt - xs;
    float r8 = c + zz * t;

    float d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3], d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7], d8 = d[8], d9 = d[9],
          d10 = d[10], d11 = d[11];

    d[0] = d0 * r0 + d4 * r1 + d8 * r2;
    d[1] = d1 * r0 + d5 * r1 + d9 * r2;
    d[2] = d2 * r0 + d6 * r1 + d10 * r2;
    d[3] = d3 * r0 + d7 * r1 + d11 * r2;
    d[4] = d0 * r3 + d4 * r4 + d8 * r5;
    d[5] = d1 * r3 + d5 * r4 + d9 * r5;
    d[6] = d2 * r3 + d6 * r4 + d10 * r5;
    d[7] = d3 * r3 + d7 * r4 + d11 * r5;
    d[8] = d0 * r6 + d4 * r7 + d8 * r8;
    d[9] = d1 * r6 + d5 * r7 + d9 * r8;
    d[10] = d2 * r6 + d6 * r7 + d10 * r8;
    d[11] = d3 * r6 + d7 * r7 + d11 * r8;
}

static inline void ks_mat4_mkscale(ks_mat4* out, const ks_vec3* v) {
    KS_ASSERT_NONNULL_ARGS(out && v);
    float* d = out->data;
    ks_mat4_zeroes(out);
    d[0] = v->x;
    d[5] = v->y;
    d[10] = v->z;
    d[15] = 1.0f;
}

static inline void ks_mat4_scale(ks_mat4* m, const ks_vec3* v) {
    KS_ASSERT(m && v, "Null arguments");
    float* d = m->data;
    float x = v->x, y = v->y, z = v->z;
    d[0] *= x;
    d[1] *= x;
    d[2] *= x;
    d[3] *= x;
    d[5] *= y;
    d[4] *= y;
    d[6] *= y;
    d[7] *= y;
    d[8] *= z;
    d[9] *= z;
    d[10] *= z;
    d[11] *= z;
}

static inline void ks_mat4_lookat(ks_mat4* out, const ks_vec3* eye, const ks_vec3* center, const ks_vec3* up) {
    KS_ASSERT(out && eye && center && up, "Null arguments");
    ks_vec3 f, s, u;

    ks_vec3_sub(&f, center, eye);
    ks_vec3_normi(&f);

    ks_vec3_cross(&s, &f, up);
    ks_vec3_normi(&s);

    ks_vec3_cross(&u, &s, &f);

    ks_mat4_id(out);
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
    KS_ASSERT(out && KS_FNZERO(aspect, KS_FEPS_MATH) && KS_FNEQ(farz, nearz, KS_FEPS_MATH), "Invalid bounds");
    float t = tanf(fovy_rad / 2.0f);

    ks_mat4_zeroes(out);
    float* d = out->data;
    float fn = farz - nearz;
    d[0] = 1.0f / (aspect * t);
    d[5] = 1.0f / (t);
    d[10] = -(farz + nearz) / fn;
    d[11] = -1.0f;
    d[14] = -(2.0f * farz * nearz) / fn;
}

static inline void ks_mat4_ortho(ks_mat4* out, float left, float right, float bottom, float top, float nearz,
                                 float farz) {
    KS_ASSERT(out && KS_FNEQ(left, right, KS_FEPS_MATH) && KS_FNEQ(bottom, top, KS_FEPS_MATH) &&
                  KS_FNEQ(farz, nearz, KS_FEPS_MATH),
              "Invalid bounds");
    ks_mat4_id(out);
    float* d = out->data;
    float rl = right - left;
    float tb = top - bottom;
    float fn = farz - nearz;
    d[0] = 2.0f / rl;
    d[5] = 2.0f / tb;
    d[10] = -2.0f / fn;
    d[12] = -(right + left) / rl;
    d[13] = -(top + bottom) / tb;
    d[14] = -(farz + nearz) / fn;
}

/* Tensors */

#define KS_TENSOR_DECLARE_UNARY(name, ...)                                                        \
    KS_API void KS_CONCAT2(ks_tensor_, name)(ks_tensor * out, const ks_tensor* a, ##__VA_ARGS__); \
    KS_API void KS_CONCAT3(ks_tensor_, name, i)(ks_tensor * a, ##__VA_ARGS__)

#define KS_TENSOR_DECLARE_BINARY(name, ...)                                                                           \
    KS_API void KS_CONCAT2(ks_tensor_, name)(ks_tensor * out, const ks_tensor* a, const ks_tensor* b, ##__VA_ARGS__); \
    KS_API void KS_CONCAT3(ks_tensor_, name, i)(ks_tensor * a, const ks_tensor* b, ##__VA_ARGS__)

#define KS_TENSOR_DECLARE_REDUCE(name, ...)                                              \
    KS_API float KS_CONCAT2(ks_tensor_reduce_, name)(const ks_tensor* a, ##__VA_ARGS__); \
    KS_API void KS_CONCAT3(ks_tensor_reduce_, name, i)(ks_tensor * a, ##__VA_ARGS__)

#define KS_TENSOR_DECLARE_REDUCE_AXIS(name, ...)                                                            \
    KS_API void KS_CONCAT2(ks_tensor_reduce_axis_, name)(ks_tensor * out, const ks_tensor* a, int32_t axis, \
                                                         ##__VA_ARGS__);                                    \
    KS_API void KS_CONCAT3(ks_tensor_reduce_axis_, name, i)(ks_tensor * a, int32_t axis, ##__VA_ARGS__)

KS_API ks_status ks_tensor_init(ks_tensor* t, const int32_t ndims, const ks_tensor_int* shape, const ks_allocator* a);
KS_API void ks_tensor_fini(ks_tensor* t);

KS_API void ks_tensor_copy(ks_tensor* dst, const ks_tensor* src);
KS_API ks_status ks_tensor_clone(ks_tensor* dst, const ks_tensor* src, const ks_allocator* a);
KS_API void ks_tensor_view(ks_tensor* dst, const ks_tensor* src);

KS_API ks_tensor_int ks_tensor_count(const ks_tensor* t);

KS_API bool ks_tensor_is_contiguous(const ks_tensor* t);
KS_API bool ks_tensor_same_shape(const ks_tensor* a, const ks_tensor* b);

KS_API ks_status ks_tensor_contiguous(ks_tensor* dst, const ks_tensor* src, const ks_allocator* a);
KS_API ks_status ks_tensor_reshape(ks_tensor* t, const int32_t new_ndims, const ks_tensor_int* new_shape);
KS_API ks_status ks_tensor_transpose(ks_tensor* t, const int32_t dim1, const int32_t dim2);

KS_API void ks_tensor_zeros(ks_tensor* t);
KS_API void ks_tensor_fill(ks_tensor* t, const float val);
KS_API void ks_tensor_rand_uniform(ks_tensor* t, const float min, const float max);
KS_API void ks_tensor_rand_normal(ks_tensor* t, const float mean, const float stddev);

KS_TENSOR_DECLARE_UNARY(neg);
KS_TENSOR_DECLARE_UNARY(clamp, float min, float max);
KS_TENSOR_DECLARE_UNARY(pow, float exponent);
KS_TENSOR_DECLARE_UNARY(sqrt);
KS_TENSOR_DECLARE_UNARY(exp);
KS_TENSOR_DECLARE_UNARY(log);
KS_TENSOR_DECLARE_UNARY(abs);
KS_TENSOR_DECLARE_UNARY(sigmoid);
KS_TENSOR_DECLARE_UNARY(tanh);
KS_TENSOR_DECLARE_UNARY(relu);
KS_TENSOR_DECLARE_UNARY(leaky_relu, float alpha);
KS_TENSOR_DECLARE_UNARY(softplus);
KS_TENSOR_DECLARE_UNARY(elu, float alpha);
KS_TENSOR_DECLARE_UNARY(selu, float scale, float alpha);

KS_TENSOR_DECLARE_BINARY(add);
KS_TENSOR_DECLARE_BINARY(sub);
KS_TENSOR_DECLARE_BINARY(mul);
KS_TENSOR_DECLARE_BINARY(div);

KS_TENSOR_DECLARE_REDUCE(sum);
KS_TENSOR_DECLARE_REDUCE(max);
KS_TENSOR_DECLARE_REDUCE(min);

KS_TENSOR_DECLARE_REDUCE_AXIS(sum);
KS_TENSOR_DECLARE_REDUCE_AXIS(max);
KS_TENSOR_DECLARE_REDUCE_AXIS(min);
KS_TENSOR_DECLARE_REDUCE_AXIS(argmax);
KS_TENSOR_DECLARE_REDUCE_AXIS(argmin);

KS_API void ks_tensor_matmul(ks_tensor* out, const ks_tensor* a, const ks_tensor* b);

// Euler

static inline void ks_euler_zeroes(ks_euler* out, ks_euler_order o) {
    KS_ASSERT(out, "Null arguments");
    memset(out, 0, sizeof(float) * 3);
    out->order = o;
}

static inline void ks_euler_init(ks_euler* out, float x, float y, float z, ks_euler_order o) {
    *out = KS_EULER(x, y, z, o);
}

// Quaternions

static inline void ks_quat_zeroes(ks_quat* q) {
    KS_ASSERT(q, "Null arguments");
    memset(q, 0, sizeof(ks_quat));
}

static inline void ks_quat_id(ks_quat* q) {
    ks_quat_zeroes(q);
    q->w = 1.0f;
}

static inline void ks_quat_add(ks_quat* out, const ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(out && q1 && q2);

    out->w = q1->w + q2->w;
    out->x = q1->x + q2->x;
    out->y = q1->y + q2->y;
    out->z = q1->z + q2->z;
}

static inline void ks_quat_addi(ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(q1 && q2);

    q1->w += q2->w;
    q1->x += q2->x;
    q1->y += q2->y;
    q1->z += q2->z;
}

static inline void ks_quat_sub(ks_quat* out, const ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(out && q1 && q2);

    out->w = q1->w - q2->w;
    out->x = q1->x - q2->x;
    out->y = q1->y - q2->y;
    out->z = q1->z - q2->z;
}

static inline void ks_quat_subi(ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(q1 && q2);

    q1->w -= q2->w;
    q1->x -= q2->x;
    q1->y -= q2->y;
    q1->z -= q2->z;
}

static inline void ks_quat_smul(ks_quat* out, const ks_quat* q, float val) {
    KS_ASSERT_NONNULL_ARGS(out && q);

    out->w = q->w * val;
    out->x = q->x * val;
    out->y = q->y * val;
    out->z = q->z * val;
}

static inline void ks_quat_smuli(ks_quat* q, float val) {
    KS_ASSERT_NONNULL_ARGS(q);

    q->w *= val;
    q->x *= val;
    q->y *= val;
    q->z *= val;
}

static inline void ks_quat_sdiv(ks_quat* out, const ks_quat* q, float val) {
    KS_ASSERT_NONNULL_ARGS(out && q);
    KS_ASSERT(KS_FNZERO(val, KS_FEPS_MATH), "Division by zero");

    float inv = 1.0f / val;

    out->w = q->w * inv;
    out->x = q->x * inv;
    out->y = q->y * inv;
    out->z = q->z * inv;
}

static inline void ks_quat_sdivi(ks_quat* q, float val) {
    KS_ASSERT_NONNULL_ARGS(q);
    KS_ASSERT(KS_FNZERO(val, KS_FEPS_MATH), "Division by zero");

    float inv = 1.0f / val;

    q->w *= inv;
    q->x *= inv;
    q->y *= inv;
    q->z *= inv;
}

static inline void ks_quat_mul(ks_quat* out, const ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(out && q1 && q2);

    float w1 = q1->w, x1 = q1->x, y1 = q1->y, z1 = q1->z;  // q1
    float w2 = q2->w, x2 = q2->x, y2 = q2->y, z2 = q2->z;  // q2

    out->w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    out->x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    out->y = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
    out->z = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;
}

static inline void ks_quat_muli(ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(q1 && q2);

    float w1 = q1->w, x1 = q1->x, y1 = q1->y, z1 = q1->z;  // q1
    float w2 = q2->w, x2 = q2->x, y2 = q2->y, z2 = q2->z;  // q2

    q1->w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    q1->x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    q1->y = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
    q1->z = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;
}

static inline void ks_quat_mulv(ks_vec3* out, const ks_quat* q, const ks_vec3* v) {
    KS_ASSERT_NONNULL_ARGS(out && q && v);

    float w = q->w, x = q->x, y = q->y, z = q->z;  // q
    float vx = v->x, vy = v->y, vz = v->z;         // v

    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx2 = x * x2, yy2 = y * y2, zz2 = z * z2;
    float wx2 = w * x2, wy2 = w * y2, wz2 = w * z2;
    float xy2 = x * y2;
    float xz2 = x * z2;
    float yz2 = y * z2;

    out->x = ((vx * ((1.0f - yy2) - zz2)) + (vy * (xy2 - wz2))) + (vz * (xz2 + wy2));
    out->y = ((vx * (xy2 + wz2)) + (vy * ((1.0f - xx2) - zz2))) + (vz * (yz2 - wx2));
    out->z = ((vx * (xz2 - wy2)) + (vy * (yz2 + wx2))) + (vz * ((1.0f - xx2) - yy2));
}

static inline void ks_quat_mulvi(const ks_quat* q, ks_vec3* v) {
    KS_ASSERT_NONNULL_ARGS(q && v);

    float w = q->w, x = q->x, y = q->y, z = q->z;  // q
    float vx = v->x, vy = v->y, vz = v->z;         // v

    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx2 = x * x2, yy2 = y * y2, zz2 = z * z2;
    float wx2 = w * x2, wy2 = w * y2, wz2 = w * z2;
    float xy2 = x * y2;
    float xz2 = x * z2;
    float yz2 = y * z2;

    v->x = ((vx * ((1.0f - yy2) - zz2)) + (vy * (xy2 - wz2))) + (vz * (xz2 + wy2));
    v->y = ((vx * (xy2 + wz2)) + (vy * ((1.0f - xx2) - zz2))) + (vz * (yz2 - wx2));
    v->z = ((vx * (xz2 - wy2)) + (vy * (yz2 + wx2))) + (vz * ((1.0f - xx2) - yy2));
}

static inline void ks_quat_conj(ks_quat* out, const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(out && q);

    out->w = q->w;
    out->x = -q->x;
    out->y = -q->y;
    out->z = -q->z;
}

static inline void ks_quat_conji(ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(q);

    q->x = -q->x;
    q->y = -q->y;
    q->z = -q->z;
}

static inline float ks_quat_dot(const ks_quat* q1, const ks_quat* q2) {
    KS_ASSERT_NONNULL_ARGS(q1 && q2);
    return q1->w * q2->w + q1->x * q2->x + q1->y * q2->y + q1->z * q2->z;
}

static inline float ks_quat_length_sq(const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(q);
    return q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
}

static inline float ks_quat_length(const ks_quat* q) {
    return sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
}

static inline void ks_quat_inv(ks_quat* out, const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(out && q);

    float l2 = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    KS_ASSERT(KS_FNZERO(l2, KS_FEPS_MATH), "Division by zero");

    float inv = 1.0f / l2;

    out->w = q->w * inv;
    out->x = -q->x * inv;
    out->y = -q->y * inv;
    out->z = -q->z * inv;
}

static inline void ks_quat_invi(ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(q);

    float l2 = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    KS_ASSERT(KS_FNZERO(l2, KS_FEPS_MATH), "Division by zero");

    float inv = 1.0f / l2;

    q->w *= inv;
    q->x *= -inv;
    q->y *= -inv;
    q->z *= -inv;
}

static inline void ks_quat_norm(ks_quat* out, const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(out && q);

    float l2 = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    KS_ASSERT(KS_FNZERO(l2, KS_FEPS_MATH), "Division by zero");

    if (KS_FEQ(l2, 1.0f, KS_FEPS_MATH)) {
        *out = *q;
        return;
    }

    float l = sqrtf(l2);
    float inv = 1.0f / l;

    out->w = q->w * inv;
    out->x = q->x * inv;
    out->y = q->y * inv;
    out->z = q->z * inv;
}

static inline void ks_quat_normi(ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(q);

    float l2 = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    KS_ASSERT(KS_FNZERO(l2, KS_FEPS_MATH), "Division by zero");

    if (KS_FEQ(l2, 1.0f, KS_FEPS_MATH)) {
        return;
    }

    float l = sqrtf(l2);
    float inv = 1.0f / l;

    q->w *= inv;
    q->x *= inv;
    q->y *= inv;
    q->z *= inv;
}

static inline ks_quat ks_quat_from_axis_angle(const ks_vec3* axis, float radians) {
    float r = radians * 0.5f;
    float sr = sinf(r);
    return KS_QUAT(cosf(r), sr * axis->x, sr * axis->y, sr * axis->z);
}

static inline ks_quat ks_euler_to_quat(const ks_euler* e) {
    KS_ASSERT_NONNULL_ARGS(e);

    ks_vec3 ax = KS_VEC3(1.0f, 0.0f, 0.0f);
    ks_vec3 ay = KS_VEC3(0.0f, 1.0f, 0.0f);
    ks_vec3 az = KS_VEC3(0.0f, 0.0f, 1.0f);

    ks_quat qx = ks_quat_from_axis_angle(&ax, e->x);
    ks_quat qy = ks_quat_from_axis_angle(&ay, e->y);
    ks_quat qz = ks_quat_from_axis_angle(&az, e->z);

    ks_quat tmp, out;

    switch (e->order) {
        case KS_EULER_XYZ:
            ks_quat_mul(&tmp, &qx, &qy);
            ks_quat_mul(&out, &tmp, &qz);
            return out;
        case KS_EULER_XZY:
            ks_quat_mul(&tmp, &qx, &qz);
            ks_quat_mul(&out, &tmp, &qy);
            return out;
        case KS_EULER_YXZ:
            ks_quat_mul(&tmp, &qy, &qx);
            ks_quat_mul(&out, &tmp, &qz);
            return out;
        case KS_EULER_YZX:
            ks_quat_mul(&tmp, &qy, &qz);
            ks_quat_mul(&out, &tmp, &qx);
            return out;
        case KS_EULER_ZXY:
            ks_quat_mul(&tmp, &qz, &qx);
            ks_quat_mul(&out, &tmp, &qy);
            return out;
        case KS_EULER_ZYX:
            ks_quat_mul(&tmp, &qz, &qy);
            ks_quat_mul(&out, &tmp, &qx);
            return out;
        default:
            return KS_QUAT(1.0f, 0.0f, 0.0f, 0.0f);
    }
}

static inline void ks_euler_to_mat4(ks_mat4* out, const ks_euler* e) {
    KS_ASSERT_NONNULL_ARGS(out && e);
    ks_quat q = ks_euler_to_quat(e);

    float x = q.x, y = q.y, z = q.z, w = q.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    float* d = out->data;
    d[0] = 1.0f - (yy + zz);
    d[1] = xy + wz;
    d[2] = xz - wy;
    d[3] = 0.0f;
    d[4] = xy - wz;
    d[5] = 1.0f - (xx + zz);
    d[6] = yz + wx;
    d[7] = 0.0f;
    d[8] = xz + wy;
    d[9] = yz - wx;
    d[10] = 1.0f - (xx + yy);
    d[11] = 0.0f;
    d[12] = 0.0f;
    d[13] = 0.0f;
    d[14] = 0.0f;
    d[15] = 1.0f;
}

static inline ks_quat ks_quat_from_euler(const ks_euler* e) {
    return ks_euler_to_quat(e);
}

static inline void ks_quat_to_mat4(ks_mat4* out, const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(out && q);
    ks_mat4_id(out);

    float w = q->w, x = q->x, y = q->y, z = q->z;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx2 = x * x2, yy2 = y * y2, zz2 = z * z2;
    float wx2 = w * x2, wy2 = w * y2, wz2 = w * z2;
    float xy2 = x * y2, xz2 = x * z2, yz2 = y * z2;

    float* d = out->data;

    d[0] = 1.0f - (yy2 + zz2);
    d[1] = xy2 + wz2;
    d[2] = xz2 - wy2;
    d[4] = xy2 - wz2;
    d[5] = 1.0f - (xx2 + zz2);
    d[6] = yz2 + wx2;
    d[8] = xz2 + wy2;
    d[9] = yz2 - wx2;
    d[10] = 1.0f - (xx2 + yy2);
}

static inline bool ks_quat_isunitary(const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(q);
    float l2 = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    return KS_FEQ(l2, 1.0f, KS_FEPS_MATH);
}

static inline bool ks_quat_isidentity(const ks_quat* q) {
    KS_ASSERT_NONNULL_ARGS(q);
    return KS_FEQ(q->w, 1.0f, KS_FEPS_MATH) && KS_FZERO(q->x, KS_FEPS_MATH) && KS_FZERO(q->y, KS_FEPS_MATH) &&
           KS_FZERO(q->z, KS_FEPS_MATH);
}

/* Derivatives and integrals */

KS_FUNC(double, realf_1d, double x, void* args);
KS_FUNC(double, realf_2d, double x, double y, void* args);
KS_FUNC(double, realf_nd, const double* vars, int32_t dims, void* args);
KS_FUNC(ks_vec2, odef_2d, double t, ks_vec2 state, void* args);
KS_FUNC(ks_vec3, odef_3d, double t, ks_vec3 state, void* args);

static inline double ks_deriv_1d(ks_realf_1d f, double x, double h, void* args) {
    KS_ASSERT(KS_DNZERO(h, KS_DEPS_MATH), "Division by zero");
    return (f(x + h, args) - f(x - h, args)) / (2.0 * h);
}

static inline double ks_deriv_2d_x(ks_realf_2d f, double x, double y, double h, void* args) {
    KS_ASSERT(KS_DNZERO(h, KS_DEPS_MATH), "Division by zero");
    return (f(x + h, y, args) - f(x - h, y, args)) / (2.0 * h);
}

static inline double ks_deriv_2d_y(ks_realf_2d f, double x, double y, double h, void* args) {
    KS_ASSERT(KS_DNZERO(h, KS_DEPS_MATH), "Division by zero");
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
            double r = (double)rand() / (double)RAND_MAX;
            pt[d] = min_bounds[d] + r * (max_bounds[d] - min_bounds[d]);
        }

        sum += f(pt, dims, args);
    }

    return volume * (sum / (double)samples);
}

/* ODE solvers */

static inline void ks_integ_euler(double* val, double rate, double dt) {
    *val += rate * dt;
}

static inline void ks_integ_semi_euler_2d(ks_vec2* pos, ks_vec2* vel, ks_vec2 accel, double dt) {
    vel->x += accel.x * dt;
    vel->y += accel.y * dt;

    pos->x += vel->x * dt;
    pos->y += vel->y * dt;
}

static inline void ks_integ_semi_euler_3d(ks_vec3* pos, ks_vec3* vel, ks_vec3 accel, double dt) {
    vel->x += accel.x * dt;
    vel->y += accel.y * dt;
    vel->z += accel.z * dt;

    pos->x += vel->x * dt;
    pos->y += vel->y * dt;
    pos->z += vel->z * dt;
}

static inline ks_vec2 ks_integ_rk4_2d(ks_odef_2d f, double t, ks_vec2 state, double dt, void* args) {
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

static inline ks_vec3 ks_integ_rk4_3d(ks_odef_3d f, double t, ks_vec3 state, double dt, void* args) {
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
    KS_ASSERT(KS_DNZERO(h2, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(h2, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(h2, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(hsq, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(h2, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(h2, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(h2, KS_DEPS_MATH), "Division by zero");

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
    KS_ASSERT(KS_DNZERO(hsq, KS_DEPS_MATH), "Division by zero");

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

#if !defined(KS_MEM_IMPL) && !defined(KS_MEM_IMPL_DONE)
#error "kitsune: math.h requires mem.h"
#endif

/* Tensors */

#define KS_TENSOR_VARS(t, prefix)                                                                                      \
    ks_tensor_int* restrict KS_CONCAT2(prefix, shape) = t->shape, * restrict KS_CONCAT2(prefix, strides) = t->strides; \
    float* restrict KS_CONCAT2(prefix, data) = t->data

#define KS_CONST_TENSOR_VARS(t, prefix)                                                                          \
    const ks_tensor_int* restrict KS_CONCAT2(prefix, shape) = t->shape, * restrict KS_CONCAT2(prefix, strides) = \
                                                                            t->strides;                          \
    const float* restrict KS_CONCAT2(prefix, data) = t->data

static inline void ks_tensor_calc_strides(ks_tensor_int* strides, int32_t length, const ks_tensor_int* shape) {
    if (length <= 0) {
        return;
    }

    strides[length - 1] = 1;

    for (int32_t i = length - 2; i >= 0; --i) {
        strides[i] = strides[i + 1] * shape[i + 1];
    }
}

static inline ks_tensor_int ks_tensor_index(const ks_tensor* t, ks_tensor_int logical_idx) {
    ks_tensor_int physical_idx = 0;
    ks_tensor_int tmp_idx = logical_idx;

    const int32_t ndims = t->ndims;

    KS_CONST_TENSOR_VARS(t, t);

    for (int32_t i = ndims - 1; i >= 0; --i) {
        ks_tensor_int s = tshape[i];
        ks_tensor_int coord = tmp_idx % s;
        physical_idx += coord * tstrides[i];
        tmp_idx /= s;
    }

    return physical_idx;
}

KS_API ks_status ks_tensor_init(ks_tensor* t, const int32_t ndims, const ks_tensor_int* shape, const ks_allocator* a) {
    KS_ASSERT_NONNULL_ARGS(t && shape);

    if (ndims < 1 || ndims > 8) {
        return KS_STATUS(KS_ERR_BOUNDS, "Number of dimensions out of range [1, 8]");
    }

    memcpy(t->shape, shape, sizeof(ks_tensor_int) * (size_t)ndims);
    ks_tensor_calc_strides(t->strides, ndims, shape);

    ks_tensor_int count = 1;
    for (int32_t i = 0; i < ndims; ++i) {
        if (shape[i] <= 0) {
            return KS_STATUS(KS_ERR_INVALID, "Shape dimensions must be positive");
        }
        count *= shape[i];
    }

    float* data = ks_alloc(a, sizeof(float) * (size_t)count);
    if (!data) {
        return KS_STATUS(KS_ERR_OOM);
    }

    t->allocator = a ? a : &std_allocator;
    t->ndims = ndims;
    t->data = data;

    KS_ASSERT(ks_tensor_is_contiguous(t), "Tensor is not contigous");

    return KS_STATUS(KS_OK);
}

KS_API void ks_tensor_fini(ks_tensor* t) {
    // NULL allocator means the tensor is a view
    if (!t || !t->allocator) {
        return;
    }

    ks_free(t->allocator, t->data);
}

KS_API void ks_tensor_copy(ks_tensor* dst, const ks_tensor* src) {
    KS_ASSERT_NONNULL_ARGS(dst && src);
    KS_ASSERT(dst->ndims == src->ndims, "Tensor dimensions mismatch");
    KS_ASSERT(dst->data && src->data, "Tensors must have allocated buffer");

    const int32_t ndims = dst->ndims;
    KS_TENSOR_VARS(dst, d);
    KS_CONST_TENSOR_VARS(src, s);

    ks_tensor_int count = 1;
    for (int32_t i = 0; i < ndims; ++i) {
        ks_tensor_int s = sshape[i];
        KS_ASSERT(dshape[i] == s, "Tensor shapes mismatch");
        count *= s;
    }

    const bool dcont = ks_tensor_is_contiguous(dst), scont = ks_tensor_is_contiguous(src);

    if (dcont && scont) {
        memcpy(ddata, sdata, sizeof(float) * (size_t)count);
        return;
    }

    for (ks_tensor_int i = 0; i < count; ++i) {
        ks_tensor_int dsti = dcont ? i : ks_tensor_index(dst, i);
        ks_tensor_int srci = scont ? i : ks_tensor_index(src, i);
        ddata[dsti] = sdata[srci];
    }
}

KS_API ks_status ks_tensor_clone(ks_tensor* dst, const ks_tensor* src, const ks_allocator* a) {
    KS_ASSERT_NONNULL_ARGS(dst && src);

    ks_status status = ks_tensor_init(dst, src->ndims, src->shape, a);
    if (status.code != KS_OK) {
        return status;
    }

    ks_tensor_copy(dst, src);

    return KS_STATUS(KS_OK);
}

KS_API void ks_tensor_view(ks_tensor* dst, const ks_tensor* src) {
    KS_ASSERT_NONNULL_ARGS(dst && src);

    *dst = *src;
    dst->allocator = NULL;
}

KS_API ks_tensor_int ks_tensor_count(const ks_tensor* t) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->data, "Tensor is not initialized");

    const int32_t ndims = t->ndims;
    if (ndims == 0) {
        return 0;
    }

    const ks_tensor_int* shape = t->shape;
    ks_tensor_int count = 1;
    for (int32_t i = 0; i < ndims; ++i) {
        count *= shape[i];
    }

    return count;
}

KS_API bool ks_tensor_is_contiguous(const ks_tensor* t) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->shape && t->strides, "Tensors shape or strides is NULL");

    ks_tensor_int expected_stride = 1;
    const int32_t n = t->ndims - 1;
    const ks_tensor_int* restrict shape = t->shape, * restrict strides = t->strides;

    for (int32_t i = n; i >= 0; --i) {
        if (strides[i] != expected_stride) {
            return false;
        }

        expected_stride *= shape[i];
    }

    return true;
}

KS_API bool ks_tensor_same_shape(const ks_tensor* a, const ks_tensor* b) {
    KS_ASSERT_NONNULL_ARGS(a && b);
    KS_ASSERT(a->shape && b->shape, "Tensors data is NULL");

    const int32_t na = a->ndims;
    const int32_t nb = b->ndims;

    if (na != nb) {
        return false;
    }

    const ks_tensor_int* restrict sa = a->shape, * restrict sb = b->shape;

    for (int32_t i = 0; i < na; ++i) {
        if (sa[i] != sb[i]) {
            return false;
        }
    }

    return true;
}

KS_API ks_status ks_tensor_contiguous(ks_tensor* dst, const ks_tensor* src, const ks_allocator* a) {
    KS_ASSERT_NONNULL_ARGS(dst && src);

    if (ks_tensor_is_contiguous(src)) {
        ks_tensor_view(dst, src);
        return KS_STATUS(KS_OK);
    }

    ks_status status = ks_tensor_init(dst, src->ndims, src->shape, a);
    if (status.code != KS_OK) {
        return status;
    }

    ks_tensor_copy(dst, src);
    return KS_STATUS(KS_OK);
}

KS_API ks_status ks_tensor_reshape(ks_tensor* t, const int32_t new_ndims, const ks_tensor_int* new_shape) {
    KS_ASSERT_NONNULL_ARGS(t && new_shape);

    if (new_ndims < 1 || new_ndims > 8) {
        return KS_STATUS(KS_ERR_BOUNDS, "Number of dimensions out of range [1, 8]");
    }

    if (!ks_tensor_is_contiguous(t)) {
        return KS_STATUS(KS_ERR_INVALID, "Tensor is not contiguous");
    }

    ks_tensor_int new_count = 1;
    for (int32_t i = 0; i < new_ndims; ++i) {
        if (new_shape[i] <= 0) {
            return KS_STATUS(KS_ERR_INVALID, "Shape dimensions must be positive");
        }
        new_count *= new_shape[i];
    }

    if (ks_tensor_count(t) != new_count) {
        return KS_STATUS(KS_ERR_INVALID, "Shape mismatch");
    }

    memcpy(t->shape, new_shape, sizeof(ks_tensor_int) * (size_t)new_ndims);
    ks_tensor_calc_strides(t->strides, new_ndims, new_shape);
    t->ndims = new_ndims;

    return KS_STATUS(KS_OK);
}

KS_API ks_status ks_tensor_transpose(ks_tensor* t, const int32_t dim1, const int32_t dim2) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->shape && t->strides, "Tensors shape or strides is NULL");

    if (dim1 < 0 || dim1 >= t->ndims) {
        return KS_STATUS(KS_ERR_BOUNDS, "dim1 out of bounds");
    }

    if (dim2 < 0 || dim2 >= t->ndims) {
        return KS_STATUS(KS_ERR_BOUNDS, "dim2 out of bounds");
    }

    if (dim1 == dim2) {
        return KS_STATUS(KS_OK);
    }

    KS_SWAP(t->shape[dim1], t->shape[dim2]);
    KS_SWAP(t->strides[dim1], t->strides[dim2]);

    return KS_STATUS(KS_OK);
}

KS_API void ks_tensor_zeros(ks_tensor* t) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->data, "Tensors data is NULL");

    const ks_tensor_int count = ks_tensor_count(t);
    float* restrict data = t->data;

    if (KS_LIKELY(ks_tensor_is_contiguous(t))) {
        memset(data, 0, sizeof(float) * (size_t)count);
        return;
    }

    for (ks_tensor_int i = 0; i < count; ++i) {
        ks_tensor_int j = ks_tensor_index(t, i);
        data[j] = 0.0f;
    }
}

KS_API void ks_tensor_fill(ks_tensor* t, const float val) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->data, "Tensors data is NULL");

    if (KS_FEQ(val, 0.0f, KS_FEPS_MATH)) {
        ks_tensor_zeros(t);
        return;
    }

    const ks_tensor_int count = ks_tensor_count(t);
    float* restrict data = t->data;

    if (KS_LIKELY(ks_tensor_is_contiguous(t))) {
        KS_SIMD_HINT for (ks_tensor_int i = 0; i < count; ++i) {
            data[i] = val;
        }

        return;
    }

    for (ks_tensor_int i = 0; i < count; ++i) {
        ks_tensor_int j = ks_tensor_index(t, i);
        data[j] = val;
    }
}

KS_API void ks_tensor_rand_uniform(ks_tensor* t, const float min, const float max) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->data, "Tensors data is NULL");
    KS_ASSERT(KS_FLESSEQ(min, max, KS_FEPS_MATH), "Min is greater than max");

    if (KS_UNLIKELY(KS_FEQ(min, 0.0f, KS_FEPS_MATH) && KS_FEQ(max, 0.0f, KS_FEPS_MATH))) {
        ks_tensor_zeros(t);
        return;
    }

    if (KS_UNLIKELY(KS_FEQ(min, max, KS_FEPS_MATH))) {
        ks_tensor_fill(t, min);
        return;
    }

    const ks_tensor_int count = ks_tensor_count(t);
    float* restrict data = t->data;
    const bool cont = ks_tensor_is_contiguous(t);

    const float dist = max - min;
    for (ks_tensor_int i = 0; i < count; ++i) {
        ks_tensor_int j = cont ? i : ks_tensor_index(t, i);
        data[j] = (float)rand() / (float)RAND_MAX * dist + min;
    }
}

KS_API void ks_tensor_rand_normal(ks_tensor* t, const float mean, const float stddev) {
    KS_ASSERT_NONNULL_ARGS(t);
    KS_ASSERT(t->data, "Tensors data is NULL");

    const ks_tensor_int count = ks_tensor_count(t);
    float* restrict data = t->data;
    const bool cont = ks_tensor_is_contiguous(t);

    if (cont && KS_FEQ(mean, 0.0f, KS_FEPS_MATH) && KS_FEQ(stddev, 0.0f, KS_FEPS_MATH)) {
        memset(data, 0, sizeof(float) * (size_t)count);
        return;
    }

    float a, b, r;

    for (ks_tensor_int i = 0; i < count; ++i) {
        ks_tensor_int j = cont ? i : ks_tensor_index(t, i);

        a = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);
        b = (float)rand() / (float)RAND_MAX;
        r = sqrtf(-2.0f * logf(a)) * cosf(KS_2PI * b);

        data[j] = r * stddev + mean;
    }
}

#define KS_TENSOR_DEFINE_UNARY(name, expr, ...)                                                    \
    KS_API void KS_CONCAT2(ks_tensor_, name)(ks_tensor * out, const ks_tensor* a, ##__VA_ARGS__) { \
        KS_ASSERT_NONNULL_ARGS(out && a);                                                          \
        KS_ASSERT(out->data && a->data, "Tensors data is NULL");                                   \
        KS_ASSERT(ks_tensor_same_shape(out, a), "Tensors have different shapes");                  \
        KS_ASSERT(out->data != a->data, "Overlapping tensors. Use in-place variants");             \
                                                                                                   \
        const ks_tensor_int count = ks_tensor_count(out);                                          \
        const bool cout = ks_tensor_is_contiguous(out), ct = ks_tensor_is_contiguous(a);           \
        float* restrict dout = out->data, x;                                                       \
        const float* restrict dt = a->data;                                                        \
                                                                                                   \
        if (KS_LIKELY(cout && ct)) {                                                               \
            KS_SIMD_HINT for (ks_tensor_int i = 0; i < count; ++i) {                               \
                x = dt[i];                                                                         \
                dout[i] = (expr);                                                                  \
            }                                                                                      \
                                                                                                   \
            return;                                                                                \
        }                                                                                          \
                                                                                                   \
        for (ks_tensor_int i = 0; i < count; ++i) {                                                \
            ks_tensor_int j = cout ? i : ks_tensor_index(out, i);                                  \
            ks_tensor_int k = ct ? i : ks_tensor_index(a, i);                                      \
            x = dt[k];                                                                             \
            dout[j] = (expr);                                                                      \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    KS_API void KS_CONCAT3(ks_tensor_, name, i)(ks_tensor * a, ##__VA_ARGS__) {                    \
        KS_ASSERT_NONNULL_ARGS(a);                                                                 \
        KS_ASSERT(a->data, "Tensors data is NULL");                                                \
                                                                                                   \
        const ks_tensor_int count = ks_tensor_count(a);                                            \
        float *data = a->data, x;                                                                  \
        const bool cont = ks_tensor_is_contiguous(a);                                              \
                                                                                                   \
        if (KS_LIKELY(cont)) {                                                                     \
            KS_SIMD_HINT for (ks_tensor_int i = 0; i < count; ++i) {                               \
                x = data[i];                                                                       \
                data[i] = (expr);                                                                  \
            }                                                                                      \
                                                                                                   \
            return;                                                                                \
        }                                                                                          \
                                                                                                   \
        for (ks_tensor_int i = 0; i < count; ++i) {                                                \
            ks_tensor_int j = ks_tensor_index(a, i);                                               \
            x = data[j];                                                                           \
            data[j] = (expr);                                                                      \
        }                                                                                          \
    }

#define KS_TENSOR_DEFINE_BINARY(name, expr, ...)                                                                       \
    KS_API void KS_CONCAT2(ks_tensor_, name)(ks_tensor * out, const ks_tensor* a, const ks_tensor* b, ##__VA_ARGS__) { \
        KS_ASSERT_NONNULL_ARGS(out && a && b);                                                                         \
        KS_ASSERT(out->data && a->data && b->data, "Tensors data is NULL");                                            \
        KS_ASSERT(ks_tensor_same_shape(out, a) && ks_tensor_same_shape(a, b), "Tensors have different shapes");        \
        KS_ASSERT(out->data != a->data && out->data != b->data, "Overlapping tensors. Use in-place variants");         \
                                                                                                                       \
        const ks_tensor_int count = ks_tensor_count(a);                                                                \
        float* restrict dout = out->data, x, y;                                                                        \
        const float* restrict da = a->data, * restrict db = b->data;                                                   \
        const bool cout = ks_tensor_is_contiguous(out), ca = ks_tensor_is_contiguous(a),                               \
                   cb = ks_tensor_is_contiguous(b);                                                                    \
                                                                                                                       \
        if (KS_LIKELY(cout && ca && cb)) {                                                                             \
            KS_SIMD_HINT for (ks_tensor_int i = 0; i < count; ++i) {                                                   \
                x = da[i];                                                                                             \
                y = db[i];                                                                                             \
                dout[i] = (expr);                                                                                      \
            }                                                                                                          \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        for (ks_tensor_int i = 0; i < count; ++i) {                                                                    \
            ks_tensor_int j = cout ? i : ks_tensor_index(out, i);                                                      \
            ks_tensor_int k = ca ? i : ks_tensor_index(a, i);                                                          \
            ks_tensor_int w = cb ? i : ks_tensor_index(b, i);                                                          \
            x = da[k];                                                                                                 \
            y = db[w];                                                                                                 \
            dout[j] = (expr);                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    KS_API void KS_CONCAT3(ks_tensor_, name, i)(ks_tensor * a, const ks_tensor* b, ##__VA_ARGS__) {                    \
        KS_ASSERT_NONNULL_ARGS(a && b);                                                                                \
        KS_ASSERT(a->data && b->data, "Tensors data is NULL");                                                         \
        KS_ASSERT(ks_tensor_same_shape(a, b), "Tensors have different shapes");                                        \
                                                                                                                       \
        const ks_tensor_int count = ks_tensor_count(a);                                                                \
        float *da = a->data, x, y;                                                                                     \
        const float* db = b->data;                                                                                     \
        const bool ca = ks_tensor_is_contiguous(a), cb = ks_tensor_is_contiguous(b);                                   \
                                                                                                                       \
        if (KS_LIKELY(ca && cb)) {                                                                                     \
            KS_SIMD_HINT for (ks_tensor_int i = 0; i < count; ++i) {                                                   \
                x = da[i];                                                                                             \
                y = db[i];                                                                                             \
                da[i] = (expr);                                                                                        \
            }                                                                                                          \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        for (ks_tensor_int i = 0; i < count; ++i) {                                                                    \
            ks_tensor_int j = ca ? i : ks_tensor_index(a, i);                                                          \
            ks_tensor_int k = cb ? i : ks_tensor_index(b, i);                                                          \
            x = da[j];                                                                                                 \
            y = db[k];                                                                                                 \
            da[j] = (expr);                                                                                            \
        }                                                                                                              \
    }

#define KS_TENSOR_DEFINE_REDUCE(name, init_val, update_expr)               \
    KS_API float KS_CONCAT2(ks_tensor_reduce_, name)(const ks_tensor* a) { \
        KS_ASSERT_NONNULL_ARGS(a);                                         \
        KS_ASSERT(a->data, "Tensors data is NULL");                        \
                                                                           \
        const ks_tensor_int count = ks_tensor_count(a);                    \
        const float* restrict data = a->data;                              \
        float acc = (init_val), val;                                       \
                                                                           \
        if (KS_LIKELY(ks_tensor_is_contiguous(a))) {                       \
            KS_SIMD_HINT for (ks_tensor_int i = 0; i < count; ++i) {       \
                val = data[i];                                             \
                acc = (update_expr);                                       \
            }                                                              \
                                                                           \
            return acc;                                                    \
        }                                                                  \
                                                                           \
        for (ks_tensor_int i = 0; i < count; ++i) {                        \
            ks_tensor_int j = ks_tensor_index(a, i);                       \
            val = data[j];                                                 \
            acc = (update_expr);                                           \
        }                                                                  \
                                                                           \
        return acc;                                                        \
    }                                                                      \
                                                                           \
    KS_API void KS_CONCAT3(ks_tensor_reduce_, name, i)(ks_tensor * a) {    \
        KS_ASSERT_NONNULL_ARGS(a);                                         \
        KS_ASSERT(a->data, "Tensors data is NULL");                        \
                                                                           \
        const float res = KS_CONCAT2(ks_tensor_reduce_, name)(a);          \
                                                                           \
        a->ndims = 1;                                                      \
        a->data[0] = res;                                                  \
        a->shape[0] = 1;                                                   \
        a->strides[0] = 1;                                                 \
    }

static inline bool ks_tensor_is_valid_reduction_shape(const ks_tensor* out, const ks_tensor* a, int32_t axis) {
    if (out->ndims != a->ndims || out->shape[axis] != 1) {
        return false;
    }

    for (int32_t i = 0; i < a->ndims; ++i) {
        if (i != axis && out->shape[i] != a->shape[i]) {
            return false;
        }
    }

    return true;
}

#define KS_TENSOR_DEFINE_REDUCE_AXIS(name, init_val, update_expr)                                              \
    KS_API void KS_CONCAT2(ks_tensor_reduce_axis_, name)(ks_tensor * out, const ks_tensor* a, int32_t axis) {  \
        KS_ASSERT_NONNULL_ARGS(out && a);                                                                      \
        KS_ASSERT(out->data != a->data, "Use in-place variant");                                               \
                                                                                                               \
        const int32_t na = a->ndims;                                                                           \
        if (axis < 0) {                                                                                        \
            axis += na;                                                                                        \
        }                                                                                                      \
        KS_ASSERT(axis >= 0 && axis < na, "Axis out of bounds");                                               \
        KS_ASSERT(ks_tensor_is_valid_reduction_shape(out, a, axis), "Shape mismatch");                         \
                                                                                                               \
        const int32_t nout = out->ndims;                                                                       \
        const ks_tensor_int count = ks_tensor_count(out), *a_shape = a->shape, *a_strides = a->strides,        \
                            *out_shape = out->shape, *out_strides = out->strides, axis_count = a_shape[axis],  \
                            axis_stride = a_strides[axis];                                                     \
        ks_tensor_int tmp, outidx, aidx, coord;                                                                \
        const float* restrict da = a->data, *row;                                                              \
        float* restrict dout = out->data, acc, val;                                                            \
                                                                                                               \
        if (KS_LIKELY(axis == na - 1 && ks_tensor_is_contiguous(a) && ks_tensor_is_contiguous(out))) {         \
            for (ks_tensor_int i = 0; i < count; ++i) {                                                        \
                acc = (init_val);                                                                              \
                row = KS_PTROFF_UNCAST(da, i * axis_count);                                                    \
                KS_SIMD_HINT for (ks_tensor_int j = 0; j < axis_count; ++j) {                                  \
                    val = row[j];                                                                              \
                    acc = (update_expr);                                                                       \
                }                                                                                              \
                dout[i] = acc;                                                                                 \
            }                                                                                                  \
                                                                                                               \
            return;                                                                                            \
        }                                                                                                      \
                                                                                                               \
        for (ks_tensor_int i = 0; i < count; ++i) {                                                            \
            tmp = i;                                                                                           \
            outidx = 0;                                                                                        \
            aidx = 0;                                                                                          \
            for (int32_t j = nout - 1; j >= 0; --j) {                                                          \
                coord = tmp % out_shape[j];                                                                    \
                outidx += coord * out_strides[j];                                                              \
                aidx += coord * a_strides[j];                                                                  \
                tmp /= out_shape[j];                                                                           \
            }                                                                                                  \
                                                                                                               \
            acc = (init_val);                                                                                  \
            for (ks_tensor_int j = 0; j < axis_count; ++j) {                                                   \
                val = da[aidx];                                                                                \
                acc = (update_expr);                                                                           \
                aidx += axis_stride;                                                                           \
            }                                                                                                  \
                                                                                                               \
            dout[outidx] = acc;                                                                                \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    KS_API void KS_CONCAT3(ks_tensor_reduce_axis_, name, i)(ks_tensor * a, int32_t axis) {                     \
        KS_ASSERT_NONNULL_ARGS(a);                                                                             \
                                                                                                               \
        const int32_t na = a->ndims;                                                                           \
        if (axis < 0) {                                                                                        \
            axis += na;                                                                                        \
        }                                                                                                      \
        KS_ASSERT(axis >= 0 && axis < na, "Axis out of bounds");                                               \
                                                                                                               \
        ks_tensor_int *a_shape = a->shape, *a_strides = a->strides, out_shape[8], count = 1, tmp, aidx, coord; \
        const ks_tensor_int axis_count = a_shape[axis], axis_stride = a_strides[axis];                         \
        float *data = a->data, acc, val, *row;                                                                 \
                                                                                                               \
        memcpy(out_shape, a_shape, sizeof(ks_tensor_int) * (size_t)na);                                        \
        out_shape[axis] = 1;                                                                                   \
        for (int32_t j = 0; j < na; ++j) {                                                                     \
            count *= out_shape[j];                                                                             \
        }                                                                                                      \
                                                                                                               \
        if (KS_LIKELY(axis == na - 1 && ks_tensor_is_contiguous(a))) {                                         \
            for (ks_tensor_int i = 0; i < count; ++i) {                                                        \
                acc = (init_val);                                                                              \
                row = KS_PTROFF_UNCAST(data, i * axis_count);                                                  \
                KS_SIMD_HINT for (ks_tensor_int j = 0; j < axis_count; ++j) {                                  \
                    val = row[j];                                                                              \
                    acc = (update_expr);                                                                       \
                }                                                                                              \
                data[i] = acc;                                                                                 \
            }                                                                                                  \
        } else {                                                                                               \
            for (ks_tensor_int i = 0; i < count; ++i) {                                                        \
                tmp = i;                                                                                       \
                aidx = 0;                                                                                      \
                for (int32_t j = na - 1; j >= 0; --j) {                                                        \
                    coord = tmp % out_shape[j];                                                                \
                    aidx += coord * a_strides[j];                                                              \
                    tmp /= out_shape[j];                                                                       \
                }                                                                                              \
                                                                                                               \
                acc = (init_val);                                                                              \
                for (ks_tensor_int j = 0; j < axis_count; ++j) {                                               \
                    val = data[aidx];                                                                          \
                    acc = (update_expr);                                                                       \
                    aidx += axis_stride;                                                                       \
                }                                                                                              \
                                                                                                               \
                data[i] = acc;                                                                                 \
            }                                                                                                  \
        }                                                                                                      \
                                                                                                               \
        a_shape[axis] = 1;                                                                                     \
        ks_tensor_calc_strides(a_strides, na, a_shape);                                                        \
    }

#define KS_TENSOR_DEFINE_REDUCE_AXIS_INDEX(name, init_val, comp_expr)                                             \
    KS_API void KS_CONCAT2(ks_tensor_reduce_axis_, name)(ks_tensor * out, const ks_tensor* a, int32_t axis) {     \
        KS_ASSERT_NONNULL_ARGS(out && a);                                                                         \
        KS_ASSERT(out->data != a->data, "Use in-place variant");                                                  \
                                                                                                                  \
        const int32_t na = a->ndims;                                                                              \
        if (axis < 0) {                                                                                           \
            axis += na;                                                                                           \
        }                                                                                                         \
        KS_ASSERT(axis >= 0 && axis < na, "Axis out of bounds");                                                  \
        KS_ASSERT(ks_tensor_is_valid_reduction_shape(out, a, axis), "Shape mismatch");                            \
                                                                                                                  \
        const int32_t nout = out->ndims;                                                                          \
        const ks_tensor_int count = ks_tensor_count(out), *a_shape = a->shape, *a_strides = a->strides,           \
                            *out_shape = out->shape, *out_strides = out->strides, axis_count = a_shape[axis],     \
                            axis_stride = a_strides[axis];                                                        \
        ks_tensor_int best_idx, tmp, outidx, aidx, coord;                                                         \
        const float* restrict da = a->data, *row;                                                                 \
        float* restrict dout = out->data, best_val;                                                               \
                                                                                                                  \
        if (KS_LIKELY(axis == na - 1 && ks_tensor_is_contiguous(a) && ks_tensor_is_contiguous(out))) {            \
            for (ks_tensor_int i = 0; i < count; ++i) {                                                           \
                best_val = (init_val);                                                                            \
                best_idx = 0;                                                                                     \
                row = KS_PTROFF_UNCAST(da, i * axis_count);                                                       \
                KS_SIMD_HINT for (ks_tensor_int j = 0; j < axis_count; ++j) {                                     \
                    if (row[j] comp_expr best_val) {                                                              \
                        best_val = row[j];                                                                        \
                        best_idx = j;                                                                             \
                    }                                                                                             \
                }                                                                                                 \
                dout[i] = (float)best_idx;                                                                        \
            }                                                                                                     \
                                                                                                                  \
            return;                                                                                               \
        }                                                                                                         \
                                                                                                                  \
        for (ks_tensor_int i = 0; i < count; ++i) {                                                               \
            tmp = i;                                                                                              \
            outidx = 0;                                                                                           \
            aidx = 0;                                                                                             \
            for (int32_t d = nout - 1; d >= 0; --d) {                                                             \
                coord = tmp % out_shape[d];                                                                       \
                outidx += coord * out_strides[d];                                                                 \
                aidx += coord * a_strides[d];                                                                     \
                tmp /= out_shape[d];                                                                              \
            }                                                                                                     \
                                                                                                                  \
            best_val = (init_val);                                                                                \
            best_idx = 0;                                                                                         \
            for (ks_tensor_int j = 0; j < axis_count; ++j) {                                                      \
                if (da[aidx] comp_expr best_val) {                                                                \
                    best_val = da[aidx];                                                                          \
                    best_idx = j;                                                                                 \
                }                                                                                                 \
                aidx += axis_stride;                                                                              \
            }                                                                                                     \
                                                                                                                  \
            dout[outidx] = (float)best_idx;                                                                       \
        }                                                                                                         \
    }                                                                                                             \
                                                                                                                  \
    KS_API void KS_CONCAT3(ks_tensor_reduce_axis_, name, i)(ks_tensor * a, int32_t axis) {                        \
        KS_ASSERT_NONNULL_ARGS(a);                                                                                \
                                                                                                                  \
        const int32_t na = a->ndims;                                                                              \
        if (axis < 0) {                                                                                           \
            axis += na;                                                                                           \
        }                                                                                                         \
        KS_ASSERT(axis >= 0 && axis < na, "Axis out of bounds");                                                  \
                                                                                                                  \
        ks_tensor_int *a_shape = a->shape, *a_strides = a->strides, out_shape[8], count = 1, best_idx, tmp, aidx, \
                      coord;                                                                                      \
        const ks_tensor_int axis_count = a_shape[axis], axis_stride = a_strides[axis];                            \
        float *data = a->data, best_val, *row;                                                                    \
                                                                                                                  \
        memcpy(out_shape, a_shape, sizeof(ks_tensor_int) * (size_t)na);                                           \
        out_shape[axis] = 1;                                                                                      \
        for (int32_t j = 0; j < na; ++j) {                                                                        \
            count *= out_shape[j];                                                                                \
        }                                                                                                         \
                                                                                                                  \
        if (KS_LIKELY(axis == na - 1 && ks_tensor_is_contiguous(a))) {                                            \
            for (ks_tensor_int i = 0; i < count; ++i) {                                                           \
                best_val = (init_val);                                                                            \
                best_idx = 0;                                                                                     \
                row = KS_PTROFF_UNCAST(data, i * axis_count);                                                     \
                KS_SIMD_HINT for (ks_tensor_int j = 0; j < axis_count; ++j) {                                     \
                    if (row[j] comp_expr best_val) {                                                              \
                        best_val = row[j];                                                                        \
                        best_idx = j;                                                                             \
                    }                                                                                             \
                }                                                                                                 \
                data[i] = (float)best_idx;                                                                        \
            }                                                                                                     \
        } else {                                                                                                  \
            for (ks_tensor_int i = 0; i < count; ++i) {                                                           \
                tmp = i;                                                                                          \
                aidx = 0;                                                                                         \
                for (int32_t j = na - 1; j >= 0; --j) {                                                           \
                    coord = tmp % out_shape[j];                                                                   \
                    aidx += coord * a_strides[j];                                                                 \
                    tmp /= out_shape[j];                                                                          \
                }                                                                                                 \
                                                                                                                  \
                best_val = (init_val);                                                                            \
                best_idx = 0;                                                                                     \
                for (ks_tensor_int j = 0; j < axis_count; ++j) {                                                  \
                    if (data[aidx] comp_expr best_val) {                                                          \
                        best_val = data[aidx];                                                                    \
                        best_idx = j;                                                                             \
                    }                                                                                             \
                    aidx += axis_stride;                                                                          \
                }                                                                                                 \
                                                                                                                  \
                data[i] = (float)best_idx;                                                                        \
            }                                                                                                     \
        }                                                                                                         \
                                                                                                                  \
        a_shape[axis] = 1;                                                                                        \
        ks_tensor_calc_strides(a_strides, na, a_shape);                                                           \
    }

KS_TENSOR_DEFINE_UNARY(neg, -x);
KS_TENSOR_DEFINE_UNARY(clamp, KS_CLAMP(x, min, max), float min, float max);
KS_TENSOR_DEFINE_UNARY(pow, powf(x, exponent), float exponent);
KS_TENSOR_DEFINE_UNARY(sqrt, sqrtf(x));
KS_TENSOR_DEFINE_UNARY(exp, expf(x));
KS_TENSOR_DEFINE_UNARY(log, logf(x));
KS_TENSOR_DEFINE_UNARY(abs, fabsf(x));
KS_TENSOR_DEFINE_UNARY(sigmoid, 1.0f / (1.0f + expf(-x)));
KS_TENSOR_DEFINE_UNARY(tanh, 2.0f / (1.0f + expf(-2.0f * x)) - 1.0f);
KS_TENSOR_DEFINE_UNARY(relu, x > 0.0f ? x : 0.0f);
KS_TENSOR_DEFINE_UNARY(leaky_relu, x > 0 ? x : alpha * x, float alpha);
KS_TENSOR_DEFINE_UNARY(softplus, logf(1.0f + expf(x)));
KS_TENSOR_DEFINE_UNARY(elu, x > 0 ? x : alpha * (expf(x) - 1.0f), float alpha);
KS_TENSOR_DEFINE_UNARY(selu, x > 0 ? scale * x : scale * alpha * (expf(x) - 1.0f), float scale, float alpha);

KS_TENSOR_DEFINE_BINARY(add, x + y);
KS_TENSOR_DEFINE_BINARY(sub, x - y);
KS_TENSOR_DEFINE_BINARY(mul, x* y);
KS_TENSOR_DEFINE_BINARY(div, x / y);

KS_TENSOR_DEFINE_REDUCE(sum, 0.0f, acc + val);
KS_TENSOR_DEFINE_REDUCE(max, -FLT_MAX, val > acc ? val : acc);
KS_TENSOR_DEFINE_REDUCE(min, FLT_MAX, val < acc ? val : acc);

KS_TENSOR_DEFINE_REDUCE_AXIS(sum, 0.0f, acc + val);
KS_TENSOR_DEFINE_REDUCE_AXIS(max, -FLT_MAX, val > acc ? val : acc);
KS_TENSOR_DEFINE_REDUCE_AXIS(min, FLT_MAX, val < acc ? val : acc);
KS_TENSOR_DEFINE_REDUCE_AXIS_INDEX(argmax, -FLT_MAX, >);
KS_TENSOR_DEFINE_REDUCE_AXIS_INDEX(argmin, FLT_MAX, <);

/* Fields */

KS_API ks_field ks_field_create(int32_t w, int32_t h, int32_t d, size_t typesize, double cellsize) {
    void* data = malloc(typesize * (size_t)(w * h * d));
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

/* FFT */

KS_API int fft(size_t n, double complex samples[static n]) {
    if (!KS_ISPOW2(n)) {
        return 1;
    }

// Reorder the array using bitwise-reversed indices
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        size_t j = bit_reverse(i, n);
        if (i < j) {
            double complex tmp = samples[i];
            samples[i] = samples[j];
            samples[j] = tmp;
        }
    }

    const size_t l = log2ld(n);
    const double complex f = -KS_PI_2 * I;
    for (size_t i = 1; i <= l; ++i) {
        size_t m = 1ull << i;  // 2^i
        double complex wm = cexp(f / m);

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