#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/assert.h>
#include <stdbool.h>

#define _ksResT(OkT, ErrT) ksConcat3(OkT, _, ErrT)
#define _ksRes(OkT, ErrT) ksTemplatedType(Res, _ksResT(OkT, ErrT))
#define _ksResFn(action, OkT, ErrT) ksTemplatedMethod(Res, action, _ksResT(OkT, ErrT))

#define ksResult(OkT, ErrT)                                                             \
    ksTemplatedStruct(Res, _ksResT(OkT, ErrT), {                                        \
        bool is_ok;                                                                     \
        union {                                                                         \
            OkT ok;                                                                     \
            ErrT err;                                                                   \
        };                                                                              \
    });                                                                                 \
                                                                                        \
    ksUnused static inline _ksRes(OkT, ErrT) _ksResFn(Ok, OkT, ErrT)(OkT value) {       \
        return (_ksRes(OkT, ErrT)){.is_ok = true, .ok = value};                         \
    }                                                                                   \
                                                                                        \
    ksUnused static inline _ksRes(OkT, ErrT) _ksResFn(Err, OkT, ErrT)(ErrT value) {     \
        return (_ksRes(OkT, ErrT)){.is_ok = false, .err = value};                       \
    }                                                                                   \
                                                                                        \
    ksUnused static inline bool _ksResFn(IsOk, OkT, ErrT)(_ksRes(OkT, ErrT) * self) {   \
        ksAssert(self, "self is NULL");                                                 \
        return self->is_ok;                                                             \
    }                                                                                   \
                                                                                        \
    ksUnused static inline bool _ksResFn(IsErr, OkT, ErrT)(_ksRes(OkT, ErrT) * self) {  \
        ksAssert(self, "self is NULL");                                                 \
        return !self->is_ok;                                                            \
    }                                                                                   \
                                                                                        \
    ksUnused static inline OkT _ksResFn(GetOk, OkT, ErrT)(_ksRes(OkT, ErrT) * self) {   \
        ksAssert(self, "self is NULL");                                                 \
        ksAssert(self->is_ok, "self is ERR");                                           \
        return self->ok;                                                                \
    }                                                                                   \
                                                                                        \
    ksUnused static inline ErrT _ksResFn(GetErr, OkT, ErrT)(_ksRes(OkT, ErrT) * self) { \
        ksAssert(self, "self is NULL");                                                 \
        ksAssert(!self->is_ok, "self is OK");                                           \
        return self->err;                                                               \
    }
