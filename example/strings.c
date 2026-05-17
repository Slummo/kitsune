#define KS_CORE_IMPL
#define KS_MEM_IMPL
#define KS_STRING_IMPL

#include <ks/core.h>
#include <ks/string.h>

int main(void) {
    ks_string str1 = ks_string_new("short string");
    ks_string str2 = ks_string_new("This is a long heap-allocated string");

    ks_print("\nstr1: \"%s\" (%zu chars)\nstr2: \"%s\" (%zu chars)", ks_string_as_raw(&str1), ks_string_len(&str1),
             ks_string_as_raw(&str2), ks_string_len(&str2));

    ks_string_append_raw(&str1, " before, now a long string");
    ks_print("str1: \"%s\" (%zu chars, is_short: %d)", ks_string_as_raw(&str1), ks_string_len(&str1),
             ks_string_is_short(&str1));

    ks_string_cut(&str2, 20);
    ks_string_shrink(&str2);
    ks_print("str2: \"%s\" (%zu chars, is_short: %d) now a short string", ks_string_as_raw(&str2), ks_string_len(&str2),
             ks_string_is_short(&str2));

    ks_string_free(&str1);
    ks_string_free(&str2);

    return EXIT_SUCCESS;
}
