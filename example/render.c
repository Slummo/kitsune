#define KS_RENDER_IMPL

#include <ks/core.h>
#include <ks/math.h>
#include <ks/render.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_log(KSINFO, "Render");
    ks_renderer_init(960, 540, "Render");

    // clang-format off
    const float tverts[] = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f 
    };
    
    const float sverts[] = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 
        -0.5f,  0.5f, 1.0f, 0.0f, 1.0f 
    };

    const uint8_t sinds[] = {0, 1, 2, 0, 2, 3};
    // clang-format on

    ks_mat3 models[10];
    ks_mat3 t = ks_mat3_idnew(), r = ks_mat3_idnew(), s = ks_mat3_idnew();

    ks_mesh tmesh = ks_mesh_create();
    ks_mesh_load_vertices(&tmesh, 3, tverts, &KS_VFMT_POS2_COL3);
    ks_shader tshader = ks_shader_create("example/assets/triangle.vert", "example/assets/triangle.frag");

    ks_mesh smesh = ks_mesh_create();
    ks_mesh_load_vertices(&smesh, 4, sverts, &KS_VFMT_POS2_COL3);
    ks_mesh_load_indices(&smesh, 6, sinds, &KS_IFMT_U8);
    ks_mesh_load_instances(&smesh, KS_ARR_SIZE(models), models, &KS_VFMT_INST_MAT3);
    ks_shader sshader = ks_shader_create("example/assets/square_inst.vert", "example/assets/square_inst.frag");

    while (!ks_win_should_close()) {
        for (uint32_t i = 0; i < KS_ARR_SIZE(models); ++i) {
            t = ks_mat3_idnew();
            r = ks_mat3_idnew();
            s = ks_mat3_idnew();
            double time = ks_gettime();
            float xoff = sinf(time + i);
            float scale = 0.2f + cosf(time) * 0.1f;
            ks_mat3_translate(&t, &KS_VEC2(xoff, 0.0f));
            ks_mat3_rotate(&r, time);
            ks_mat3_scale(&s, &KS_VEC2(scale, scale));
            models[i] = ks_mat3_model(&t, &r, &s);
        }
        ks_buffer_update(smesh.vbos.data[1], 0, sizeof(models), models);

        ks_begin_drawing();
        ks_mesh_draw(&tmesh, tshader);
        ks_mesh_draw(&smesh, sshader);
        ks_end_drawing();
    }

    ks_mesh_destroy(&tmesh);
    ks_renderer_destroy();

    return EXIT_SUCCESS;
}