#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/assert.h>
#include <kitsune/core/numbers.h>
#include <stdbool.h>
#include <stddef.h>

#define _ksOpt(T) ksTemplatedType(Opt, T)
#define _ksOptFn(T, action) ksTemplatedMethod(Opt, T, action)

#define ksOptional(T)                                                 \
    ksTemplatedStruct(Opt, T, {                                       \
        bool has_value;                                               \
        T value;                                                      \
    });                                                               \
                                                                      \
    static inline _ksOpt(T) _ksOptFn(T, some)(T value) {              \
        return (_ksOpt(T)){.has_value = true, .value = value};        \
    }                                                                 \
                                                                      \
    static inline _ksOpt(T) _ksOptFn(T, none)(void) {                 \
        return (_ksOpt(T)){.has_value = false};                       \
    }                                                                 \
                                                                      \
    static inline bool _ksOptFn(T, issome)(_ksOpt(T) * self) {        \
        ksAssert(self, "self is NULL");                               \
        return self->has_value;                                       \
    }                                                                 \
                                                                      \
    static inline bool _ksOptFn(T, isnone)(_ksOpt(T) * self) {        \
        ksAssert(self, "self is NULL");                               \
        return !self->has_value;                                      \
    }                                                                 \
                                                                      \
    static inline T* _ksOptFn(T, ptr)(_ksOpt(T) * self) {             \
        ksAssert(self, "self is NULL");                               \
        return _ksOptFn(T, issome)(self) ? &self->value : NULL;       \
    }                                                                 \
                                                                      \
    static inline T _ksOptFn(T, unwrap_or)(_ksOpt(T) * self, T def) { \
        ksAssert(self, "self is NULL");                               \
        return _ksOptFn(T, issome)(self) ? self->value : def;         \
    }                                                                 \
                                                                      \
    static inline T _ksOptFn(T, unwrap)(_ksOpt(T) * self) {           \
        ksAssert(self, "self is NULL");                               \
        ksAssert(_ksOptFn(T, issome)(self), "self is NONE");          \
        return self->value;                                           \
    }
