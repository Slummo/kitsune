#ifndef KITSUNE_CORE_STRING_STRING_H
#define KITSUNE_CORE_STRING_STRING_H

#include <kitsune/core/number/integer.h>

typedef union kit_string kit_String;

kit_String kit_string_new(usize cap);
kit_String kit_string_from_raw(const char* s, usize len);
usize kit_string_len(const kit_String* s);
usize kit_string_cap(const kit_String* s);
char* kit_string_as_raw(kit_String* s);
i32 kit_string_cmp(const kit_String* s);

#endif