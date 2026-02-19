#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/assert.h>
#include <stdbool.h>

#define _ksOpt(T) ksTemplatedType(Opt, T)
#define _ksOptFn(action, T) ksTemplatedMethod(Opt, action, T)

#define ksOptional(T)                                                         \
    ksTemplatedStruct(Opt, T, {                                               \
        bool has_value;                                                       \
        T value;                                                              \
    });                                                                       \
                                                                              \
    ksUnused static inline _ksOpt(T) _ksOptFn(Some, T)(T value) {             \
        return (_ksOpt(T)){.has_value = true, .value = value};                \
    }                                                                         \
                                                                              \
    ksUnused static inline _ksOpt(T) _ksOptFn(None, T)(void) {                \
        return (_ksOpt(T)){.has_value = false};                               \
    }                                                                         \
                                                                              \
    ksUnused static inline bool _ksOptFn(IsSome, T)(_ksOpt(T) * self) {       \
        ksAssert(self, "self is NULL");                                       \
        return self->has_value;                                               \
    }                                                                         \
                                                                              \
    ksUnused static inline bool _ksOptFn(IsNone, T)(_ksOpt(T) * self) {       \
        ksAssert(self, "self is NULL");                                       \
        return !self->has_value;                                              \
    }                                                                         \
                                                                              \
    ksUnused static inline T* _ksOptFn(Ptr, T)(_ksOpt(T) * self) {            \
        ksAssert(self, "self is NULL");                                       \
        return _ksOptFn(IsSome, T)(self) ? &self->value : NULL;               \
    }                                                                         \
                                                                              \
    ksUnused static inline T _ksOptFn(UnwrapOr, T)(_ksOpt(T) * self, T def) { \
        ksAssert(self, "self is NULL");                                       \
        return _ksOptFn(IsSome, T)(self) ? self->value : def;                 \
    }                                                                         \
                                                                              \
    ksUnused static inline T _ksOptFn(Unwrap, T)(_ksOpt(T) * self) {          \
        ksAssert(self, "self is NULL");                                       \
        ksAssert(_ksOptFn(IsSome, T)(self), "self is NONE");                  \
        return self->value;                                                   \
    }
