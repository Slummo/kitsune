#define KS_CORE_IMPL

#include <ks/core.h>

KS_USING(int32_t, i32);
KS_OPTIONAL(i32);

int main(void) {
    ks_opt_i32 some_i32 = ks_opt_some_i32(5);
    ks_opt_i32 none_i32 = ks_opt_none_i32();

    ks_print("some_i32 is %s", ks_opt_is_some(&some_i32) ? "Some" : "None");
    ks_print("none_i32 is %s", ks_opt_is_some(&none_i32) ? "Some" : "None");

    i32 v = ks_opt_unwrap_i32(&some_i32);
    ks_print("some_i32 has the value %d", v);

    i32 def_none = ks_opt_unwrap_or_i32(&none_i32, 3);
    ks_print("none_i32 defaulted to %d", def_none);

    ks_opt_unwrap_i32(&none_i32);

    return EXIT_SUCCESS;
}
