#ifndef KISTUNE_CORE_LOG_H
#define KISTUNE_CORE_LOG_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define KIT_LOG_ERR 0
#define KIT_LOG_WARN 1
#define KIT_LOG_INFO 2
#define KIT_LOG_DEBUG 3

extern int kit_log_level;

/**
 * @brief Print a message to stdout
 *
 */
#define KIT_OUT(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/**
 * @brief Print a message and eventually errno to stderr
 *
 */
#define KIT_ERR(fmt, ...)                    \
    do {                                     \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        if (errno) {                         \
            perror(". ERR");                 \
            errno = 0;                       \
        } else {                             \
            fprintf(stderr, "\n");           \
        }                                    \
    } while (0)

#define kit_log(type, fmt, ...)                                                    \
    do {                                                                           \
        if (kit_log_level >= type) {                                               \
            switch (type) {                                                        \
                case KIT_LOG_ERR:                                                  \
                    KIT_ERR("{-}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                         \
                case KIT_LOG_WARN:                                                 \
                    KIT_OUT("{~}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                         \
                case KIT_LOG_INFO:                                                 \
                    KIT_OUT("{+}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                         \
                case KIT_LOG_DEBUG:                                                \
                    KIT_OUT("{*}[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break;                                                         \
                default:                                                           \
                    break;                                                         \
            }                                                                      \
        }                                                                          \
    } while (0)

static inline void kit_log_init(void) {
    char* env = getenv("KIT_LOG_LEVEL");
    if (env) {
        kit_log_level = atoi(env);
    }
}

#endif