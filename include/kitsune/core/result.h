#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/utils.h>
#include <kitsune/core/assert.h>
#include <stdbool.h>

#define _KS_RES_T(ok_t, err_t) KS_CONCAT3(ok_t, _, err_t)
#define _KS_RES(ok_t, err_t) KS_TEMPLATED_TYPE(res, _KS_RES_T(ok_t, err_t))
#define _KS_RES_FN(action, ok_t, err_t) KS_TEMPLATED_METHOD(res, action, _KS_RES_T(ok_t, err_t))

#define KS_RESULT(ok_t, err_t)                                                                    \
    KS_TEMPLATED_STRUCT(res, _KS_RES_T(ok_t, err_t), {                                            \
        bool is_ok;                                                                               \
        union {                                                                                   \
            ok_t ok;                                                                              \
            err_t err;                                                                            \
        };                                                                                        \
    });                                                                                           \
                                                                                                  \
    KS_UNUSED static inline _KS_RES(ok_t, err_t) _KS_RES_FN(ok, ok_t, err_t)(ok_t value) {        \
        return (_KS_RES(ok_t, err_t)){.is_ok = true, .ok = value};                                \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline _KS_RES(ok_t, err_t) _KS_RES_FN(err, ok_t, err_t)(err_t value) {      \
        return (_KS_RES(ok_t, err_t)){.is_ok = false, .err = value};                              \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline bool _KS_RES_FN(is_ok, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) {    \
        KS_ASSERT(self, "self is NULL");                                                          \
        return self->is_ok;                                                                       \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline bool _KS_RES_FN(is_err, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) {   \
        KS_ASSERT(self, "self is NULL");                                                          \
        return !self->is_ok;                                                                      \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline ok_t _KS_RES_FN(get_ok, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) {   \
        KS_ASSERT(self, "self is NULL");                                                          \
        KS_ASSERT(self->is_ok, "self is ERR");                                                    \
        return self->ok;                                                                          \
    }                                                                                             \
                                                                                                  \
    KS_UNUSED static inline err_t _KS_RES_FN(get_err, ok_t, err_t)(_KS_RES(ok_t, err_t) * self) { \
        KS_ASSERT(self, "self is NULL");                                                          \
        KS_ASSERT(!self->is_ok, "self is OK");                                                    \
        return self->err;                                                                         \
    }
