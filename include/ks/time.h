#ifndef KS_TIME_H
#define KS_TIME_H

#include <ks/core.h>

#include <time.h>

KS_STRUCT(time, {
    double current;
    double previous;
    float frame_dt;
    float fixed_dt;
    float accumulator;
});

KS_API void ks_time_init(float refresh_rate);
KS_API void ks_time_update(void);
KS_API bool ks_time_consume_fixed(void);
KS_API double ks_time_get(void);
KS_API double ks_time_dt(void);
KS_API double ks_time_fixed_dt(void);

KS_API extern ks_time g_time;

#endif  // KS_TIME_H

#if defined(KS_TIME_IMPL) && !defined(KS_TIME_IMPL_DONE)
#define KS_TIME_IMPL_DONE

KS_API ks_time g_time = {0};

#if KS_PLATFORM_WINDOWS

#include <windows.h>

static double ks_os_time(void) {
    static LARGE_INTEGER frequency;
    static int initialized = 0;
    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

#else

static double ks_os_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ((double)ts.tv_nsec / 1e9);
}

#endif

KS_API void ks_time_init(float refresh_rate) {
    g_time.current = ks_os_time();
    g_time.previous = g_time.current;
    g_time.fixed_dt = 1.0f / refresh_rate;
}

KS_API void ks_time_update(void) {
    g_time.current = ks_os_time();
    g_time.frame_dt = (float)(g_time.current - g_time.previous);
    g_time.previous = g_time.current;

    g_time.frame_dt = KS_CLAMP(g_time.frame_dt, 0.0f, 0.25f);
    g_time.accumulator += g_time.frame_dt;
}

KS_API bool ks_time_consume_fixed(void) {
    if (g_time.accumulator >= g_time.fixed_dt) {
        g_time.accumulator -= g_time.fixed_dt;
        return true;
    }
    return false;
}

KS_API double ks_time_get(void) {
    return g_time.current;
}

KS_API double ks_time_dt(void) {
    return g_time.frame_dt;
}

KS_API double ks_time_fixed_dt(void) {
    return g_time.fixed_dt;
}

#endif  // KS_TIME_IMPL