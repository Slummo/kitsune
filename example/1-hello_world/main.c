#include <kitsune/core/log.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_log(KSINFO, "Hello from example 1!");
    ks_log(KSINFO, "Hello World!");
    return EXIT_SUCCESS;
}