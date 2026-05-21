#define KS_CORE_IMPL
#define KS_MATH_IMPL

#include <ks/core.h>
#include <ks/math.h>

int main(void) {
    ks_vec3 v1 = KS_VEC3(1.0f, 0.0f, 0.0f);
    ks_vec3 v2 = KS_VEC3(0.0f, 1.0f, 0.0f);
    ks_vec3 v3;
    ks_vec3_cross(&v3, &v1, &v2);
    ks_vec3_smuli(&v3, 3.0f);

    ks_print(KS_VEC3_FMT " with length=%f\n", KS_VEC3_ARGS(v3), ks_vec3_length(&v3));

    ks_mat3 m1 = ks_mat3_idnew();
    ks_mat3 m2 = ks_mat3_idnew();
    ks_mat3_smuli(&m1, 0.5f);
    ks_mat3_addi(&m2, &m1);
    ks_mat3_set(&m2, 2, 0, 5.0f);
    ks_mat3_set(&m2, 0, 1, 2.3f);
    ks_mat3 m3;
    ks_mat3_transp(&m3, &m2);

    ks_print(KS_MAT3_FMT "\n\n\twith det=%f\n", KS_MAT3_ARGS(m2), ks_mat3_det(&m2));
    ks_print(KS_MAT3_FMT "\n\n\twith det=%f\n", KS_MAT3_ARGS(m3), ks_mat3_det(&m3));

    ks_mat4 m4;
    ks_mat4_lookat(&m4, &v1, &v2, &v3);
    ks_print(KS_MAT4_FMT "\n\n\twith det=%f\n", KS_MAT4_ARGS(m4), ks_mat4_det(&m4));

    return EXIT_SUCCESS;
}