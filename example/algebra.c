#define KS_CORE_IMPL
#define KS_MATH_IMPL

#include <ks/core.h>
#include <ks/math.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_vec3 a = ks_vec3_new(1.0f, 0.0f, 0.0f);
    ks_vec3 b = ks_vec3_new(0.0f, 1.0f, 0.0f);
    ks_vec3 c;
    ks_vec3_cross(&c, &a, &b);
    ks_vec3_muli(&c, 3.0f);

    ks_string formatted = ks_vec3_tostr(&c);
    printf("%s with length=%f\n", ks_string_as_raw(&formatted), ks_vec3_length(&c));

    ks_string_free(&formatted);
    return EXIT_SUCCESS;
}