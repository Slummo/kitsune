#include <kitsune/core/log.h>

int ksLogLevel = KSINFO;

int main(void) {
    ksLog(KSINFO, "Hello from example 1!");
    ksLog(KSINFO, "Hello World!");
    return EXIT_SUCCESS;
}