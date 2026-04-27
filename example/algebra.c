#define KS_CORE_IMPL
#define KS_MATH_IMPL

#include <ks/core.h>
#include <ks/math.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_vec3 v1 = ks_vec3_new(1.0f, 0.0f, 0.0f);
    ks_vec3 v2 = ks_vec3_new(0.0f, 1.0f, 0.0f);
    ks_vec3 v3;
    ks_vec3_cross(&v3, &v1, &v2);
    ks_vec3_smuli(&v3, 3.0f);

    ks_string formatted = ks_vec3_tostr(&v3);
    ks_log(KSINFO, "%s with length=%f\n", ks_string_as_raw(&formatted), ks_vec3_length(&v3));
    ks_string_free(&formatted);

    ks_mat3 m1 = ks_mat3_idnew();
    ks_mat3 m2 = ks_mat3_idnew();
    ks_mat3_smuli(&m1, 0.5f);
    ks_mat3_addi(&m2, &m1);
    ks_mat3_set(&m2, 2, 0, 5.0f);
    ks_mat3_set(&m2, 0, 1, 2.3f);
    ks_mat3 m3;
    ks_mat3_trans(&m3, &m2);

    formatted = ks_mat3_tostr(&m2);
    ks_log(KSINFO, "%s with det=%f\n", ks_string_as_raw(&formatted), ks_mat3_det(&m2));
    ks_string_free(&formatted);

    formatted = ks_mat3_tostr(&m3);
    ks_log(KSINFO, "%s with det=%f\n", ks_string_as_raw(&formatted), ks_mat3_det(&m3));
    ks_string_free(&formatted);

    ks_mat4 m4;
    ks_mat4_lookat(&m4, &v1, &v2, &v3);
    formatted = ks_mat4_tostr(&m4);
    ks_log(KSINFO, "%s with det=%f\n", ks_string_as_raw(&formatted), ks_mat4_det(&m4));
    ks_string_free(&formatted);

    return EXIT_SUCCESS;
}