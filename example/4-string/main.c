#include <kitsune/core/log.h>
#include <kitsune/core/result.h>
#include <kitsune/core/numbers.h>
#include <kitsune/core/strings.h>

int ksLogLevel = KSINFO;

int main(void) {
    ksLog(KSINFO, "Hello from example 4!");

    ksString str1 = ksStringNew("short string");
    ksString str2 = ksStringNew("This is a long heap-allocated string");

    ksLog(KSINFO, "\nstr1: \"%s\"(%zu chars)\nstr2: \"%s\"(%zu chars)", ksStringAsRaw(&str1), ksStringLen(&str1),
          ksStringAsRaw(&str2), ksStringLen(&str2));

    ksStringAppendRaw(&str1, " before, now a long string");
    ksLog(KSINFO, "str1: \"%s\"(%zu chars)", ksStringAsRaw(&str1), ksStringLen(&str1));

    return EXIT_SUCCESS;
}
