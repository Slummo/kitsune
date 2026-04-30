#define KS_RENDER_IMPL

#include <ks/core.h>
#include <ks/math.h>
#include <ks/render.h>

int ks_log_level = KSINFO;

#define WIN_WIDTH 960
#define WIN_HEIGHT 540
#define KS_GRID_WIDTH 20
#define KS_GRID_HEIGHT 20
#define KS_GRID_CELLSIZE 0.1
#define KS_WAVE_FREQ 0.5
#define KS_WAVE_SPEED 10.0

static double g_time;

ks_vec2 source(float x, float y) {
    float cx = KS_GRID_WIDTH / 2.0f;
    float cy = KS_GRID_HEIGHT / 2.0f;

    float dx = x - cx;
    float dy = y - cy;

    return KS_VEC2(dx * 0.5f, dy * 0.5f);
}

ks_vec2 sink(float x, float y) {
    float cx = KS_GRID_WIDTH / 2.0f;
    float cy = KS_GRID_HEIGHT / 2.0f;

    float dx = x - cx;
    float dy = y - cy;

    return KS_VEC2(-dx * 0.5f, -dy * 0.5f);
}

ks_vec2 pulse(float x, float y) {
    float cx = KS_GRID_WIDTH / 2.0f;
    float cy = KS_GRID_HEIGHT / 2.0f;

    float dx = x - cx;
    float dy = y - cy;

    float pulse = sinf(g_time * 2.0f) * 0.5f;

    return KS_VEC2(dx * pulse, dy * pulse);
}

ks_vec2 vortex(float x, float y) {
    float orbitr = 5.0f;
    float cx = KS_GRID_WIDTH / 2.0f + cosf(g_time) * orbitr;
    float cy = KS_GRID_HEIGHT / 2.0f + sinf(g_time) * orbitr;

    float dx = x - cx;
    float dy = y - cy;

    return KS_VEC2(-dy * 0.5f, dx * 0.5f);
}

int main(void) {
    ks_log(KSINFO, "Vector");
    ks_renderer_init(WIN_WIDTH, WIN_HEIGHT, "Vector");

    ks_vec2 (*sample)(float x, float y) = vortex;

    ks_vf2 vf = ks_vf2_create(KS_GRID_WIDTH, KS_GRID_HEIGHT, KS_GRID_CELLSIZE);
    ks_vf2_sample(&vf, sample);

    // clang-format off
    const float verts[] = {
        0.0f, -0.25f,
        0.0f,  0.25f,
        1.0f,  0.0f,
    };
    // clang-format on

    ks_mesh arrow = ks_mesh_create();
    ks_mesh_load_vertices(&arrow, 3, verts, &KS_VFMT_POS2);
    ks_mesh_load_instances(&arrow, (uint32_t)(vf.width * vf.height), vf.data, &KS_VFMT_INST_VEC2);
    ks_shader shader = ks_shader_create("example/assets/vector.vert", "example/assets/vector.frag");

    float grid_width = vf.width * vf.cellsize;
    float grid_height = vf.height * vf.cellsize;

    ks_mat4 view;
    ks_mat4_lookat(&view, &KS_VEC3(0.0f, 0.0f, 3.0f), &KS_VEC3(0.0f, 0.0f, 0.0f), &KS_VEC3(0.0f, 1.0f, 0.0f));
    ks_mat4 proj;
    ks_mat4_ortho(&proj, 0.0f, grid_width, 0.0f, grid_height, 0.1f, 100.0f);
    ks_shader_bind(shader);
    ks_shader_set_int(shader, "uGridWidth", &vf.width);
    ks_shader_set_float(shader, "uCellSize", &vf.cellsize);
    ks_shader_set_mat4(shader, "uView", ks_mat4_ptr(&view));
    ks_shader_set_mat4(shader, "uProj", ks_mat4_ptr(&proj));

    while (!ks_win_should_close()) {
        g_time = ks_gettime();
        ks_vf2_sample(&vf, sample);
        ks_mesh_update_instances(&arrow, (uint32_t)(vf.width * vf.height), vf.data, &KS_VFMT_INST_VEC2);

        ks_begin_drawing();
        ks_mesh_draw(&arrow, shader);
        ks_end_drawing();
    }

    ks_mesh_destroy(&arrow);
    ks_vf2_destroy(&vf);
    ks_renderer_destroy();

    return EXIT_SUCCESS;
}