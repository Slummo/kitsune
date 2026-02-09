#include <kitsune/core/log.h>
#include <kitsune/core/optional.h>

int kitlog_level = KITINFO;

kit_optional(int);

int main(void) {
    kit_log(KITINFO, "Hello from example 2!");

    kitOpt(int) some_int = kit_opt_int_some(5);
    kitOpt(int) none_int = kit_opt_int_none();

    int v = kit_opt_int_unwrap(&some_int);
    kit_log(KITINFO, "some_int has the value %d", v);

    int def_none = kit_opt_int_unwrap_or(&none_int, 3);
    kit_log(KITINFO, "none_int defaulted to %d", def_none);

    return EXIT_SUCCESS;
}