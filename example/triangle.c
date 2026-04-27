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

    ks_renderer_init(640, 400, "Triangle");

    while (!ks_renderer_should_close()) {
        ks_renderer_begin_drawing();
        ks_renderer_end_drawing();
    }

    ks_renderer_destroy();

    return EXIT_SUCCESS;
}