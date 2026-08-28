#ifndef KS_DS_H
#define KS_DS_H

#include <ks/core.h>
#include <ks/mem.h>

/* Static array */

#define KS_SA(T, cap) \
    KS_STRUCT(T, {    \
        T data[cap];  \
        size_t len;   \
    })

#define KS_SA_UNNAMED(T, cap) \
    struct {                  \
        T data[cap];          \
        size_t len;           \
    }

#define ks_sa_data(sa) ((sa)->data)
#define ks_sa_len(sa) ((sa)->len)
#define ks_sa_cap(sa) (sizeof(ks_sa_data(sa)) / sizeof(ks_sa_data(sa)[0]))
#define ks_sa_get(sa, i) (ks_sa_data(sa)[i])

#define ks_sa_isempty(sa) (ks_sa_len(sa) == 0)
#define ks_sa_isfull(sa) (ks_sa_len(sa) >= ks_sa_cap(sa))
#define ks_sa_clear(sa) (ks_sa_len(sa) = 0)
#define ks_sa_init(sa) ks_sa_clear(sa)

#define ks_sa_push(sa, item)                          \
    do {                                              \
        if (!ks_sa_isfull(sa)) {                      \
            ks_sa_data(sa)[ks_sa_len(sa)++] = (item); \
        }                                             \
    } while (0)

#define ks_sa_pop(sa) (ks_sa_isempty(sa) ? ks_sa_data(sa)[0] : ks_sa_data(sa)[--ks_sa_len(sa)])

/* Dynamic array */

#define KS_DA(T)                 \
    KS_STRUCT(T##_da, {          \
        ks_allocator *allocator; \
        T *data;                 \
        size_t len;              \
        size_t cap;              \
    })

#define KS_DA_UNNAMED(T)         \
    struct {                     \
        ks_allocator *allocator; \
        T *data;                 \
        size_t len;              \
        size_t cap;              \
    }

#define ks_da_data(da) ((da)->data)
#define ks_da_len(da) ((da)->len)
#define ks_da_cap(da) ((da)->cap)
#define ks_da_get(da, i) (ks_da_data(da)[i])

#define ks_da_isempty(da) (ks_da_len(da) == 0)
#define ks_da_isfull(da) false
#define ks_da_isnull(da) (ks_da_data(da) == NULL)
#define ks_da_clear(da) (ks_da_len(da) = 0)

#define ks_da_reserve(da, n)                                                                                \
    do {                                                                                                    \
        size_t oldsize = ks_da_cap(da) * sizeof(*ks_da_data(da));                                           \
        size_t newsize = (n) * sizeof(*ks_da_data(da));                                                     \
        void *tmp = (da)->allocator->realloc(ks_da_data(da), oldsize, newsize, (da)->allocator->user_data); \
        if (tmp) {                                                                                          \
            ks_da_data(da) = tmp;                                                                           \
            ks_da_cap(da) = (n);                                                                            \
        }                                                                                                   \
    } while (0)

#define ks_da_init(da, initial_cap, alloc)                    \
    do {                                                      \
        (da)->allocator = (alloc) ? (alloc) : &std_allocator; \
        ks_da_len(da) = 0;                                    \
        ks_da_cap(da) = 0;                                    \
        ks_da_data(da) = NULL;                                \
        if ((initial_cap) > 0) {                              \
            ks_da_reserve(da, initial_cap);                   \
        }                                                     \
    } while (0)

#define ks_da_append(da, item)                                              \
    do {                                                                    \
        if (ks_da_len(da) >= ks_da_cap(da)) {                               \
            ks_da_reserve(da, ks_da_cap(da) == 0 ? 1 : ks_da_cap(da) << 1); \
        }                                                                   \
        ks_da_data(da)[ks_da_len(da)++] = (item);                           \
    } while (0)

#define ks_da_pop(da)             \
    do {                          \
        if (!ks_da_isempty(da)) { \
            ks_da_len(da)--;      \
        }                         \
    } while (0)

#define ks_da_free(da)                                                         \
    do {                                                                       \
        if (!ks_da_isnull(da)) {                                               \
            (da)->allocator->free(ks_da_data(da), (da)->allocator->user_data); \
        }                                                                      \
        ks_da_data(da) = NULL;                                                 \
        ks_da_len(da) = 0;                                                     \
        ks_da_cap(da) = 0;                                                     \
    } while (0)

#define ks_array_foreach(it, arr) for (KS_TYPEOF((arr)->data[0]) *it = (arr)->data; it < (arr)->data + (arr)->len; ++it)

#define KS_CONTAINER_OF(ptr, type, member) ((type *)((int8_t *)(ptr) - offsetof(type, member)))

#endif  // KS_DS_H

#if defined(KS_DS_IMPL) && !defined(KS_DS_IMPL_DONE)
#define KS_DS_IMPL_DONE

#if !defined(KS_MEM_IMPL) && !defined(KS_MEM_IMPL_DONE)
#error "kitsune: ds.h requires mem.h"
#endif

#endif  // KS_DS_IMPL