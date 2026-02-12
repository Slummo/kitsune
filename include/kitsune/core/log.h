#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define KSERR 0
#define KSWARN 1
#define KSINFO 2
#define KSDEBUG 3

extern int ksLogLevel;

/**
 * @brief Print a message to stdout
 *
 */
#define ksPrintOut(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/**
 * @brief Print a message and eventually errno to stderr
 *
 */
#define ksPrintErr(fmt, ...)                 \
    do {                                     \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        if (errno) {                         \
            perror(". ERR");                 \
            errno = 0;                       \
        } else {                             \
            fprintf(stderr, "\n");           \
        }                                    \
    } while (0)

#define ksLog(type, fmt, ...)                                                         \
    do {                                                                              \
        if (ksLogLevel >= type) {                                                     \
            switch (type) {                                                           \
                case KSERR:                                                           \
                    ksPrintErr("{-}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                            \
                case KSWARN:                                                          \
                    ksPrintOut("{~}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                            \
                case KSINFO:                                                          \
                    ksPrintOut("{+}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                            \
                case KSDEBUG:                                                         \
                    ksPrintOut("{*}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                            \
                default:                                                              \
                    break;                                                            \
            }                                                                         \
        }                                                                             \
    } while (0)

static inline void ksLogInit(void) {
    char* env = getenv("KSLOGLVL");
    if (env) {
        ksLogLevel = strtol(env, NULL, 10);
    }
}