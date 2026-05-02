#define KS_RENDER_IMPL

#include <ks/core.h>
#include <ks/math.h>
#include <ks/render.h>

int ks_log_level = KSINFO;

#define KS_WIN_WIDTH 960
#define KS_WIN_HEIGHT 540
#define KS_GRID_CELLSIZE 0.1
#define KS_WAVE_FREQ 0.5
#define KS_WAVE_SPEED 10.0

static void wave(ks_field_ctx* ctx, void* out) {
    float* o = out;
    ks_vec2 p = KS_VEC2(ctx->x, ctx->y);
    ks_vec2 c = KS_VEC2(ctx->dst->width / 2.0f, ctx->dst->height / 2.0f);
    float dist = ks_vec2_dist(&p, &c);
    float ripple = sinf(dist * KS_WAVE_FREQ - ctx->t * KS_WAVE_SPEED);
    float falloff = 1.0f / (1.0f * dist * 0.1f);
    *o = ripple * falloff;
}

static void gradient(ks_field_ctx* ctx, void* out) {
    ks_vec2* o = out;
    *o = ks_sf2_grad(ctx->src, ctx->x, ctx->y);
}

static void divergence(ks_field_ctx* ctx, void* out) {
    float* o = out;
    *o = ks_vf2_div(ctx->src, ctx->x, ctx->y);
}

KS_UNUSED static void curl(ks_field_ctx* ctx, void* out) {
    float* o = out;
    *o = ks_vf2_curl(ctx->src, ctx->x, ctx->y);
}

static void lap(ks_field_ctx* ctx, void* out) {
    float* o = out;
    *o = ks_sf2_lap(ctx->src, ctx->x, ctx->y);
}

int main(void) {
    ks_renderer_init(KS_WIN_WIDTH, KS_WIN_HEIGHT, "Fields");

    const ks_field_cb scalar_sample = wave;
    const ks_field_cb grad_sample = gradient;
    const ks_field_cb div_sample = divergence;
    const ks_field_cb lap_sample = lap;

    float time = ks_gettime();

    ks_field scalar_field = ks_field_create(50, 50, 1, sizeof(float), KS_GRID_CELLSIZE);
    ks_field_sample(&scalar_field, NULL, time, scalar_sample);

    ks_field grad_field = ks_field_create(50, 50, 1, sizeof(ks_vec2), KS_GRID_CELLSIZE);
    ks_field_sample(&grad_field, &scalar_field, time, grad_sample);

    ks_field div_field = ks_field_create(50, 50, 1, sizeof(float), KS_GRID_CELLSIZE);
    ks_field_sample(&div_field, &grad_field, time, div_sample);

    ks_field lap_field = ks_field_create(50, 50, 1, sizeof(float), KS_GRID_CELLSIZE);
    ks_field_sample(&lap_field, &scalar_field, time, lap_sample);

    // clang-format off
    const float rect_verts[] = {
         0.0f,  0.0f,
         1.0f,  0.0f,
         1.0f,  1.0f, 
         0.0f,  1.0f 
    };
    
    const float triangle_verts[] = {
        0.0f, -0.25f,
        0.0f,  0.25f,
        1.0f,  0.0f,
    };

    const uint8_t rect_inds[] = {0, 1, 2, 0, 2, 3};
    // clang-format on

    ks_mesh rect = ks_mesh_create();
    ks_mesh_load_vertices(&rect, 4, rect_verts, &KS_VFMT_POS2);
    ks_mesh_load_indices(&rect, 6, rect_inds, &KS_IFMT_U8);
    ks_mesh_load_instances(&rect, scalar_field.width * scalar_field.height, scalar_field.data, &KS_VFMT_INST_FLOAT);
    ks_shader rect_shader = ks_shader_create("example/assets/scalar.vert", "example/assets/scalar.frag");

    ks_mesh triangle = ks_mesh_create();
    ks_mesh_load_vertices(&triangle, 3, triangle_verts, &KS_VFMT_POS2);
    ks_mesh_load_instances(&triangle, grad_field.width * grad_field.height, grad_field.data, &KS_VFMT_INST_VEC2);
    ks_shader triangle_shader = ks_shader_create("example/assets/vector.vert", "example/assets/vector.frag");

    float grid_width = scalar_field.width * scalar_field.cellsize;
    float grid_height = scalar_field.height * scalar_field.cellsize;

    float min_val = -1.0f;
    float max_val = 1.0f;

    ks_mat4 rect_view;
    ks_mat4_lookat(&rect_view, &KS_VEC3(0.0f, 0.0f, 3.0f), &KS_VEC3(0.0f, 0.0f, 0.0f), &KS_VEC3(0.0f, 1.0f, 0.0f));
    ks_mat4 rect_proj;
    ks_mat4_ortho(&rect_proj, 0.0f, grid_width, 0.0f, grid_height, 0.1f, 100.0f);
    ks_shader_bind(rect_shader);
    ks_shader_set_int(rect_shader, "uGridWidth", &scalar_field.width);
    ks_shader_set_float(rect_shader, "uCellSize", &scalar_field.cellsize);
    ks_shader_set_mat4(rect_shader, "uView", ks_mat4_ptr(&rect_view));
    ks_shader_set_mat4(rect_shader, "uProj", ks_mat4_ptr(&rect_proj));
    ks_shader_set_float(rect_shader, "uMinVal", &min_val);
    ks_shader_set_float(rect_shader, "uMaxVal", &max_val);

    grid_width = grad_field.width * grad_field.cellsize;
    grid_height = grad_field.height * grad_field.cellsize;

    ks_mat4 triangle_view;
    ks_mat4_lookat(&triangle_view, &KS_VEC3(0.0f, 0.0f, 3.0f), &KS_VEC3(0.0f, 0.0f, 0.0f), &KS_VEC3(0.0f, 1.0f, 0.0f));
    ks_mat4 triangle_proj;
    ks_mat4_ortho(&triangle_proj, 0.0f, grid_width, 0.0f, grid_height, 0.1f, 100.0f);
    ks_shader_bind(triangle_shader);
    ks_shader_set_int(triangle_shader, "uGridWidth", &grad_field.width);
    ks_shader_set_float(triangle_shader, "uCellSize", &grad_field.cellsize);
    ks_shader_set_mat4(triangle_shader, "uView", ks_mat4_ptr(&triangle_view));
    ks_shader_set_mat4(triangle_shader, "uProj", ks_mat4_ptr(&triangle_proj));

    const int32_t fw = KS_WIN_WIDTH / 2;
    const int32_t fh = KS_WIN_HEIGHT / 2;

    while (!ks_win_should_close()) {
        time = ks_gettime();

        ks_field_sample(&scalar_field, NULL, time, scalar_sample);
        ks_mesh_update_instances(&rect, scalar_field.width * scalar_field.height, scalar_field.data,
                                 &KS_VFMT_INST_FLOAT);
        ks_field_sample(&grad_field, &scalar_field, time, grad_sample);
        ks_mesh_update_instances(&triangle, grad_field.width * grad_field.height, grad_field.data, &KS_VFMT_INST_VEC2);

        ks_begin_drawing();
        ks_background(KS_COL4(0.1f, 0.1f, 0.1f, 1.0f));

        // Wave scalar field
        ks_drawbox(0, KS_WIN_HEIGHT / 2, fw, fh);
        ks_mesh_draw(&rect, rect_shader);

        // Gradient vector field
        ks_drawbox(KS_WIN_WIDTH / 2, KS_WIN_HEIGHT / 2, fw, fh);
        ks_mesh_draw(&triangle, triangle_shader);

        // Divergence scalar field
        ks_drawbox(0, 0, fw, fh);
        ks_field_sample(&div_field, &grad_field, time, div_sample);
        ks_mesh_update_instances(&rect, div_field.width * div_field.height, div_field.data, &KS_VFMT_INST_FLOAT);
        ks_mesh_draw(&rect, rect_shader);

        // Laplacian scalar field
        ks_drawbox(KS_WIN_WIDTH / 2, 0, fw, fh);
        ks_field_sample(&lap_field, &scalar_field, time, lap_sample);
        ks_mesh_update_instances(&rect, lap_field.width * lap_field.height, lap_field.data, &KS_VFMT_INST_FLOAT);
        ks_mesh_draw(&rect, rect_shader);

        ks_drawbox_reset();
        ks_end_drawing();
    }

    ks_mesh_destroy(&rect);
    ks_mesh_destroy(&triangle);
    ks_field_destroy(&scalar_field);
    ks_field_destroy(&lap_field);
    ks_field_destroy(&div_field);
    ks_field_destroy(&grad_field);
    ks_renderer_destroy();

    return EXIT_SUCCESS;
}