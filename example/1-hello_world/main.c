#include <kitsune/core/log.h>

int kitlog_level = KITINFO;

int main(void) {
    kit_log(KITINFO, "Hello from example 1!");
    kit_log(KITINFO, "Hello World!");
    return EXIT_SUCCESS;
}