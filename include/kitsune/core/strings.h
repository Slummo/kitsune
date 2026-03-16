#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/assert.h>
#include <kitsune/core/utils.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>

#define KS_SSO_CAP (sizeof(size_t) * 3 - 2)

KS_STRUCT(string, {
    union {
        struct {
            char data[KS_SSO_CAP + 1];
            uint8_t meta;
        } s;
        struct {
            size_t cap;
            size_t len;
            char* data;
        } l;
    };
});

KS_STRUCT(str, {
    const char* ptr;
    size_t len;
});

ks_string ks_string_new(const char* s);
ks_string ks_string_new2(const char* s, size_t len);
ks_string ks_string_from(const ks_string* s);
ks_string ks_string_from_view(ks_str view);
ks_string ks_string_empty(size_t cap);
void ks_string_reserve(ks_string* s, size_t newcap);
void ks_string_upper(ks_string* s);
void ks_string_lower(ks_string* s);
int32_t ks_string_to_int64(const ks_string* s, int64_t* out, int32_t base);
void ks_string_append(ks_string* dst, const ks_string* src);
void ks_string_append_view(ks_string* dst, ks_str src);
void ks_string_append_raw(ks_string* dst, const char* src);
void ks_string_append_raw2(ks_string* dst, const char* src, size_t srclen);
ks_string ks_string_concat(const ks_string* s1, const ks_string* s2);
ks_string ks_string_concat_raw(const char* s1, const char* s2);
ks_string ks_string_concat_raw2(const char* s1, size_t len1, const char* s2, size_t len2);
void ks_string_push(ks_string* dst, char c);
void ks_string_cut(ks_string* s, size_t n);
char ks_string_pop(ks_string* s);
void ks_string_trim_left(ks_string* s);
void ks_string_trim_right(ks_string* s);
void ks_string_trim(ks_string* s);
ks_str* ks_string_split(const ks_string* s, const char* separator);
ks_string ks_string_join(const ks_str* parts, const char* separator);
char* ks_string_find(const ks_string* s, const ks_string* needle);
char* ks_string_find_raw(const ks_string* s, const char* needle);
char* ks_string_find_raw2(const ks_string* s, const char* needle, size_t len);
void ks_string_replace(ks_string* s, const ks_string* a, const ks_string* b);
void ks_string_replace_raw(ks_string* s, const char* a, const char* b);
void ks_string_replace_raw2(ks_string* s, const char* a, size_t lena, const char* b, size_t lenb);
ks_string ks_string_format(const char* fmt, ...);
static inline char* ks_string_as_raw(ks_string* s);
static inline ks_str ks_string_view(const ks_string* s);
static inline bool ks_string_is_empty(const ks_string* s);
static inline bool ks_string_is_short(const ks_string* s);
bool ks_string_is_ascii(const ks_string* s);
bool ks_string_is_upper(const ks_string* s);
bool ks_string_is_lower(const ks_string* s);
bool ks_string_is_alpha(const ks_string* s);
bool ks_string_is_num(const ks_string* s);
bool ks_string_is_alnum(const ks_string* s);
static inline size_t ks_string_cap(const ks_string* s);
static inline size_t ks_string_len(const ks_string* s);
int32_t ks_string_cmp(const ks_string* s1, const ks_string* s2);
void ks_string_shrink(ks_string* s);
void ks_string_clear(ks_string* s);
void ks_string_free(ks_string* s);

ks_str ks_str_new(const char* s, size_t start, size_t end);
ks_str ks_str_from(ks_str s, size_t start, size_t end);
const char* ks_str_as_raw(ks_str s);
void ks_str_trim(ks_str* s);
int32_t ks_str_cmp(ks_str s1, ks_str s2);
bool ks_str_starts_with(ks_str s, const char* prefix);
bool ks_str_ends_with(ks_str s, const char* suffix);
static inline bool ks_str_is_empty(ks_str s);
static inline size_t ks_str_len(ks_str s);

#ifdef KS_STRING_IMPLEMENTATION

#define ks_string_foreach(it, s) \
    for (char *it = ks_string_as_raw((ks_string*)s), *_end = it + ks_string_len(s); it < _end; ++it)
#define ks_str_foreach(it, s) \
    for (const char *it = (s).ptr, *_end = (const char*)KS_PTROFF((s).ptr, (s).len); it < _end; ++it)

static inline void _ks_string_resize(ks_string* s, size_t newcap) {
    size_t len = ks_string_len(s);
    size_t oldcap = ks_string_cap(s);

    if (ks_string_is_short(s)) {
        if (newcap <= KS_SSO_CAP) {
            return;
        }

        // upgrade to a long string
        char* newdata = malloc(newcap + 1);
        KS_ASSERT(newdata, "OOM");
        memcpy(newdata, s->s.data, len);
        newdata[len] = '\0';
        memset(s, 0, sizeof(ks_string));

        s->l.cap = newcap;
        s->l.len = len;
        s->l.data = newdata;
    } else if (newcap <= KS_SSO_CAP) {
        // downgrade to a short string
        char* oldata = s->l.data;
        size_t newlen = KS_MIN(len, newcap);
        memcpy(s->s.data, oldata, newlen);
        s->s.data[newlen] = '\0';
        s->s.meta = newlen << 1 | 1;
        free(oldata);
    } else {
        if (newcap == oldcap) {
            return;
        }

        // create a shorter/longer long string
        char* newdata = realloc(s->l.data, newcap + 1);
        KS_ASSERT(newdata, "OOM");
        s->l.data = newdata;
        s->l.cap = newcap;
        if (newcap < len) {
            s->l.len = newcap;
            s->l.data[newcap] = '\0';
        }
    }
}

static inline void _ks_string_set_len(ks_string* str, size_t newlen) {
    if (ks_string_is_short(str)) {
        str->s.meta = newlen << 1 | 1;
    } else {
        str->l.len = newlen;
    }
}

static inline void _ks_skip_white_spaces(const char** str_ptr) {
    const char* str = *str_ptr;
    while (isspace((unsigned char)*str)) {
        ++str;
    }
    *str_ptr = str;
}

ks_string ks_string_new(const char* s) {
    return ks_string_new2(s, strlen(s));
}

ks_string ks_string_new2(const char* s, size_t len) {
    KS_ASSERT(s, "s is NULL");

    ks_string string = {0};
    if (len <= KS_SSO_CAP) {
        memcpy(string.s.data, s, len);
        string.s.data[len] = '\0';
        string.s.meta = len << 1 | 1;
    } else {
        size_t cap = KS_NEXTPOW2(len + 1);
        char* data = malloc(cap);
        KS_ASSERT(data, "OOM");
        string.l.cap = cap - 1;
        string.l.len = len;
        string.l.data = data;
        memcpy(string.l.data, s, len);
        string.l.data[len] = '\0';
    }

    return string;
}

ks_string ks_string_from(const ks_string* s) {
    if (!s) {
        return ks_string_empty(0);
    }

    char* str = ks_string_as_raw((ks_string*)s);
    size_t len = ks_string_len(s);
    return ks_string_new2(str, len);
}

ks_string ks_string_from_view(ks_str view) {
    return ks_string_new2(view.ptr, view.len);
}

ks_string ks_string_empty(size_t cap) {
    ks_string s = {0};
    s.s.data[0] = '\0';
    s.s.meta = 0 << 1 | 1;

    if (cap > KS_SSO_CAP) {
        ks_string_reserve(&s, cap);
    }
    return s;
}

void ks_string_reserve(ks_string* s, size_t newcap) {
    KS_ASSERT(s, "s is NULL");

    size_t oldcap = ks_string_cap(s);
    if (newcap <= oldcap) {
        return;
    }

    _ks_string_resize(s, newcap);
}

void ks_string_upper(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    ks_string_foreach(c, s) {
        *c = toupper(*c);
    }
}

void ks_string_lower(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    ks_string_foreach(c, s) {
        *c = tolower(*c);
    }
}

int32_t ks_string_to_int64(const ks_string* s, int64_t* out, int32_t base) {
    KS_ASSERT(s && out, "NULL parameters");

    const char* str = ks_string_as_raw((ks_string*)s);
    _ks_skip_white_spaces(&str);

    if (*str == '\0') {
        return -1;
    }

    errno = 0;
    char* endptr = NULL;
    long long val = strtoll(str, &endptr, base);

    if (endptr == str || errno == ERANGE) {
        return -1;
    }

    const char* cend = endptr;
    _ks_skip_white_spaces(&cend);

    if (*cend != '\0') {
        return -1;
    }

    *out = (int64_t)val;

    return 0;
}

void ks_string_append(ks_string* dst, const ks_string* src) {
    ks_string_append_raw2(dst, ks_string_as_raw((ks_string*)src), ks_string_len(src));
}

void ks_string_append_view(ks_string* dst, ks_str src) {
    KS_ASSERT(dst, "dst is NULL");
    if (src.len == 0 || !src.ptr) {
        return;
    }

    ks_string_append_raw2(dst, src.ptr, src.len);
}

void ks_string_append_raw(ks_string* dst, const char* src) {
    ks_string_append_raw2(dst, src, strlen(src));
}

void ks_string_append_raw2(ks_string* dst, const char* src, size_t srclen) {
    KS_ASSERT(dst && src, "dst or src is NULL");

    if (srclen == 0) {
        return;
    }

    size_t dstlen = ks_string_len(dst);
    size_t newlen = srclen + dstlen;

    char* dstraw = ks_string_as_raw(dst);
    bool is_overlapping = src >= dstraw && src < dstraw + dstlen;
    size_t srcoff = is_overlapping ? (size_t)(src - dstraw) : 0;

    if (ks_string_cap(dst) < newlen) {
        size_t newcap = newlen <= KS_SSO_CAP ? KS_SSO_CAP : KS_NEXTPOW2(newlen + 1);
        _ks_string_resize(dst, newcap);

        dstraw = ks_string_as_raw(dst);
        if (is_overlapping) {
            src = dstraw + srcoff;
        }
    }

    memcpy(KS_PTROFF(dstraw, dstlen), src, srclen);
    dstraw[newlen] = '\0';

    _ks_string_set_len(dst, newlen);
}

ks_string ks_string_concat(const ks_string* s1, const ks_string* s2) {
    return ks_string_concat_raw2(ks_string_as_raw((ks_string*)s1), ks_string_len(s1), ks_string_as_raw((ks_string*)s2),
                                 ks_string_len(s2));
}

ks_string ks_string_concat_raw(const char* s1, const char* s2) {
    return ks_string_concat_raw2(s1, strlen(s1), s2, strlen(s2));
}

ks_string ks_string_concat_raw2(const char* s1, size_t len1, const char* s2, size_t len2) {
    ks_string str = ks_string_new2(s1, len1);
    ks_string_append_raw2(&str, s2, len2);
    return str;
}

void ks_string_push(ks_string* dst, char c) {
    KS_ASSERT(dst, "dst is NULL");

    ks_string_append_raw2(dst, &c, 1);
}

void ks_string_cut(ks_string* s, size_t n) {
    KS_ASSERT(s, "s is NULL");
    if (n == 0) {
        return;
    }

    size_t len = ks_string_len(s);
    KS_ASSERT(n <= len, "n is greater than the current length");

    size_t newlen = len - n;
    char* raw = ks_string_as_raw(s);
    raw[newlen] = '\0';

    _ks_string_set_len(s, newlen);

    size_t cap = ks_string_cap(s);
    if (!ks_string_is_short(s) && newlen < cap >> 2) {
        _ks_string_resize(s, cap >> 1);
    }
}

char ks_string_pop(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    size_t len = ks_string_len(s);
    if (len == 0) {
        return '\0';
    }

    char* raw = ks_string_as_raw(s);
    char last = raw[len - 1];

    ks_string_cut(s, 1);

    return last;
}

void ks_string_trim_left(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    size_t len = ks_string_len(s);
    if (len == 0) {
        return;
    }

    char* raw = ks_string_as_raw(s);
    size_t start = 0;

    while (start < len && isspace(raw[start])) {
        ++start;
    }

    if (start > 0) {
        size_t newlen = len - start;
        memmove(raw, raw + start, newlen);
        raw[newlen] = '\0';
        _ks_string_set_len(s, newlen);
    }
}

void ks_string_trim_right(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    size_t len = ks_string_len(s);
    if (len == 0) {
        return;
    }

    char* raw = ks_string_as_raw(s);
    size_t end = len;

    while (end > 0 && isspace((unsigned char)raw[end - 1])) {
        --end;
    }

    if (end < len) {
        raw[end] = '\0';
        _ks_string_set_len(s, end);
    }
}

void ks_string_trim(ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    size_t len = ks_string_len(s);
    if (len == 0) {
        return;
    }

    char* raw = ks_string_as_raw(s);
    size_t start = 0;
    while (start < len && isspace((unsigned char)raw[start])) {
        ++start;
    }

    if (start == len) {
        ks_string_clear(s);
        return;
    }

    size_t end = len;
    while (end > start && isspace(raw[end - 1])) {
        --end;
    }

    size_t newlen = end - start;
    if (start > 0) {
        memmove(raw, raw + start, newlen);
    }

    raw[newlen] = '\0';
    _ks_string_set_len(s, newlen);
}

ks_str* ks_string_split(const ks_string* s, const char* separator) {
    KS_ASSERT(s && separator, "s or separator is NULL");

    size_t seplen = strlen(separator);
    if (seplen == 0) {
        return NULL;
    }

    char* raw = ks_string_as_raw((ks_string*)s);
    size_t slen = ks_string_len(s);

    size_t count = 1;
    char* temp = raw;
    while ((temp = strstr(temp, separator))) {
        ++count;
        temp += seplen;
    }

    ks_str* result = malloc((count + 1) * sizeof(ks_str));
    KS_ASSERT(result, "OOM");

    size_t i = 0;
    char* startptr = raw;
    char* matchptr;

    while ((matchptr = strstr(startptr, separator))) {
        size_t startidx = KS_PTRDIFF_ABS(startptr, raw);
        size_t endidx = KS_PTRDIFF_ABS(matchptr, raw);

        result[i++] = ks_str_new(raw, startidx, endidx);
        startptr = matchptr + seplen;
    }

    result[i++] = ks_str_new(raw, (size_t)(startptr - raw), slen);

    result[i].ptr = NULL;
    result[i].len = 0;

    return result;
}

ks_string ks_string_join(const ks_str* parts, const char* separator) {
    if (!parts) {
        return ks_string_empty(0);
    }
    size_t seplen = separator ? strlen(separator) : 0;

    size_t totlen = 0;
    size_t count = 0;

    while (parts[count].ptr) {
        totlen += parts[count].len;
        ++count;
    }

    if (count == 0) {
        return ks_string_empty(0);
    }

    if (count > 1) {
        totlen += (count - 1) * seplen;
    }

    ks_string result = ks_string_empty(totlen);
    for (size_t i = 0; i < count; ++i) {
        ks_string_append_raw2(&result, parts[i].ptr, parts[i].len);
        if (i < count - 1 && seplen > 0) {
            ks_string_append_raw2(&result, separator, seplen);
        }
    }

    return result;
}

char* ks_string_find(const ks_string* s, const ks_string* needle) {
    return ks_string_find_raw2(s, ks_string_as_raw((ks_string*)needle), ks_string_len(needle));
}

char* ks_string_find_raw(const ks_string* s, const char* needle) {
    return ks_string_find_raw2(s, needle, strlen(needle));
}

char* ks_string_find_raw2(const ks_string* s, const char* needle, size_t len) {
    char* s1 = ks_string_as_raw((ks_string*)s);
    const char* p = s1;

    if (!len) {
        return s1;
    }

    for (; (p = strchr(p, *needle)); ++p) {
        if (strncmp(p, needle, len) == 0) {
            return (char*)p;
        }
    }

    return NULL;
}

void ks_string_replace(ks_string* s, const ks_string* a, const ks_string* b) {
    ks_string_replace_raw2(s, ks_string_as_raw((ks_string*)a), ks_string_len(a), ks_string_as_raw((ks_string*)b),
                           ks_string_len(b));
}

void ks_string_replace_raw(ks_string* s, const char* a, const char* b) {
    ks_string_replace_raw2(s, a, strlen(a), b, strlen(b));
}

void ks_string_replace_raw2(ks_string* s, const char* a, size_t lena, const char* b, size_t lenb) {
    KS_ASSERT(s && a && b, "Some parameters are NULL");

    char* pos = ks_string_find_raw2(s, a, lena);
    if (!pos) {
        return;
    }

    char* haystack = ks_string_as_raw(s);
    size_t prelen = KS_PTRDIFF_ABS(pos, haystack);
    size_t suflen = ks_string_len(s) - prelen - lena;
    size_t totlen = prelen + lenb + suflen;

    ks_string tmp = ks_string_empty(totlen);

    if (prelen > 0) {
        ks_string_append_raw2(&tmp, haystack, prelen);
    }

    if (lenb > 0) {
        ks_string_append_raw2(&tmp, b, lenb);
    }

    if (suflen > 0) {
        ks_string_append_raw2(&tmp, pos + lena, suflen);
    }

    ks_string_free(s);

    *s = tmp;
}

ks_string ks_string_format(const char* fmt, ...) {
    KS_ASSERT(fmt, "fmt is NULL");

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        va_end(args);
        return ks_string_empty(0);
    }

    ks_string result = ks_string_empty((size_t)len);

    char* raw = ks_string_as_raw(&result);
    vsnprintf(raw, (size_t)len + 1, fmt, args);
    va_end(args);

    _ks_string_set_len(&result, (size_t)len);

    return result;
}

static inline char* ks_string_as_raw(ks_string* s) {
    if (!s) {
        return NULL;
    }

    return ks_string_is_short(s) ? s->s.data : s->l.data;
}

static inline ks_str ks_string_view(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    return ks_str_new(ks_string_as_raw((ks_string*)s), 0, ks_string_len(s));
}

static inline bool ks_string_is_empty(const ks_string* s) {
    return ks_string_len(s) == 0;
}

static inline bool ks_string_is_short(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    return s->s.meta & 1;
}

bool ks_string_is_ascii(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isascii(*c)) {
            return false;
        }
    }

    return true;
}

bool ks_string_is_upper(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isupper(*c)) {
            return false;
        }
    }

    return true;
}

bool ks_string_is_lower(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!islower(*c)) {
            return false;
        }
    }

    return true;
}

bool ks_string_is_alpha(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isalpha(*c)) {
            return false;
        }
    }

    return true;
}

bool ks_string_is_num(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (*c < '0' || *c > '9') {
            return false;
        }
    }

    return true;
}

bool ks_string_is_alnum(const ks_string* s) {
    KS_ASSERT(s, "s is NULL");
    if (ks_string_is_empty(s)) {
        return false;
    }

    ks_string_foreach(c, s) {
        if (!isalnum(*c)) {
            return false;
        }
    }

    return true;
}

static inline size_t ks_string_cap(const ks_string* s) {
    if (!s) {
        return 0;
    }

    return ks_string_is_short(s) ? KS_SSO_CAP : s->l.cap;
}

static inline size_t ks_string_len(const ks_string* s) {
    if (!s) {
        return 0;
    }

    return ks_string_is_short(s) ? s->s.meta >> 1 : s->l.len;
}

int32_t ks_string_cmp(const ks_string* s1, const ks_string* s2) {
    KS_ASSERT(s1 && s2, "s1 or s2 is NULL");
    return strcmp(ks_string_as_raw((ks_string*)s1), ks_string_as_raw((ks_string*)s2));
}

void ks_string_shrink(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    size_t len = ks_string_len(s);
    _ks_string_resize(s, len);
}

void ks_string_clear(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    char* raw = ks_string_as_raw(s);
    raw[0] = '\0';
    _ks_string_set_len(s, 0);
}

void ks_string_free(ks_string* s) {
    KS_ASSERT(s, "s is NULL");

    if (!ks_string_is_short(s)) {
        free(s->l.data);
    }

    memset(s, 0, sizeof(ks_string));
    s->s.meta = 1;
}

ks_str ks_str_new(const char* s, size_t start, size_t end) {
    KS_ASSERT(s, "s is NULL");
    KS_ASSERT(end >= start, "end greater or equal to start");
    return (ks_str){.ptr = (const char*)KS_PTROFF(s, start), .len = end - start};
}

ks_str ks_str_from(ks_str s, size_t start, size_t end) {
    KS_ASSERT(end >= start && end <= s.len, "Invalid bounds");
    return (ks_str){.ptr = (const char*)KS_PTROFF(s.ptr, start), .len = end - start};
}

const char* ks_str_as_raw(ks_str s) {
    return s.ptr;
}

void ks_str_trim(ks_str* s) {
    KS_ASSERT(s, "s is NULL");

    while (s->len > 0 && isspace((unsigned char)s->ptr[0])) {
        ++s->ptr;
        --s->len;
    }

    while (s->len > 0 && isspace((unsigned char)s->ptr[s->len - 1])) {
        --s->len;
    }
}

int32_t ks_str_cmp(ks_str s1, ks_str s2) {
    size_t minlen = KS_MIN(s1.len, s2.len);
    int32_t cmp = strncmp(s1.ptr, s2.ptr, minlen);

    if (cmp == 0) {
        if (s1.len < s2.len) {
            return -1;
        }

        if (s1.len > s2.len) {
            return 1;
        }

        return 0;
    }

    return cmp;
}

bool ks_str_starts_with(ks_str s, const char* prefix) {
    KS_ASSERT(prefix, "prefix is NULL");
    size_t prelen = strlen(prefix);

    if (prelen > s.len) {
        return false;
    }

    return strncmp(s.ptr, prefix, prelen) == 0;
}

bool ks_str_ends_with(ks_str s, const char* suffix) {
    KS_ASSERT(suffix, "suffix is NULL");
    size_t suflen = strlen(suffix);

    if (suflen > s.len) {
        return false;
    }

    return strncmp(s.ptr + (s.len - suflen), suffix, suflen) == 0;
}

static inline size_t ks_str_len(ks_str s) {
    return s.len;
}

static inline bool ks_str_is_empty(ks_str s) {
    return ks_str_len(s) == 0;
}

#endif
