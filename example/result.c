#define KS_CORE_IMPL

#include <ks/core.h>

KS_USING(int32_t, i32);
KS_RESULT(i32, i32);

int main(void) {
    ks_res_i32_i32 ok_i32 = ks_res_ok_i32_i32(5);
    ks_res_i32_i32 err_i32 = ks_res_err_i32_i32(-1);

    ks_print("ok_i32 is %s", ks_res_is_ok(&ok_i32) ? "Ok" : "Err");
    ks_print("err_i32 is %s", ks_res_is_ok(&err_i32) ? "Ok" : "Err");

    i32 ok = ks_res_get_ok(&ok_i32);
    ks_print("ok_i32 has the value %d", ok);

    i32 err = ks_res_get_err(&err_i32);
    ks_print("err_i32 has the value %d", err);

    return EXIT_SUCCESS;
}