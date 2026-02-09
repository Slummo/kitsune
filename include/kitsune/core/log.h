#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define KITERR 0
#define KITWARN 1
#define KITINFO 2
#define KITDEBUG 3

extern int kitlog_level;

/**
 * @brief Print a message to stdout
 *
 */
#define KIT_PRINT_OUT(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/**
 * @brief Print a message and eventually errno to stderr
 *
 */
#define KIT_PRINT_ERR(fmt, ...)              \
    do {                                     \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        if (errno) {                         \
            perror(". ERR");                 \
            errno = 0;                       \
        } else {                             \
            fprintf(stderr, "\n");           \
        }                                    \
    } while (0)

#define kit_log(type, fmt, ...)                                                          \
    do {                                                                                 \
        if (kitlog_level >= type) {                                                      \
            switch (type) {                                                              \
                case KITERR:                                                             \
                    KIT_PRINT_ERR("{-}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                               \
                case KITWARN:                                                            \
                    KIT_PRINT_OUT("{~}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                               \
                case KITINFO:                                                            \
                    KIT_PRINT_OUT("{+}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                               \
                case KITDEBUG:                                                           \
                    KIT_PRINT_OUT("{*}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                               \
                default:                                                                 \
                    break;                                                               \
            }                                                                            \
        }                                                                                \
    } while (0)

static inline void kitlog_init(void) {
    char* env = getenv("KITLOG_LEVEL");
    if (env) {
        kitlog_level = strtol(env, NULL, 10);
    }
}