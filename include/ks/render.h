#ifndef KS_RENDER_H
#define KS_RENDER_H

#include <ks/core.h>
#include <ks/math.h>
#include <ks/ds.h>
#include <signal.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

KS_STRUCT(col3, {
    float r;
    float g;
    float b;
});

#define KS_COL3(r, g, b) \
    (ks_col3) {          \
        r, g, b          \
    }

KS_STRUCT(col4, {
    float r;
    float g;
    float b;
    float a;
});

#define KS_COL4(r, g, b, a) \
    (ks_col4) {             \
        r, g, b, a          \
    }

/* Shader */

KS_STRUCT(shader, { uint32_t id; });

ks_shader ks_shader_create(const char* vert_filename, const char* frag_filename);
void ks_shader_bind(ks_shader s);
void ks_shader_unbind(void);
void ks_shader_set_int(ks_shader s, const char* name, int* data);
void ks_shader_set_float(ks_shader s, const char* name, float* data);
void ks_shader_set_vec2(ks_shader s, const char* name, float* data);
void ks_shader_set_vec3(ks_shader s, const char* name, float* data);
void ks_shader_set_vec4(ks_shader s, const char* name, float* data);
void ks_shader_set_mat3(ks_shader s, const char* name, float* data);
void ks_shader_set_mat4(ks_shader s, const char* name, float* data);
void ks_shader_destroy(ks_shader s);

/* Buffer */

KS_STRUCT(buffer, {
    uint32_t id;
    uint32_t type;
});

ks_buffer ks_buffer_create(uint32_t type, size_t size, const void* data, uint32_t usage);
void ks_buffer_update(ks_buffer b, size_t off, size_t size, const void* data);
void ks_buffer_bind(ks_buffer b);
void ks_buffer_unbind(uint32_t type);
void ks_buffer_destroy(ks_buffer b);

/* Texture */

// TODO
KS_STRUCT(texture, { uint32_t id; });

/* Vertex attribute and format */

KS_STRUCT(vattr, {
    uint32_t loc;
    int32_t count;  // number of floats in data type
    uint32_t type;  // data type code
    uint32_t off;
    int32_t stride;
    uint32_t divisor;  // 0 = per-vertex, 1 = per-instance
});

#define KS_MAX_VATTRS_PER_VBO 8
KS_STRUCT(vfmt, {
    KS_SA_UNNAMED(ks_vattr, KS_MAX_VATTRS_PER_VBO) attrs;
    uint32_t vsize;
});

extern const ks_vfmt KS_VFMT_POS2;
extern const ks_vfmt KS_VFMT_POS3;
extern const ks_vfmt KS_VFMT_POS2_COL3;
extern const ks_vfmt KS_VFMT_POS3_COL3;
extern const ks_vfmt KS_VFMT_INST_FLOAT;
extern const ks_vfmt KS_VFMT_INST_VEC2;
extern const ks_vfmt KS_VFMT_INST_VEC3;
extern const ks_vfmt KS_VFMT_INST_MAT3;
extern const ks_vfmt KS_VFMT_INST_MAT4;

/* Index format */
KS_STRUCT(ifmt, {
    uint32_t type;    // data type code
    uint32_t ixsize;  // size of data type
});

extern const ks_ifmt KS_IFMT_U8;
extern const ks_ifmt KS_IFMT_U16;
extern const ks_ifmt KS_IFMT_U32;

/* Mesh */

#define KS_MAX_VBOS_PER_MESH 8
KS_STRUCT(mesh, {
    uint32_t vao;
    KS_SA_UNNAMED(ks_buffer, KS_MAX_VBOS_PER_MESH) vbos;
    ks_buffer ebo;
    bool has_indices;
    ks_buffer inst_vbo;
    bool has_instances;
    uint32_t ixtype;
    int32_t vcount;
    int32_t ixcount;
    int32_t iecount;
    uint32_t next_loc;
});

ks_mesh ks_mesh_create(void);
void ks_mesh_load_vertices(ks_mesh* m, int32_t vcount, const void* verts, const ks_vfmt* vfmt);
void ks_mesh_load_indices(ks_mesh* m, int32_t ixcount, const void* inds, const ks_ifmt* ifmt);
void ks_mesh_load_instances(ks_mesh* m, int32_t iecount, const void* insts, const ks_vfmt* vfmt);
void ks_mesh_update_instances(ks_mesh* m, int32_t iecount, const void* insts, const ks_vfmt* vfmt);
void ks_mesh_draw(ks_mesh* m, ks_shader s);
void ks_mesh_destroy(ks_mesh* m);

/* Batcher */

KS_STRUCT(batcher, {
    void* vertices;
    void* indices;
    int32_t vcount;
    int32_t ixcount;
    int32_t max_verts;
    int32_t max_inds;
    int32_t stride;
    ks_mesh gpu_handle;
});

void ks_batcher_init(int32_t max_verts, int32_t max_inds, int32_t stride, const ks_vattr* attributes,
                     int32_t attr_count);
void ks_batcher_next_vertex(void);
void ks_batcher_push_index(uint32_t i);
void ks_batcher_flush(void);

/* Renderer */

KS_FUNC(void, win_resize, int32_t width, int32_t height);

KS_STRUCT(renderer, {
    int32_t width, height;
    ks_shader shader;
    bool is_drawing;
    GLFWwindow* win;
    ks_batcher batcher;
    win_resize resize_cb;
    uint32_t primitive;
});

extern ks_renderer g_renderer;

void ks_renderer_init(int32_t width, int32_t height, const char* title);
bool ks_win_should_close(void);
void ks_win_resize_cb(win_resize cb);
ks_vec2 ks_win_size(void);
double ks_gettime(void);
void ks_primitive(uint32_t type);
void ks_drawbox(int32_t x, int32_t y, int32_t w, int32_t h);
void ks_drawbox_reset(void);
void ks_background(ks_col4 col);
void ks_wiremode(bool state);
void ks_line_width(float width);
void ks_begin_drawing(void);
void ks_draw_line(ks_vec3 p1, ks_vec3 p2, float size, ks_col4 c);
void ks_draw_rect(ks_vec3 left, ks_vec3 dim, float line_size, ks_col4 c);
void ks_end_drawing(void);
void ks_renderer_destroy(void);

#endif  // KS_RENDER_H

#if defined(KS_RENDER_IMPL) && !defined(KS_RENDER_IMPL_DONE)
#define KS_RENDER_IMPL_DONE

#ifndef KS_CORE_IMPL_DONE
#define KS_CORE_IMPL
#include <ks/core.h>
#endif

#ifndef KS_MATH_IMPL_DONE
#define KS_MATH_IMPL
#include <ks/math.h>
#endif

#ifndef KS_DS_IMPL_DONE
#define KS_DS_IMPL
#include <ks/ds.h>
#endif

#ifndef KS_IO_IMPL_DONE
#define KS_IO_IMPL
#include <ks/io.h>
#endif

/* Shader */

static GLuint compile_shader(GLenum type, const char* source) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &source, NULL);
    glCompileShader(s);

    int ret;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        char buf[1024];
        glGetShaderInfoLog(s, 1024, NULL, buf);
        fprintf(stderr, "%s", buf);
        return 0;
    }

    return s;
}

ks_shader ks_shader_create(const char* vert_filename, const char* frag_filename) {
    ks_shader s = {0};

    char* vert_src = ks_fread(vert_filename);
    char* frag_src = ks_fread(frag_filename);

    if (!vert_src || !frag_src) {
        ks_log(KSERR, "Error while reading from shader sources");
        free(vert_src);
        free(frag_src);
        return s;
    }

    GLuint vertex = compile_shader(GL_VERTEX_SHADER, vert_src);
    if (!vertex) {
        ks_log(KSERR, "Error while compiling vertex shader");
        free(vert_src);
        free(frag_src);
        return s;
    }

    GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, frag_src);
    if (!fragment) {
        ks_log(KSERR, "Error while compiling fragment shader");
        free(vert_src);
        free(frag_src);
        return s;
    }

    free(vert_src);
    free(frag_src);

    GLuint p = glCreateProgram();
    glAttachShader(p, vertex);
    glAttachShader(p, fragment);
    glLinkProgram(p);

    int ret;
    glGetProgramiv(p, GL_LINK_STATUS, &ret);
    if (!ret) {
        char buf[1024];
        glGetProgramInfoLog(p, 1024, NULL, buf);
        fprintf(stderr, "%s", buf);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(p);
        return s;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    s.id = p;
    return s;
}

void ks_shader_bind(ks_shader s) {
    glUseProgram(s.id);
}

void ks_shader_unbind(void) {
    glUseProgram(0);
}

void ks_shader_set_int(ks_shader s, const char* name, int* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniform1iv(loc, 1, data);
}

void ks_shader_set_float(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniform1fv(loc, 1, data);
}

void ks_shader_set_vec2(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniform2fv(loc, 1, data);
}

void ks_shader_set_vec3(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniform3fv(loc, 1, data);
}

void ks_shader_set_vec4(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniform4fv(loc, 1, data);
}

void ks_shader_set_mat3(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniformMatrix3fv(loc, 1, GL_FALSE, data);
}

void ks_shader_set_mat4(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != -1, "Uniform not found");
    glUniformMatrix4fv(loc, 1, GL_FALSE, data);
}

void ks_shader_destroy(ks_shader s) {
    glDeleteProgram(s.id);
}

/* Buffer */

ks_buffer ks_buffer_create(uint32_t type, size_t size, const void* data, uint32_t usage) {
    ks_buffer b = {.id = 0, .type = type};
    glGenBuffers(1, &b.id);
    ks_buffer_bind(b);
    glBufferData(b.type, (GLsizeiptr)size, data, usage);
    return b;
}

void ks_buffer_update(ks_buffer b, size_t off, size_t size, const void* data) {
    ks_buffer_bind(b);
    glBufferSubData(b.type, (GLintptr)off, (GLsizeiptr)size, data);
}

void ks_buffer_bind(ks_buffer b) {
    glBindBuffer(b.type, b.id);
}

void ks_buffer_unbind(uint32_t type) {
    glBindBuffer(type, 0);
}

void ks_buffer_destroy(ks_buffer b) {
    glDeleteBuffers(1, &b.id);
}

/* Texture*/

/* Vertex format */

// clang-format off
const ks_vfmt KS_VFMT_POS2 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 2, GL_FLOAT, 0, sizeof(ks_vec2), 0},
        },
        .len = 1
    },
    .vsize = sizeof(ks_vec2)
};

const ks_vfmt KS_VFMT_POS3 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 3, GL_FLOAT, 0, sizeof(ks_vec3), 0},
        },
        .len = 1
    },
    .vsize = sizeof(ks_vec3)
};

const ks_vfmt KS_VFMT_POS2_COL3 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 2, GL_FLOAT, 0               , sizeof(ks_vec2) + sizeof(ks_col3), 0},
            {1, 3, GL_FLOAT, sizeof(ks_vec2) , sizeof(ks_vec2) + sizeof(ks_col3), 0}
        },
        .len = 2
    },
    .vsize = sizeof(ks_vec2) + sizeof(ks_col3)
};

const ks_vfmt KS_VFMT_POS3_COL3 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 3, GL_FLOAT, 0               , sizeof(ks_vec3) + sizeof(ks_col3), 0},
            {1, 3, GL_FLOAT, sizeof(ks_vec3) , sizeof(ks_vec3) + sizeof(ks_col3), 0}
        },
        .len = 2
    },
    .vsize = sizeof(ks_vec3) + sizeof(ks_col3)
};

const ks_vfmt KS_VFMT_INST_FLOAT = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 1, GL_FLOAT, 0, sizeof(float), 1}
        },
        .len = 1
    },
    .vsize = sizeof(float)
};

const ks_vfmt KS_VFMT_INST_VEC2 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 2, GL_FLOAT, 0, sizeof(ks_vec2), 1}
        },
        .len = 1
    },
    .vsize = sizeof(ks_vec2)
};

const ks_vfmt KS_VFMT_INST_VEC3 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 3, GL_FLOAT, 0, sizeof(ks_vec3), 1}
        },
        .len = 1
    },
    .vsize = sizeof(ks_vec3)
};

const ks_vfmt KS_VFMT_INST_MAT3 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 3, GL_FLOAT, 0,                   sizeof(ks_mat3), 1},
            {1, 3, GL_FLOAT, 1 * sizeof(ks_vec3), sizeof(ks_mat3), 1},
            {2, 3, GL_FLOAT, 2 * sizeof(ks_vec3), sizeof(ks_mat3), 1}
        },
        .len = 3
    },
    .vsize = sizeof(ks_mat3)
};

const ks_vfmt KS_VFMT_INST_MAT4 = {
    .attrs = {
        .data = {
            // loc, count, type, offset, stride, divisor
            {0, 4, GL_FLOAT, 0,                   sizeof(ks_mat4), 1},
            {1, 4, GL_FLOAT, 1 * sizeof(ks_vec4), sizeof(ks_mat4), 1},
            {2, 4, GL_FLOAT, 2 * sizeof(ks_vec4), sizeof(ks_mat4), 1},
            {3, 4, GL_FLOAT, 3 * sizeof(ks_vec4), sizeof(ks_mat4), 1} 
        },
        .len = 4
    },
    .vsize = sizeof(ks_mat4)
};

/* Index format */
                            // type, ixsize
const ks_ifmt KS_IFMT_U8 = {GL_UNSIGNED_BYTE , sizeof(uint8_t)};
const ks_ifmt KS_IFMT_U16 = {GL_UNSIGNED_SHORT, sizeof(uint16_t)};
const ks_ifmt KS_IFMT_U32 = {GL_UNSIGNED_INT  , sizeof(uint32_t)};

// clang-format on

/* Mesh */

static void _ks_mesh_init_vbo_internal(ks_mesh* m, ks_buffer b, const ks_vfmt* vfmt) {
    KS_ASSERT_NONNULL_ARGS(m && vfmt);
    if (ks_sa_isfull(&m->vbos)) {
        return;
    }

    glBindVertexArray(m->vao);
    ks_buffer_bind(b);

    ks_array_foreach(attr, &vfmt->attrs) {
        uint32_t loc = m->next_loc + attr->loc;
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, attr->count, attr->type, GL_FALSE, attr->stride,
                              (const void*)(uintptr_t)(attr->off));
        if (attr->divisor > 0) {
            glVertexAttribDivisor(loc, attr->divisor);
        }
    }

    m->next_loc += vfmt->attrs.len;

    glBindVertexArray(0);
}

ks_mesh ks_mesh_create(void) {
    ks_mesh m = {0};

    glGenVertexArrays(1, &m.vao);
    m.ixtype = GL_UNSIGNED_INT;

    return m;
}

void ks_mesh_load_vertices(ks_mesh* m, int32_t vcount, const void* verts, const ks_vfmt* vfmt) {
    KS_ASSERT_NONNULL_ARGS(m && verts && vfmt);
    glBindVertexArray(m->vao);
    ks_buffer vbo = ks_buffer_create(GL_ARRAY_BUFFER, (size_t)vcount * vfmt->vsize, verts, GL_STATIC_DRAW);
    _ks_mesh_init_vbo_internal(m, vbo, vfmt);
    ks_sa_push(&m->vbos, vbo);

    m->vcount = vcount;
    m->iecount = 1;
}

void ks_mesh_load_indices(ks_mesh* m, int32_t ixcount, const void* inds, const ks_ifmt* ifmt) {
    KS_ASSERT_NONNULL_ARGS(m && inds && ifmt);
    glBindVertexArray(m->vao);
    ks_buffer ebo = ks_buffer_create(GL_ELEMENT_ARRAY_BUFFER, (size_t)ixcount * ifmt->ixsize, inds, GL_STATIC_DRAW);
    ks_buffer_bind(ebo);

    m->ebo = ebo;
    m->has_indices = true;
    m->ixtype = ifmt->type;
    m->ixcount = ixcount;

    glBindVertexArray(0);
}

void ks_mesh_load_instances(ks_mesh* m, int32_t iecount, const void* insts, const ks_vfmt* vfmt) {
    KS_ASSERT_NONNULL_ARGS(m && insts && vfmt);
    glBindVertexArray(m->vao);
    ks_buffer vbo = ks_buffer_create(GL_ARRAY_BUFFER, (size_t)iecount * vfmt->vsize, insts, GL_DYNAMIC_DRAW);
    _ks_mesh_init_vbo_internal(m, vbo, vfmt);
    ks_sa_push(&m->vbos, vbo);

    m->inst_vbo = vbo;
    m->has_instances = true;
    m->iecount = iecount;
}

void ks_mesh_update_instances(ks_mesh* m, int32_t iecount, const void* insts, const ks_vfmt* vfmt) {
    KS_ASSERT_NONNULL_ARGS(m && insts && vfmt);
    KS_ASSERT(m->has_instances, "No instance buffer initialized for this mesh");
    glBindVertexArray(m->vao);
    ks_buffer_update(m->inst_vbo, 0, (size_t)iecount * vfmt->vsize, insts);
    glBindVertexArray(0);
}

void ks_mesh_draw(ks_mesh* m, ks_shader s) {
    KS_ASSERT_NONNULL_ARGS(m);

    g_renderer.is_drawing = true;

    ks_shader_bind(s);
    glBindVertexArray(m->vao);

    if (m->has_indices) {
        if (m->iecount > 1) {
            glDrawElementsInstanced(GL_TRIANGLES, m->ixcount, m->ixtype, NULL, m->iecount);
        } else {
            glDrawElements(GL_TRIANGLES, m->ixcount, m->ixtype, NULL);
        }
    } else {
        if (m->iecount > 1) {
            glDrawArraysInstanced(GL_TRIANGLES, 0, m->vcount, m->iecount);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, m->vcount);
        }
    }

    glBindVertexArray(0);

    g_renderer.is_drawing = false;
}

void ks_mesh_destroy(ks_mesh* m) {
    KS_ASSERT_NONNULL_ARGS(m);
    glDeleteVertexArrays(1, &m->vao);
    // Don't destroy buffers to allow sharing between renderables
}

/* Batcher */

void ks_batcher_init(int32_t max_verts, int32_t max_inds, int32_t stride, const ks_vattr* attrs, int32_t attr_count);
void ks_batcher_next_vertex(void);
void ks_batcher_push_index(uint32_t i);

void ks_batcher_flush(void) {
    return;
}

/* Renderer */

static void GLAPIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, KS_UNUSED GLsizei length,
                                      const GLchar* message, KS_UNUSED const void* user_param) {
    fprintf(stderr, "[GL_DEBUG] source=0x%x type=0x%x id=%u severity=0x%x\n\t%s\n", source, type, id, severity,
            message);

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        raise(SIGTRAP);
    }
}

static void framebuffer_size_callback(KS_UNUSED GLFWwindow* window, int32_t width, int32_t height) {
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    g_renderer.width = width;
    g_renderer.height = height;
    if (g_renderer.resize_cb) {
        g_renderer.resize_cb(width, height);
    }
}

ks_renderer g_renderer = {0};

void ks_renderer_init(int32_t width, int32_t height, const char* title) {
    // Initialize GLFW
    if (!glfwInit()) {
        ks_log(KSERR, "Error with glfwInit");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        ks_log(KSERR, "Error with glfwCreateWindow");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ks_log(KSERR, "Error with gladLoadGLLoader");
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    // Set debug callback
    if (GLAD_GL_KHR_debug || GLAD_GL_VERSION_4_3) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debug_callback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    g_renderer.width = width;
    g_renderer.height = height;
    g_renderer.is_drawing = false;
    g_renderer.win = window;
    g_renderer.primitive = GL_TRIANGLES;
    // TODO: init batcher
}

bool ks_win_should_close(void) {
    return glfwWindowShouldClose(g_renderer.win);
}

void ks_win_resize_cb(win_resize cb) {
    g_renderer.resize_cb = cb;
}

ks_vec2 ks_win_size(void) {
    return KS_VEC2(g_renderer.width, g_renderer.height);
}

double ks_gettime(void) {
    return glfwGetTime();
}

void ks_primitive(uint32_t type) {
    g_renderer.primitive = type;
}

void ks_drawbox(int32_t x, int32_t y, int32_t w, int32_t h) {
    glViewport(x, y, w, h);
    glScissor(x, y, w, h);
}

void ks_drawbox_reset(void) {
    ks_drawbox(0, 0, g_renderer.width, g_renderer.height);
}

void ks_background(ks_col4 col) {
    glClearColor(col.r, col.g, col.b, col.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ks_wiremode(bool state) {
    glPolygonMode(GL_FRONT_AND_BACK, state ? GL_LINE : GL_FILL);
}
void ks_line_width(float width) {
    glLineWidth(width);
}

void ks_begin_drawing(void) {
    glClear(GL_DEPTH_BUFFER_BIT);
    g_renderer.is_drawing = true;
}

void ks_draw_line(ks_vec3 p1, ks_vec3 p2, float size, ks_col4 c);
void ks_draw_rect(ks_vec3 left, ks_vec3 dim, float line_size, ks_col4 c);

void ks_end_drawing(void) {
    ks_batcher_flush();
    glfwSwapBuffers(g_renderer.win);
    glfwPollEvents();
    g_renderer.is_drawing = false;
}

void ks_renderer_destroy(void) {
    glfwDestroyWindow(g_renderer.win);
    glfwTerminate();
}

#endif  // KS_RENDER_IMPL