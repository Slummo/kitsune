#include <kitsune/core/log.h>
#include <kitsune/core/result.h>
#include <kitsune/core/numbers.h>

int ksLogLevel = KSINFO;

ksResult(i32, i32);

int main(void) {
    ksLog(KSINFO, "Hello from example 3!");

    ksRes_i32_i32 ok_i32 = ksResOk_i32_i32(5);
    ksRes_i32_i32 err_i32 = ksResErr_i32_i32(-1);

    i32 ok = ksResGetOk_i32_i32(&ok_i32);
    ksLog(KSINFO, "ok_i32 has the value %d", ok);

    i32 err = ksResGetErr_i32_i32(&err_i32);
    ksLog(KSINFO, "err_i32 has the value %d", err);

    return EXIT_SUCCESS;
}
