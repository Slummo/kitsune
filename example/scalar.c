#define KS_RENDER_IMPL

#include <ks/core.h>
#include <ks/math.h>
#include <ks/render.h>

int ks_log_level = KSINFO;

#define KS_GRID_WIDTH 100
#define KS_GRID_HEIGHT 100
#define KS_GRID_CELLSIZE 0.1
#define KS_WAVE_FREQ 0.5
#define KS_WAVE_SPEED 10.0

static double g_time;

float wave(float x, float y) {
    float cx = KS_GRID_WIDTH / 2.0f;
    float cy = KS_GRID_HEIGHT / 2.0f;

    float dx = x - cx;
    float dy = y - cy;
    float dist = sqrtf(dx * dx + dy * dy);

    float ripple = sinf(dist * KS_WAVE_FREQ - g_time * KS_WAVE_SPEED);
    float falloff = 1.0f / (1.0f + dist * 0.1f);
    return ripple * falloff;
}

int main(void) {
    ks_log(KSINFO, "Scalar");
    ks_renderer_init(960, 540, "Scalar");

    float (*sample)(float x, float y) = wave;

    ks_sf2 sf = ks_sf2_create(KS_GRID_WIDTH, KS_GRID_HEIGHT, KS_GRID_CELLSIZE);
    ks_sf2_sample(&sf, sample);

    // clang-format off
    const float verts[] = {
         0.0f,  0.0f,
         1.0f,  0.0f,
         1.0f,  1.0f, 
         0.0f,  1.0f 
    };

    const uint8_t inds[] = {0, 1, 2, 0, 2, 3};
    // clang-format on

    ks_mesh quad = ks_mesh_create();
    ks_mesh_load_vertices(&quad, 4, verts, &KS_VFMT_POS2);
    ks_mesh_load_indices(&quad, 6, inds, &KS_IFMT_U8);
    ks_mesh_load_instances(&quad, (uint32_t)(sf.width * sf.height), sf.data, &KS_VFMT_INST_FLOAT);
    ks_shader shader = ks_shader_create("example/assets/scalar.vert", "example/assets/scalar.frag");

    float grid_width = sf.width * sf.cellsize;
    float grid_height = sf.height * sf.cellsize;
    float max_val = sf.width * sf.width + sf.height * sf.height;

    ks_mat4 view;
    ks_mat4_lookat(&view, &KS_VEC3(0.0f, 0.0f, 3.0f), &KS_VEC3(0.0f, 0.0f, 0.0f), &KS_VEC3(0.0f, 1.0f, 0.0f));
    ks_mat4 proj;
    ks_mat4_ortho(&proj, 0.0f, grid_width, 0.0f, grid_height, 0.1f, 100.0f);
    ks_shader_bind(shader);
    ks_shader_set_int(shader, "uGridWidth", &sf.width);
    ks_shader_set_float(shader, "uCellSize", &sf.cellsize);
    ks_shader_set_mat4(shader, "uView", ks_mat4_ptr(&view));
    ks_shader_set_mat4(shader, "uProj", ks_mat4_ptr(&proj));
    ks_shader_set_float(shader, "uMaxVal", &max_val);

    while (!ks_win_should_close()) {
        g_time = ks_gettime();
        ks_sf2_sample(&sf, sample);
        ks_mesh_update_instances(&quad, (uint32_t)(sf.width * sf.height), sf.data, &KS_VFMT_INST_FLOAT);

        ks_begin_drawing();
        ks_mesh_draw(&quad, shader);
        ks_end_drawing();
    }

    ks_mesh_destroy(&quad);
    ks_sf2_destroy(&sf);
    ks_renderer_destroy();

    return EXIT_SUCCESS;
}