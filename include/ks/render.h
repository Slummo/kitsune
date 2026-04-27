#pragma once

#include <ks/core.h>
#include <ks/math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

KS_STRUCT(color, {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
});

KS_STRUCT(shader, { uint32_t id; });

// TODO
ks_shader ks_shader_create(const char* vsrc, const char* fsrc);
void ks_shader_bind(ks_shader s);
void ks_shader_unbind(void);
void ks_shader_set_vec3(ks_shader s, const char* name, float* data);
void ks_shader_set_vec4(ks_shader s, const char* name, float* data);
void ks_shader_set_mat3(ks_shader s, const char* name, float* data);
void ks_shader_set_mat4(ks_shader s, const char* name, float* data);
void ks_shader_destroy(void);

KS_ENUM(buftype, {KS_BUF_VERTEX = 0, KS_BUF_INDEX});
KS_STRUCT(buffer, {
    uint32_t id;
    ks_buftype type;
});

// TODO
ks_buffer ks_buffer_create(ks_buftype type, size_t size, const void* data);
ks_buffer ks_buffer_update(ks_buffer b, size_t size, const void* data);
ks_buffer ks_buffer_bind(ks_buffer b);
ks_buffer ks_buffer_unbind(void);
ks_buffer ks_buffer_destroy(ks_buffer b);

// TODO
KS_STRUCT(texture, { uint32_t id; });

KS_STRUCT(vert_attr, {
    uint32_t loc;
    uint32_t count;  // number of floats in data type
    uint32_t type;   // data type code
    uint32_t off;
    uint32_t stride;
    uint32_t divisor;  // 0 = per-vertex, 1 = per-instance
});

#define KS_MAX_VBOS 8
#define KS_MAX_ATTR_PER_VBO 8

KS_STRUCT(renderable, {
    uint32_t vao;
    ks_buffer vbos[KS_MAX_VBOS];
    uint32_t vbo_count;
    ks_buffer ebo;
    int32_t has_indices;
    uint32_t vert_count;
    uint32_t ind_count;
    uint32_t inst_count;
});

// TODO
void ks_renderable_create(ks_renderable* r);
void ks_renderable_add_vbo(ks_renderable* r, ks_buffer b, const ks_vert_attr* attrs, uint32_t attr_count);
void ks_renderable_set_ebo(ks_renderable* r, ks_buffer b, uint32_t ind_count);
void ks_renderable_set_counts(ks_renderable* r, uint32_t vert_count, uint32_t ind_count);
void ks_renderable_destroy(ks_renderable* r);

KS_STRUCT(batcher, {
    uint8_t* vertices;
    uint32_t* indices;
    uint32_t vert_count;
    uint32_t ind_count;
    uint32_t max_verts;
    uint32_t max_inds;
    uint32_t stride;
    ks_renderable gpu_handle;
});

// TODO
void batcher_init(uint32_t max_verts, uint32_t max_inds, uint32_t stride, const ks_vert_attr* attributes,
                  uint32_t attr_count);
void batcher_next_vertex(void);
void batcher_push_index(uint32_t i);
void batcher_flush(void);

KS_UNUSED static ks_batcher g_batcher = {0};

KS_STRUCT(renderer, {
    int32_t width, height;
    ks_shader shader;
    bool is_drawing;
    GLFWwindow* win;
});

// TODO
void ks_renderer_init(int32_t width, int32_t height, const char* title);
void ks_renderer_draw(ks_renderable r, ks_shader s);
void ks_renderer_begin_drawing(void);
void ks_renderer_draw_line(ks_vec3 p1, ks_vec3 p2, float size, ks_color c);
void ks_renderer_draw_rect(ks_vec3 left, ks_vec3 dim, float line_size, ks_color c);
void ks_renderer_end_drawing(void);

#ifdef KS_RENDER_IMPL

/* shader */

ks_shader ks_shader_create(const char* vsrc, const char* fsrc);
void ks_shader_bind(ks_shader s);
void ks_shader_unbind(void);
void ks_shader_set_vec3(ks_shader s, const char* name, float* data);
void ks_shader_set_vec4(ks_shader s, const char* name, float* data);
void ks_shader_set_mat3(ks_shader s, const char* name, float* data);
void ks_shader_set_mat4(ks_shader s, const char* name, float* data);
void ks_shader_destroy(void);

/* buffer */

ks_buffer ks_buffer_create(ks_buftype type, size_t size, const void* data);
ks_buffer ks_buffer_update(ks_buffer b, size_t size, const void* data);
ks_buffer ks_buffer_bind(ks_buffer b);
ks_buffer ks_buffer_unbind(void);
ks_buffer ks_buffer_destroy(ks_buffer b);

/* texture*/

/* renderable */

void ks_renderable_create(ks_renderable* r);
void ks_renderable_add_vbo(ks_renderable* r, ks_buffer b, const ks_vert_attr* attrs, uint32_t attr_count);
void ks_renderable_set_ebo(ks_renderable* r, ks_buffer b, uint32_t ind_count);
void ks_renderable_set_counts(ks_renderable* r, uint32_t vert_count, uint32_t ind_count);
void ks_renderable_destroy(ks_renderable* r);

/* batcher */

void batcher_init(uint32_t max_verts, uint32_t max_inds, uint32_t stride, const ks_vert_attr* attributes,
                  uint32_t attr_count);
void batcher_next_vertex(void);
void batcher_push_index(uint32_t i);
void batcher_flush(void);

/* renderer */

void ks_renderer_init(int32_t width, int32_t height, const char* title);
void ks_renderer_draw(ks_renderable r, ks_shader s);
void ks_renderer_begin_drawing(void);
void ks_renderer_draw_line(ks_vec3 p1, ks_vec3 p2, float size, ks_color c);
void ks_renderer_draw_rect(ks_vec3 left, ks_vec3 dim, float line_size, ks_color c);
void ks_renderer_end_drawing(void);

#endif