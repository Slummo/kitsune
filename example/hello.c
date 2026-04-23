#define KS_CORE_IMPL

#include <ks/core.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_log(KSINFO, "Hello from example 1!");
    ks_log(KSINFO, "Hello World!");
    return EXIT_SUCCESS;
}