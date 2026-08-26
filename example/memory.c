#define KS_CORE_IMPL
#define KS_MEM_IMPL
#define KS_STRING_IMPL

#include <ks/core.h>
#include <ks/mem.h>
#include <ks/string.h>

int main(void) {
    size_t size = KS_KB(5);
    void* mem = calloc(1, size);

    ks_arena arena;
    ks_arena_init(&arena, mem, size);
    ks_allocator allocator = ks_arena_allocator(&arena);

    uint32_t l1 = 3;
    int32_t* arr1 = KS_ALLOC_ARR(&allocator, int32_t, l1);
    for (uint32_t i = 0; i < l1; ++i) {
        arr1[i] = (int32_t)i;
    }

    for (uint32_t i = 0; i < l1; ++i) {
        ks_log(KSINFO, "arr1[%u] = %d", i, arr1[i]);
    }

    ks_arena_reset(&arena);

    uint32_t l2 = 3;
    ks_string* arr2 = KS_ALLOC_ARR(&allocator, ks_string, l2);
    for (uint32_t i = 0; i < l2; ++i) {
        arr2[i] = ks_string_format("i=%u", i);
    }

    for (uint32_t i = 0; i < l2; ++i) {
        ks_log(KSINFO, "arr2[%u] = %s", i, ks_string_as_raw(&arr2[i]));
    }

    free(mem);

    return EXIT_SUCCESS;
}