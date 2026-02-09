#pragma once

#include <kitsune/core/log.h>
#include <stdlib.h>

#define kit_assert(condition, message)                        \
    do {                                                      \
        if (!condition) {                                     \
            kit_log(KITERR, "ASSERTION FAILED: %s", message); \
            abort();                                          \
        }                                                     \
    } while (0)