#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/utils.h>
#include <kitsune/core/assert.h>
#include <stdbool.h>

#define _KS_OPT(T) KS_TEMPLATED_TYPE(opt, T)
#define _KS_OPT_FN(action, T) KS_TEMPLATED_METHOD(opt, action, T)

#define KS_OPTIONAL(T)                                                             \
    KS_TEMPLATED_STRUCT(opt, T, {                                                  \
        bool has_value;                                                            \
        T value;                                                                   \
    });                                                                            \
                                                                                   \
    KS_UNUSED static inline _KS_OPT(T) _KS_OPT_FN(some, T)(T value) {              \
        return (_KS_OPT(T)){.has_value = true, .value = value};                    \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline _KS_OPT(T) _KS_OPT_FN(none, T)(void) {                 \
        return (_KS_OPT(T)){.has_value = false};                                   \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline bool _KS_OPT_FN(is_some, T)(_KS_OPT(T) * self) {       \
        KS_ASSERT(self, "self is NULL");                                           \
        return self->has_value;                                                    \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline bool _KS_OPT_FN(is_none, T)(_KS_OPT(T) * self) {       \
        KS_ASSERT(self, "self is NULL");                                           \
        return !self->has_value;                                                   \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline T* _KS_OPT_FN(ptr, T)(_KS_OPT(T) * self) {             \
        KS_ASSERT(self, "self is NULL");                                           \
        return _KS_OPT_FN(is_some, T)(self) ? &self->value : NULL;                 \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline T _KS_OPT_FN(unwrap_or, T)(_KS_OPT(T) * self, T def) { \
        KS_ASSERT(self, "self is NULL");                                           \
        return _KS_OPT_FN(is_some, T)(self) ? self->value : def;                   \
    }                                                                              \
                                                                                   \
    KS_UNUSED static inline T _KS_OPT_FN(unwrap, T)(_KS_OPT(T) * self) {           \
        KS_ASSERT(self, "self is NULL");                                           \
        KS_ASSERT(_KS_OPT_FN(is_some, T)(self), "self is NONE");                   \
        return self->value;                                                        \
    }
