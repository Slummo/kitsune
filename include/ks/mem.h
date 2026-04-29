#ifndef KS_MEM_H
#define KS_MEM_H

#include <ks/core.h>

#define KS_B(x) ((unsigned long long)x)
#define KS_KB(x) (KS_B(x) * 1000ull)
#define KS_MB(x) (KS_KB(x) * 1000ull)
#define KS_GB(x) (KS_MB(x) * 1000ull)
#define KS_TB(x) (KS_GB(x) * 1000ull)
#define KS_KiB(x) (KS_B(x) * (1ull << 10))
#define KS_MiB(x) (KS_B(x) * (1ull << 20))
#define KS_GiB(x) (KS_B(x) * (1ull << 30))
#define KS_TiB(x) (KS_B(x) * (1ull << 40))

KS_STRUCT(allocator, {
    void* (*alloc)(size_t size, void* user_data);
    void* (*realloc)(void* ptr, size_t oldsize, size_t newsize, void* user_data);
    void (*free)(void* ptr, void* user_data);
    void* user_data;
});

extern const ks_allocator std_allocator;

KS_STRUCT(arena, {
    uint8_t* data;
    size_t cap;
    size_t off;
});

void ks_arena_init(ks_arena* arena, void* mem, size_t cap);
void ks_arena_reset(ks_arena* arena);
ks_allocator ks_arena_allocator(ks_arena* arena);

#endif  // KS_MEM_H

#if defined(KS_MEM_IMPL) && !defined(KS_MEM_IMPL_DONE)
#define KS_MEM_IMPL_DONE

static void* std_alloc(size_t size, KS_UNUSED void* user_data) {
    return malloc(size);
}

static void* std_realloc(void* ptr, KS_UNUSED size_t oldsize, size_t newsize, KS_UNUSED void* user_data) {
    return realloc(ptr, newsize);
}

static void std_free(void* ptr, KS_UNUSED void* user_data) {
    free(ptr);
}

const ks_alloctor std_allocator = {.alloc = std_alloc, .realloc = std_realloc, .free = std_free, .user_data = NULL};

static void* ks_arena_alloc(size_t size, void* user_data) {
    ks_arena* arena = user_data;
    size_t aligned_off = KS_ALIGN_UP(arena->off, 8);
    if (aligned_off + size > arena->cap) {
        return NULL;  // OOM
    }

    void* ptr = KS_PTROFF(arena->data, aligned_off);
    arena->off = aligned_off + size;
    return ptr;
}

static void* ks_arena_realloc(void* ptr, size_t oldsize, size_t newsize, void* user_data) {
    ks_arena* arena = user_data;
    if (KS_PTROFF(ptr, oldsize) == KS_PTROFF(arena->data, arena->off)) {
        arena->off += newsize - oldsize;
        return ptr;
    }

    void* newptr = arena_alloc(newsize, user_data);
    if (newptr && ptr) {
        memcpy(newptr, ptr, oldsize);
    }

    return newptr;
}

static void ks_arena_free(KS_UNUSED void* ptr, KS_UNUSED void* user_data) {
}

void ks_arena_init(ks_arena* arena, void* mem, size_t cap) {
    KS_ASSERT_NONNULL_ARGS(arena && mem);
    arena->data = (uint8_t)mem;
    arena->cap = cap;
    arena->off = 0;
}

void ks_arena_reset(ks_arena* arena) {
    KS_ASSERT_NONNULL_ARGS(arena);
    arena->off = 0;
}

ks_allocator ks_arena_allocator(ks_arena* arena) {
    KS_ASSERT_NONNULL_ARGS(arena);
    return (ks_allocator){
        .alloc = ks_arena_alloc, .realloc = ks_arena_realloc, .free = ks_arena_free, .user_data = arena};
}

#endif  // KS_MEM_IMPL