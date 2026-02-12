#pragma once

#include <kitsune/core/def.h>

ksEnum(Res, {KIT_OK = 0, KIT_ERR_GENERIC = -1, KIT_ERR_INVALID = -2, KIT_ERR_OOM = -3, KIT_ERR_NOT_FOUND = -4,
             KIT_ERR_DUPLICATE = -5, KIT_ERR_EMPTY = -6, KIT_ERR_FULL = -7, KIT_ERR_BOUNDS = -8});

static inline const char* kit_res_str(int err) {
    switch (err) {
        case KIT_OK:
            return "Success";
        case KIT_ERR_GENERIC:
            return "Generic error";
        case KIT_ERR_INVALID:
            return "Invalid argument";
        case KIT_ERR_OOM:
            return "Out of memory";
        case KIT_ERR_NOT_FOUND:
            return "Not found";
        case KIT_ERR_DUPLICATE:
            return "Duplicate entry";
        case KIT_ERR_EMPTY:
            return "Container empty";
        case KIT_ERR_FULL:
            return "Container full";
        case KIT_ERR_BOUNDS:
            return "Index out of bounds";
        default:
            return "Unknown error";
    }
}