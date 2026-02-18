#include <kitsune/core/log.h>
#include <kitsune/core/optional.h>
#include <kitsune/core/numbers.h>

int ksLogLevel = KSINFO;

ksOptional(i32);

int main(void) {
    ksLog(KSINFO, "Hello from example 2!");

    ksOpt_i32 some_i32 = ksOptSome_i32(5);
    ksOpt_i32 none_i32 = ksOptNone_i32();

    i32 v = ksOptUnwrap_i32(&some_i32);
    ksLog(KSINFO, "some_i32 has the value %d", v);

    i32 def_none = ksOptUnwrapOr_i32(&none_i32, 3);
    ksLog(KSINFO, "none_i32 defaulted to %d", def_none);

    ksOptUnwrap_i32(&none_i32);

    return EXIT_SUCCESS;
}
