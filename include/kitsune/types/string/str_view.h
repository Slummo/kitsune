#ifndef KITSUNE_CORE_STRING_STR_VIEW_H
#define KITSUNE_CORE_STRING_STR_VIEW_H

#include <kitsune/core/number/integer.h>

typedef union kit_string kit_String;

typedef struct kit_str_view {
    const char* ptr;
    usize len;
} kit_StrView;

kit_StrView kit_str_view_new(const char* s, usize len);
kit_StrView kit_str_view_from_string(const kit_String* s, usize start, usize end);
kit_StrView kit_str_view_from_other(const kit_StrView* s, usize start, usize end);
kit_StrView kit_str_view_from_raw(const char* s, usize start, usize end);
const char* kit_str_view_as_raw(const kit_StrView* s);
i32 kit_str_view_cmp(const kit_StrView* s);

#endif