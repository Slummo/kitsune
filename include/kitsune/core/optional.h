#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/assert.h>
#include <stdbool.h>
#include <stddef.h>

#define __opt_T(T) kit_concat2(Opt_, T)
#define __opt_F(T, action) kit_concat4(kit_opt_, T, _, action)

#define kitOpt(T) kit_concat2(kitOpt_, T)

#define kit_optional(T)                                              \
    kit_struct(__opt_T(T), {                                         \
        bool has_value;                                              \
        T value;                                                     \
    });                                                              \
                                                                     \
    static inline kitOpt(T) __opt_F(T, some)(T value) {              \
        return (kitOpt(T)){.has_value = true, .value = value};       \
    }                                                                \
                                                                     \
    static inline kitOpt(T) __opt_F(T, none)(void) {                 \
        return (kitOpt(T)){.has_value = false};                      \
    }                                                                \
                                                                     \
    static inline bool __opt_F(T, issome)(kitOpt(T) * self) {        \
        kit_assert(self, "self is NULL");                            \
        return self->has_value;                                      \
    }                                                                \
                                                                     \
    static inline bool __opt_F(T, isnone)(kitOpt(T) * self) {        \
        kit_assert(self, "self is NULL");                            \
        return !self->has_value;                                     \
    }                                                                \
                                                                     \
    static inline T* __opt_F(T, ptr)(kitOpt(T) * self) {             \
        kit_assert(self, "self is NULL");                            \
        return __opt_F(T, issome)(self) ? &self->value : NULL;       \
    }                                                                \
                                                                     \
    static inline T __opt_F(T, unwrap_or)(kitOpt(T) * self, T def) { \
        kit_assert(self, "self is NULL");                            \
        return __opt_F(T, issome)(self) ? self->value : def;         \
    }                                                                \
                                                                     \
    static inline T __opt_F(T, unwrap)(kitOpt(T) * self) {           \
        kit_assert(self, "self is NULL");                            \
        kit_assert(__opt_F(T, issome)(self), "self is NONE");        \
        return self->value;                                          \
    }
