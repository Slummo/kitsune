#include <kitsune/core/log.h>
#include <kitsune/core/optional.h>
#include <kitsune/core/numbers.h>

int ks_log_level = KSINFO;

KS_OPTIONAL(i32);

int main(void) {
    ks_log(KSINFO, "Hello from example 2!");
    ks_opt_i32 some_i32 = ks_opt_some_i32(5);
    ks_opt_i32 none_i32 = ks_opt_none_i32();

    i32 v = ks_opt_unwrap_i32(&some_i32);
    ks_log(KSINFO, "some_i32 has the value %d", v);

    i32 def_none = ks_opt_unwrap_or_i32(&none_i32, 3);
    ks_log(KSINFO, "none_i32 defaulted to %d", def_none);

    ks_opt_unwrap_i32(&none_i32);

    return EXIT_SUCCESS;
}
