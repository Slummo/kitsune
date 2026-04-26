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
    printf("%s with length=%f\n", ks_string_as_raw(&formatted), ks_vec3_length(&v3));
    ks_string_free(&formatted);

    ks_mat3 m1 = ks_mat3_id();
    ks_mat3 m2 = ks_mat3_id();
    ks_mat3_smuli(&m1, 0.5f);
    ks_mat3_sumi(&m2, &m1);

    formatted = ks_mat3_tostr(&m2);
    printf("%s\n", ks_string_as_raw(&formatted));
    ks_string_free(&formatted);

    return EXIT_SUCCESS;
}