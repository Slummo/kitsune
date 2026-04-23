#pragma once

#include <ks/core.h>
#include <stdio.h>
#include <math.h>

KS_STRUCT(vec2, {
    float x;
    float y;
});

KS_STRUCT(vec3, {
    float x;
    float y;
    float z;
});

KS_STRUCT(vec4, {
    float x;
    float y;
    float z;
    float w;
});

KS_STRUCT(mat2, {
    ks_vec2 c1;
    ks_vec2 c2;
});

KS_STRUCT(mat3, {
    ks_vec3 c1;
    ks_vec3 c2;
    ks_vec3 c3;
});

KS_STRUCT(mat4, {
    ks_vec4 c1;
    ks_vec4 c2;
    ks_vec4 c3;
    ks_vec4 c4;
});

/* vec2 */

static inline ks_vec2 ks_vec2_num(float n) {
    return (ks_vec2){n, n};
}

static inline ks_vec2 ks_vec2_nums(float n1, float n2) {
    return (ks_vec2){n1, n2};
}

static inline ks_vec2 ks_vec2_sum(const ks_vec2* v1, const ks_vec2* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return (ks_vec2){v1->x + v2->x, v1->y + v2->y};
}

static inline ks_vec2 ks_vec2_negate(const ks_vec2* v) {
    KS_ASSERT(v, "Null arguments");
    return (ks_vec2){-v->x, -v->y};
}

static inline ks_vec2 ks_vec2_sub(const ks_vec2* v1, const ks_vec2* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return (ks_vec2){v1->x - v2->x, v1->y - v2->y};
}

static inline ks_vec2 ks_vec2_mul(const ks_vec2* v, float n) {
    KS_ASSERT(v, "Null arguments");
    return (ks_vec2){n * v->x, n * v->y};
}

static inline ks_vec2 ks_vec2_div(const ks_vec2* v, float n) {
    KS_ASSERT(v, "Null arguments");
    KS_ASSERT(n != 0.0f, "Zero division");
    return (ks_vec2){v->x / n, v->y / n};
}

static inline float ks_vec2_dot(const ks_vec2* v1, const ks_vec2* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return sqrtf(v1->x * v2->x + v1->y * v2->y);
}

static inline float ks_vec2_norm(const ks_vec2* v) {
    KS_ASSERT(v, "Null arguments;");
    return ks_vec2_dot(v, v);
}

static inline ks_vec2 ks_vec2_normalize(const ks_vec2* v) {
    KS_ASSERT(v, "Null arguments");
    float norm = ks_vec2_norm(v);
    return ks_vec2_div(v, norm);
}

static inline void ks_vec2_print(const ks_vec2* v, FILE* stream) {
    KS_ASSERT(v && stream, "Null arguments");
    fprintf(stream, "vec2(%f, %f)", v->x, v->y);
}

static inline void ks_vec2_println(const ks_vec2* v, FILE* stream) {
    KS_ASSERT(v && stream, "Null arguments");
    fprintf(stream, "vec2(%f, %f)\n", v->x, v->y);
}

static inline ks_string ks_vec2_tostr(const ks_vec2* v) {
    KS_ASSERT(v, "Null arguments");
    return ks_string_format("vec2(%f, %f)", v->x, v->y);
}

/* vec3 */

static inline ks_vec3 ks_vec3_num(float n) {
    return (ks_vec3){n, n, n};
}

static inline ks_vec3 ks_vec3_nums(float n1, float n2, float n3) {
    return (ks_vec3){n1, n2, n3};
}

static inline ks_vec3 ks_vec3_sum(const ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return (ks_vec3){v1->x + v2->x, v1->y + v2->y, v1->z + v2->z};
}

static inline ks_vec3 ks_vec3_negate(const ks_vec3* v) {
    KS_ASSERT(v, "Null arguments");
    return (ks_vec3){-v->x, -v->y, -v->z};
}

static inline ks_vec3 ks_vec3_sub(const ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return (ks_vec3){v1->x - v2->x, v1->y - v2->y, v1->z - v2->z};
}

static inline ks_vec3 ks_vec3_mul(const ks_vec3* v, float n) {
    KS_ASSERT(v, "Null arguments");
    return (ks_vec3){n * v->x, n * v->y, n * v->z};
}

static inline ks_vec3 ks_vec3_div(const ks_vec3* v, float n) {
    KS_ASSERT(v, "Null arguments");
    KS_ASSERT(n != 0.0f, "Zero division");
    return (ks_vec3){v->x / n, v->y / n, v->z / n};
}

static inline float ks_vec3_dot(const ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return sqrtf(v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

static inline float ks_vec3_norm(const ks_vec3* v) {
    KS_ASSERT(v, "Null arguments;");
    return ks_vec3_dot(v, v);
}

static inline ks_vec3 ks_vec3_normalize(const ks_vec3* v) {
    KS_ASSERT(v, "Null arguments");
    float norm = ks_vec3_norm(v);
    return ks_vec3_div(v, norm);
}

static inline ks_vec3 ks_vec3_cross(const ks_vec3* v1, const ks_vec3* v2) {
    KS_ASSERT(v1 && v2, "Null arguments");
    return (ks_vec3){v1->y * v2->z - v1->z * v2->y, v1->z * v2->x - v1->x * v2->z, v1->x * v2->y - v1->y * v2->x};
}

static inline void ks_vec3_print(const ks_vec3* v, FILE* stream) {
    KS_ASSERT(v && stream, "Null arguments");
    fprintf(stream, "vec3(%f, %f, %f)", v->x, v->y, v->z);
}

static inline void ks_vec3_println(const ks_vec3* v, FILE* stream) {
    KS_ASSERT(v && stream, "Null arguments");
    fprintf(stream, "vec3(%f, %f, %f)\n", v->x, v->y, v->z);
}

static inline ks_string ks_vec3_tostr(const ks_vec3* v) {
    KS_ASSERT(v, "Null arguments");
    return ks_string_format("vec3(%f, %f, %f)", v->x, v->y, v->z);
}

#ifdef KS_MATH_IMPL

#endif