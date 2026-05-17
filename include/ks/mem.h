#ifndef KS_MEM_H
#define KS_MEM_H

#include <ks/core.h>

/* Memory calculations macros */

#define KS_B(x) ((unsigned long long)x)
#define KS_KB(x) (KS_B(x) * 1000ull)
#define KS_MB(x) (KS_KB(x) * 1000ull)
#define KS_GB(x) (KS_MB(x) * 1000ull)
#define KS_TB(x) (KS_GB(x) * 1000ull)
#define KS_KiB(x) (KS_B(x) * (1ull << 10))
#define KS_MiB(x) (KS_B(x) * (1ull << 20))
#define KS_GiB(x) (KS_B(x) * (1ull << 30))
#define KS_TiB(x) (KS_B(x) * (1ull << 40))

/* Allocator */

KS_STRUCT(allocator, {
    void* ctx;
    void* (*alloc)(void* ctx, size_t size);
    void* (*calloc)(void* ctx, size_t n, size_t size);
    void* (*realloc)(void* ctx, void* ptr, size_t oldsize, size_t newsize);
    void (*free)(void* ctx, void* ptr);
});

KS_API extern const ks_allocator std_allocator;

static inline void* ks_alloc(ks_allocator a, size_t size) {
    return a.alloc(a.ctx, size);
}

static inline void* ks_calloc(ks_allocator a, size_t n, size_t size) {
    return a.calloc(a.ctx, n, size);
}

static inline void* ks_realloc(ks_allocator a, void* ptr, size_t oldsize, size_t newsize) {
    return a.realloc(a.ctx, ptr, oldsize, newsize);
}

static inline void ks_free(ks_allocator a, void* ptr) {
    return a.free(a.ctx, ptr);
}

#define KS_ALLOC_ONE(alloc, type) ((type*)ks_alloc(alloc, sizeof(type)))
#define KS_CALLOC_ONE(alloc, type) ((type*)ks_calloc(alloc, 1, sizeof(type)))
#define KS_ALLOC_ARR(alloc, type, count) ((type*)ks_alloc(alloc, sizeof(type) * (count)))
#define KS_CALLOC_ARR(alloc, type, count) ((type*)ks_alloc(alloc, count, sizeof(type)))

/* Arena allocator */

KS_STRUCT(arena, {
    uint8_t* data;
    size_t cap;
    size_t off;
});

KS_API void ks_arena_init(ks_arena* arena, void* mem, size_t cap);
KS_API void ks_arena_reset(ks_arena* arena);
KS_API ks_allocator ks_arena_allocator(ks_arena* arena);

#endif  // KS_MEM_H

#if defined(KS_MEM_IMPL) && !defined(KS_MEM_IMPL_DONE)
#define KS_MEM_IMPL_DONE

#if !defined(KS_CORE_IMPL) && !defined(KS_CORE_IMPL_DONE)
#error "kitsune: mem.h requires core.h"
#endif

/* Standard allocator */

static void* std_alloc(KS_UNUSED void* ctx, size_t size) {
    return malloc(size);
}

static void* std_calloc(KS_UNUSED void* ctx, size_t n, size_t size) {
    return calloc(n, size);
}

static void* std_realloc(KS_UNUSED void* ctx, void* ptr, KS_UNUSED size_t oldsize, size_t newsize) {
    return realloc(ptr, newsize);
}

static void std_free(KS_UNUSED void* ctx, void* ptr) {
    free(ptr);
}

KS_API const ks_allocator
    std_allocator = {.ctx = NULL, .alloc = std_alloc, .calloc = std_calloc, .realloc = std_realloc, .free = std_free};

/* Arena allocator */

static void* ks_arena_alloc(void* ctx, size_t size) {
    ks_arena* arena = ctx;
    size_t aligned_off = KS_ALIGN_UP(arena->off, 8);
    if (aligned_off + size > arena->cap) {
        return NULL;  // OOM
    }

    void* ptr = KS_PTROFF(arena->data, aligned_off);
    arena->off = aligned_off + size;
    return ptr;
}

static void* ks_arena_calloc(void* ctx, size_t n, size_t size) {
    size_t s = n * size;
    void* ptr = ks_arena_alloc(ctx, s);
    if (ptr) {
        memset(ptr, 0, s);
    }
    return ptr;
}

static void* ks_arena_realloc(void* ctx, void* ptr, size_t oldsize, size_t newsize) {
    ks_arena* arena = ctx;
    if (KS_PTROFF(ptr, oldsize) == KS_PTROFF(arena->data, arena->off)) {
        if (arena->off - oldsize + newsize > arena->cap) {
            return NULL;
        }

        arena->off += newsize - oldsize;
        return ptr;
    }

    void* newptr = ks_arena_alloc(ctx, newsize);
    if (newptr && ptr) {
        size_t cpysize = KS_MIN(oldsize, newsize);
        memcpy(newptr, ptr, cpysize);
    }

    return newptr;
}

static void ks_arena_free(KS_UNUSED void* ctx, KS_UNUSED void* ptr) {
}

KS_API void ks_arena_init(ks_arena* arena, void* mem, size_t cap) {
    KS_ASSERT_NONNULL_ARGS(arena && mem);
    arena->data = (uint8_t*)mem;
    arena->cap = cap;
    arena->off = 0;
}

KS_API void ks_arena_reset(ks_arena* arena) {
    KS_ASSERT_NONNULL_ARGS(arena);
    arena->off = 0;
}

KS_API ks_allocator ks_arena_allocator(ks_arena* arena) {
    KS_ASSERT_NONNULL_ARGS(arena);
    return (ks_allocator){.ctx = arena,
                          .alloc = ks_arena_alloc,
                          .calloc = ks_arena_calloc,
                          .realloc = ks_arena_realloc,
                          .free = ks_arena_free};
}

#endif  // KS_MEM_IMPL