#include <kitsune/core/log.h>
#include <kitsune/core/optional.h>

int ksLogLevel = KSINFO;

ksOptional(int);

int main(void) {
    ksLog(KSINFO, "Hello from example 2!");

    ksOpt_int some_int = ksOpt_int_some(5);
    ksOpt_int none_int = ksOpt_int_none();

    int v = ksOpt_int_unwrap(&some_int);
    ksLog(KSINFO, "some_int has the value %d", v);

    int def_none = ksOpt_int_unwrap_or(&none_int, 3);
    ksLog(KSINFO, "none_int defaulted to %d", def_none);

    ksOpt_int_unwrap(&none_int);

    return EXIT_SUCCESS;
}