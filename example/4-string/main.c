#include <kitsune/core/log.h>
#include <kitsune/core/result.h>
#include <kitsune/core/numbers.h>
#include <kitsune/core/strings.h>

int ks_log_level = KSINFO;

int main(void) {
    ks_log(KSINFO, "Hello from example 4!");

    ks_string str1 = ks_string_new("short string");
    ks_string str2 = ks_string_new("This is a long heap-allocated string");

    ks_log(KSINFO, "\nstr1: \"%s\"(%zu chars)\nstr2: \"%s\"(%zu chars)", ks_string_as_raw(&str1), ks_string_len(&str1),
           ks_string_as_raw(&str2), ks_string_len(&str2));

    ks_string_append_raw(&str1, " before, now a long string");
    ks_log(KSINFO, "str1: \"%s\"(%zu chars)", ks_string_as_raw(&str1), ks_string_len(&str1));

    return EXIT_SUCCESS;
}
