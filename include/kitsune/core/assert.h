#pragma once

#include <kitsune/core/log.h>
#include <stdlib.h>

#define ksPanic(fmt, ...)                                                                                    \
    do {                                                                                                     \
        ksPrintErr("Program panicked at %s:%d in %s():\n\t" fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        abort();                                                                                             \
    } while (0);

#define ksAssert(condition, message)                                   \
    do {                                                               \
        if (!condition) {                                              \
            ksPanic("Assertion failed (" #condition "): %s", message); \
        }                                                              \
    } while (0)
