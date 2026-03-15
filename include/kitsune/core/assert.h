#pragma once

#include <kitsune/core/log.h>

#define KS_PANIC(fmt, ...)                                                                                     \
    do {                                                                                                       \
        KS_PRINT_ERR("Program panicked at %s:%d in %s():\n\t" fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        abort();                                                                                               \
    } while (0);

#define KS_ASSERT(condition, message)                                   \
    do {                                                                \
        if (!(condition)) {                                             \
            KS_PANIC("Assertion failed (" #condition "): %s", message); \
        }                                                               \
    } while (0)
