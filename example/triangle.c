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

    ks_renderable triangle;
    ks_renderable_init(&triangle);

    ks_shader s = ks_shader_create("example/triangle.vert", "example/triangle.frag");

    typedef struct vert {
        ks_vec2 pos;
        ks_col3 col;
    } vert;
    const vert vertices[3] = {
        {KS_VEC2(-0.5f, -0.5f), KS_COL3(1.0f, 0.0f, 0.0f)},  //
        {KS_VEC2(0.5f, -0.5f), KS_COL3(0.0f, 1.0f, 0.0f)},   //
        {KS_VEC2(0.0f, 0.5f), KS_COL3(0.0f, 0.0f, 1.0f)}     //
    };
    ks_buffer vbo = ks_buffer_create(GL_ARRAY_BUFFER, sizeof(vert) * 3, vertices, GL_STATIC_DRAW);

    ks_vert_attr attrs[2] = {
        {.loc = 0, .count = 2, .type = GL_FLOAT, .off = 0, .stride = sizeof(vert), .divisor = 0},
        {.loc = 1, .count = 3, .type = GL_FLOAT, .off = sizeof(ks_vec2), .stride = sizeof(vert), .divisor = 0}};

    ks_renderable_add_vbo(&triangle, vbo, attrs, 2);
    ks_renderable_set_counts(&triangle, 3, 0);

    while (!ks_renderer_should_close()) {
        ks_renderer_begin_drawing();
        ks_renderer_draw(&triangle, s);
        ks_renderer_end_drawing();
    }

    ks_renderable_destroy(&triangle);
    ks_buffer_destroy(vbo);
    ks_renderer_destroy();

    return EXIT_SUCCESS;
}