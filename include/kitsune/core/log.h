#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define KSERR 0
#define KSWARN 1
#define KSINFO 2
#define KSDEBUG 3

extern int ks_log_level;

/**
 * @brief Print a message to stdout
 *
 */
#define KS_PRINT_OUT(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/**
 * @brief Print a message and eventually errno to stderr
 *
 */
#define KS_PRINT_ERR(fmt, ...)               \
    do {                                     \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        if (errno) {                         \
            perror(". ERR");                 \
            errno = 0;                       \
        } else {                             \
            fprintf(stderr, "\n");           \
        }                                    \
    } while (0)

#define ks_log(type, fmt, ...)                                                          \
    do {                                                                                \
        if (ks_log_level >= type) {                                                     \
            switch (type) {                                                             \
                case KSERR:                                                             \
                    KS_PRINT_ERR("{-}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                case KSWARN:                                                            \
                    KS_PRINT_OUT("{~}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                case KSINFO:                                                            \
                    KS_PRINT_OUT("{+}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                case KSDEBUG:                                                           \
                    KS_PRINT_OUT("{*}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                              \
                default:                                                                \
                    break;                                                              \
            }                                                                           \
        }                                                                               \
    } while (0)

static inline void ksLogInit(void) {
    char* env = getenv("KSLOGLVL");
    if (env) {
        ks_log_level = strtol(env, NULL, 10);
    }
}
