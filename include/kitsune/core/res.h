#pragma once

#include <kitsune/core/def.h>

KS_ENUM(res, {KS_OK = 0, KS_ERR_GENERIC = -1, KS_ERR_INVALID = -2, KS_ERR_OOM = -3, KS_ERR_NOT_FOUND = -4,
              KS_ERR_DUPLICATE = -5, KS_ERR_EMPTY = -6, KS_ERR_FULL = -7, KS_ERR_BOUNDS = -8});

static inline const char* ks_res_str(int res) {
    switch (res) {
        case KS_OK:
            return "Success";
        case KS_ERR_GENERIC:
            return "Generic error";
        case KS_ERR_INVALID:
            return "Invalid argument";
        case KS_ERR_OOM:
            return "Out of memory";
        case KS_ERR_NOT_FOUND:
            return "Not found";
        case KS_ERR_DUPLICATE:
            return "Duplicate entry";
        case KS_ERR_EMPTY:
            return "Container empty";
        case KS_ERR_FULL:
            return "Container full";
        case KS_ERR_BOUNDS:
            return "Index out of bounds";
        default:
            return "Unknown error";
    }
}