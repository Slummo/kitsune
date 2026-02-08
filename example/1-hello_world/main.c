#include <kitsune/core/log.h>

int kit_log_level = KIT_LOG_INFO;

int main(void) {
    kit_log(KIT_LOG_INFO, "Hello World!");
    return 0;
}