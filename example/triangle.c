#define KS_CORE_IMPL
#define KS_MATH_IMPL
#define KS_IO_IMPL
#define KS_RENDER_IMPL

#include <ks/core.h>
#include <ks/math.h>
#include <ks/render.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_log(KSINFO, "Triangle");
    return EXIT_SUCCESS;
}