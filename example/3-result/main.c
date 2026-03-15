#include <kitsune/core/log.h>
#include <kitsune/core/result.h>
#include <kitsune/core/numbers.h>

int ks_log_level = KSINFO;

KS_RESULT(i32, i32);

int main(void) {
    ks_log(KSINFO, "Hello from example 3!");

    ks_res_i32_i32 ok_i32 = ks_res_ok_i32_i32(5);
    ks_res_i32_i32 err_i32 = ks_res_err_i32_i32(-1);

    i32 ok = ks_res_get_ok_i32_i32(&ok_i32);
    ks_log(KSINFO, "ok_i32 has the value %d", ok);

    i32 err = ks_res_get_err_i32_i32(&err_i32);
    ks_log(KSINFO, "err_i32 has the value %d", err);

    return EXIT_SUCCESS;
}
