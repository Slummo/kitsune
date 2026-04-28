#pragma once

#include <ks/core.h>
#include <ks/math.h>
#include <ks/io.h>
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

KS_STRUCT(shader, { uint32_t id; });

ks_shader ks_shader_create(const char* vert_filename, const char* frag_filename);
void ks_shader_bind(ks_shader s);
void ks_shader_unbind(void);
void ks_shader_set_vec3(ks_shader s, const char* name, float* data);
void ks_shader_set_vec4(ks_shader s, const char* name, float* data);
void ks_shader_set_mat3(ks_shader s, const char* name, float* data);
void ks_shader_set_mat4(ks_shader s, const char* name, float* data);
void ks_shader_destroy(ks_shader s);

KS_STRUCT(buffer, {
    uint32_t id;
    uint32_t type;
});

ks_buffer ks_buffer_create(uint32_t type, size_t size, const void* data, uint32_t usage);
void ks_buffer_update(ks_buffer b, size_t size, const void* data);
void ks_buffer_bind(ks_buffer b);
void ks_buffer_unbind(uint32_t type);
void ks_buffer_destroy(ks_buffer b);

// TODO
KS_STRUCT(texture, { uint32_t id; });

KS_STRUCT(vert_attr, {
    uint32_t loc;
    int32_t count;  // number of floats in data type
    uint32_t type;  // data type code
    uint32_t off;
    int32_t stride;
    uint32_t divisor;  // 0 = per-vertex, 1 = per-instance
});

#define KS_MAX_VBOS 8
#define KS_MAX_ATTR_PER_VBO 8

KS_STRUCT(renderable, {
    uint32_t vao;
    ks_buffer vbos[KS_MAX_VBOS];
    int32_t vbo_count;
    ks_buffer ebo;
    int32_t has_indices;
    int32_t vert_count;
    int32_t ind_count;
    int32_t inst_count;
});

void ks_renderable_init(ks_renderable* r);
void ks_renderable_add_vbo(ks_renderable* r, ks_buffer b, const ks_vert_attr* attrs, int32_t attr_count);
void ks_renderable_set_ebo(ks_renderable* r, ks_buffer b, int32_t ind_count);
void ks_renderable_set_counts(ks_renderable* r, int32_t vert_count, int32_t ind_count);
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

void ks_batcher_init(uint32_t max_verts, uint32_t max_inds, uint32_t stride, const ks_vert_attr* attributes,
                     uint32_t attr_count);
void ks_batcher_next_vertex(void);
void ks_batcher_push_index(uint32_t i);
void ks_batcher_flush(void);

KS_STRUCT(renderer, {
    int32_t width, height;
    ks_shader shader;
    bool is_drawing;
    GLFWwindow* win;
    ks_batcher batcher;
});

KS_UNUSED static ks_renderer g_renderer = {0};

// TODO
void ks_renderer_init(int32_t width, int32_t height, const char* title);
bool ks_renderer_should_close(void);
void ks_renderer_begin_drawing(void);
void ks_renderer_draw(ks_renderable* r, ks_shader s);
void ks_renderer_draw_line(ks_vec3 p1, ks_vec3 p2, float size, ks_col4 c);
void ks_renderer_draw_rect(ks_vec3 left, ks_vec3 dim, float line_size, ks_col4 c);
void ks_renderer_end_drawing(void);
void ks_renderer_destroy(void);

#ifdef KS_RENDER_IMPL

/* shader */

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

    char* vert_src = file_content(vert_filename);
    char* frag_src = file_content(frag_filename);

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

void ks_shader_set_vec3(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != 0, "Uniform not found");
    glUniform3fv(loc, 1, data);
}

void ks_shader_set_vec4(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != 0, "Uniform not found");
    glUniform4fv(loc, 1, data);
}

void ks_shader_set_mat3(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != 0, "Uniform not found");
    glUniformMatrix3fv(loc, 1, GL_FALSE, data);
}

void ks_shader_set_mat4(ks_shader s, const char* name, float* data) {
    GLint loc = glGetUniformLocation(s.id, name);
    KS_ASSERT(loc != 0, "Uniform not found");
    glUniformMatrix4fv(loc, 1, GL_FALSE, data);
}

void ks_shader_destroy(ks_shader s) {
    glDeleteProgram(s.id);
}

/* buffer */

ks_buffer ks_buffer_create(uint32_t type, size_t size, const void* data, uint32_t usage) {
    ks_buffer b = {.id = 0, .type = type};
    glGenBuffers(1, &b.id);
    ks_buffer_bind(b);
    glBufferData(b.type, (GLsizeiptr)size, data, usage);
    return b;
}

void ks_buffer_update(ks_buffer b, size_t size, const void* data) {
    ks_buffer_bind(b);
    glBufferSubData(b.type, 0, (GLsizeiptr)size, data);
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

/* texture*/

/* renderable */

void ks_renderable_init(ks_renderable* r) {
    KS_ASSERT_NONNULL_ARGS(r);
    memset(r, 0, sizeof(ks_renderable));
    glGenVertexArrays(1, &r->vao);
}

void ks_renderable_add_vbo(ks_renderable* r, ks_buffer b, const ks_vert_attr* attrs, int32_t attr_count) {
    KS_ASSERT_NONNULL_ARGS(r && attrs);
    if (r->vbo_count == KS_MAX_VBOS || attr_count >= KS_MAX_ATTR_PER_VBO) {
        return;
    }

    glBindVertexArray(r->vao);
    ks_buffer_bind(b);

    for (int32_t i = 0; i < attr_count; ++i) {
        const ks_vert_attr* attr = &attrs[i];

        glEnableVertexAttribArray(attr->loc);
        glVertexAttribPointer(attr->loc, attr->count, attr->type, GL_FALSE, attr->stride,
                              (const void*)(uintptr_t)(attr->off));
        if (attr->divisor > 0) {
            glVertexAttribDivisor(attr->loc, attr->divisor);
        }
    }

    r->vbos[r->vbo_count++] = b;
    glBindVertexArray(0);
}

void ks_renderable_set_ebo(ks_renderable* r, ks_buffer b, int32_t ind_count) {
    KS_ASSERT_NONNULL_ARGS(r);
    glBindVertexArray(r->vao);
    ks_buffer_bind(b);

    r->ebo = b;
    r->has_indices = 1;
    r->ind_count = ind_count;

    glBindVertexArray(0);
}

void ks_renderable_set_counts(ks_renderable* r, int32_t vert_count, int32_t ind_count) {
    KS_ASSERT_NONNULL_ARGS(r);
    r->vert_count = vert_count;
    r->ind_count = ind_count;
}

void ks_renderable_destroy(ks_renderable* r) {
    KS_ASSERT_NONNULL_ARGS(r);
    glDeleteVertexArrays(1, &r->vao);
    // Don't destroy buffers to allow sharing between renderables
}

/* batcher */

void ks_batcher_init(uint32_t max_verts, uint32_t max_inds, uint32_t stride, const ks_vert_attr* attrs,
                     uint32_t attr_count);
void ks_batcher_next_vertex(void);
void ks_batcher_push_index(uint32_t i);

void ks_batcher_flush(void) {
    return;
}

/* renderer */

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
}

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

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    g_renderer.width = width;
    g_renderer.height = height;
    g_renderer.is_drawing = false;
    g_renderer.win = window;
    // TODO: init batcher
}

bool ks_renderer_should_close(void) {
    return glfwWindowShouldClose(g_renderer.win);
}

void ks_renderer_begin_drawing(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ks_renderer_draw(ks_renderable* r, ks_shader s) {
    KS_ASSERT_NONNULL_ARGS(r);
    ks_shader_bind(s);
    glBindVertexArray(r->vao);

    if (r->has_indices) {
        if (r->inst_count > 1) {
            glDrawElementsInstanced(GL_TRIANGLES, r->ind_count, GL_UNSIGNED_INT, NULL, r->inst_count);
        } else {
            glDrawElements(GL_TRIANGLES, r->ind_count, GL_UNSIGNED_INT, NULL);
        }
    } else {
        if (r->inst_count > 1) {
            glDrawArraysInstanced(GL_TRIANGLES, 0, r->vert_count, r->inst_count);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, r->vert_count);
        }
    }

    glBindVertexArray(0);
}

void ks_renderer_draw_line(ks_vec3 p1, ks_vec3 p2, float size, ks_col4 c);
void ks_renderer_draw_rect(ks_vec3 left, ks_vec3 dim, float line_size, ks_col4 c);

void ks_renderer_end_drawing(void) {
    ks_batcher_flush();
    glfwSwapBuffers(g_renderer.win);
    glfwPollEvents();
}

void ks_renderer_destroy(void) {
    glfwDestroyWindow(g_renderer.win);
    glfwTerminate();
}

#endif