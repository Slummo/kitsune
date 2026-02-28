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

#define KS_SSO_CAP (sizeof(size_t) * 3 - 1)

ksStruct(String, {
    union {
        struct {
            char data[KS_SSO_CAP];
            uint8_t meta;
        } s;
        struct {
            size_t cap;
            size_t len;
            char* data;
        } l;
    };
});

ksStruct(Str, {
    const char* ptr;
    size_t len;
});

ksString ksStringNew(const char* s);
ksString ksStringNew2(const char* s, size_t len);
ksString ksStringFrom(const ksString* s);
ksString ksStringFromView(const ksStr* view);
ksString ksStringEmpty(size_t cap);
void ksStringReserve(ksString* s, size_t newcap);
void ksStringUpper(ksString* s);
void ksStringLower(ksString* s);
int32_t ksStringToInt64(const ksString* s, int64_t* out, int32_t base);
void ksStringAppend(ksString* dst, const ksString* src);
void ksStringAppendView(ksString* dst, const ksStr* src);
void ksStringAppendRaw(ksString* dst, const char* src);
void ksStringAppendRaw2(ksString* dst, const char* src, size_t srclen);
ksString ksStringConcat(const ksString* s1, const ksString* s2);
ksString ksStringConcatRaw(const char* s1, const char* s2);
ksString ksStringConcatRaw2(const char* s1, size_t len1, const char* s2, size_t len2);
void ksStringPush(ksString* dst, char c);
void ksStringCut(ksString* s, size_t n);
char ksStringPop(ksString* s);
void ksStringTrimLeft(ksString* s);
void ksStringTrimRight(ksString* s);
void ksStringTrim(ksString* s);
ksStr* ksStringSplit(const ksString* s, const char* separator);
ksString ksStringJoin(const ksStr* parts, const char* separator);
char* ksStringFind(const ksString* s, const ksString* needle);
char* ksStringFindRaw(const ksString* s, const char* needle);
char* ksStringFindRaw2(const ksString* s, const char* needle, size_t len);
void ksStringReplace(ksString* s, const ksString* a, const ksString* b);
void ksStringReplaceRaw(ksString* s, const char* a, const char* b);
void ksStringReplaceRaw2(ksString* s, const char* a, size_t lena, const char* b, size_t lenb);
ksString ksStringFormat(const char* fmt, ...);
char* ksStringAsRaw(ksString* s);
ksStr ksStringView(const ksString* s);
bool ksStringIsEmpty(const ksString* s);
bool ksStringIsShort(const ksString* s);
bool ksStringIsAscii(const ksString* s);
bool ksStringIsUpper(const ksString* s);
bool ksStringIsLower(const ksString* s);
bool ksStringIsAlpha(const ksString* s);
bool ksStringIsNum(const ksString* s);
bool ksStringIsAlnum(const ksString* s);
static inline size_t ksStringCap(const ksString* s);
static inline size_t ksStringLen(const ksString* s);
int32_t ksStringCmp(const ksString* s1, const ksString* s2);
void ksStringShrink(ksString* s);
void ksStringClear(ksString* s);
void ksStringFree(ksString* s);

ksStr ksStrNew(const char* s, size_t start, size_t end);
ksStr ksStrFrom(const ksStr* s, size_t start, size_t end);
const char* ksStrAsRaw(const ksStr* s);
void ksStrTrim(ksStr* s);
int32_t ksStrCmp(const ksStr* s1, const ksStr* s2);
bool ksStrStartsWith(const ksStr* s, const char* prefix);
bool ksStrEndsWith(const ksStr* s, const char* suffix);
static inline bool ksStrIsEmpty(const ksStr* s);
static inline size_t ksStrLen(const ksStr* s);

#define ksStringForeach(it, s) \
    for (char *it = ksStringAsRaw((ksString*)s), *_end = it + ksStringLen(s); it < _end; ++it)
#define ksStrForeach(it, s) \
    for (const char *it = (s).ptr, *_end = (const char*)ksPtrOffset((s).ptr, (s).len); it < _end; ++it)

static inline void _ksStringResize(ksString* s, size_t newcap) {
    size_t len = ksStringLen(s);
    size_t oldcap = ksStringCap(s);

    if (ksStringIsShort(s)) {
        if (newcap <= KS_SSO_CAP) {
            return;
        }

        // upgrade to a long string
        char* newdata = malloc(newcap + 1);
        ksAssert(newdata, "OOM");
        memcpy(newdata, s->s.data, len);
        newdata[len] = '\0';
        memset(s, 0, sizeof(ksString));

        s->l.cap = newcap;
        s->l.len = len;
        s->l.data = newdata;
    } else if (newcap <= KS_SSO_CAP) {
        // downgrade to a short string
        char* oldata = s->l.data;
        size_t newlen = ksMin(len, newcap);
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
        ksAssert(newdata, "OOM");
        s->l.data = newdata;
        s->l.cap = newcap;
        if (newcap < len) {
            s->l.len = newcap;
            s->l.data[newcap] = '\0';
        }
    }
}

static inline void _ksStringSetLen(ksString* str, size_t newlen) {
    if (ksStringIsShort(str)) {
        str->s.meta = newlen << 1 | 1;
    } else {
        str->l.len = newlen;
    }
}

static inline void _ksSkipWhiteSpaces(const char** str_ptr) {
    const char* str = *str_ptr;
    while (isspace((unsigned char)*str)) {
        ++str;
    }
    *str_ptr = str;
}

ksString ksStringNew(const char* s) {
    return ksStringNew2(s, strlen(s));
}

ksString ksStringNew2(const char* s, size_t len) {
    ksAssert(s, "s is NULL");

    ksString string = {0};
    if (len <= KS_SSO_CAP) {
        memcpy(string.s.data, s, len);
        string.s.data[len] = '\0';
        string.s.meta = len << 1 | 1;
    } else {
        size_t cap = ksNextPow2(len + 1);
        char* data = malloc(cap);
        ksAssert(data, "OOM");
        string.l.cap = cap - 1;
        string.l.len = len;
        string.l.data = data;
        memcpy(string.l.data, s, len);
        string.l.data[len] = '\0';
    }

    return string;
}

ksString ksStringFrom(const ksString* s) {
    if (!s) {
        return ksStringEmpty(0);
    }

    char* str = ksStringAsRaw((ksString*)s);
    size_t len = ksStringLen(s);
    return ksStringNew2(str, len);
}

ksString ksStringFromView(const ksStr* view) {
    ksAssert(view, "view is NULL");
    return ksStringNew2(view->ptr, view->len);
}

ksString ksStringEmpty(size_t cap) {
    ksString s = {0};
    s.s.data[0] = '\0';
    s.s.meta = 0 << 1 | 1;

    if (cap > KS_SSO_CAP) {
        ksStringReserve(&s, cap);
    }
    return s;
}

void ksStringReserve(ksString* s, size_t newcap) {
    ksAssert(s, "s is NULL");

    size_t oldcap = ksStringCap(s);
    if (newcap <= oldcap) {
        return;
    }

    _ksStringResize(s, newcap);
}

void ksStringUpper(ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        *c = toupper(*c);
    }
}

void ksStringLower(ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        *c = tolower(*c);
    }
}

int32_t ksStringToInt64(const ksString* s, int64_t* out, int32_t base) {
    ksAssert(s && out, "NULL parameters");

    const char* str = ksStringAsRaw((ksString*)s);
    _ksSkipWhiteSpaces(&str);

    if (*str == '\0') {
        return -1;
    }

    errno = 0;
    char* endptr = NULL;
    long long val = strtoll(str, &endptr, base);

    if (endptr == str || errno == ERANGE) {
        return -1;
    }

    _ksSkipWhiteSpaces((const char**)&endptr);

    if (*endptr != '\0') {
        return -1;
    }

    *out = (int64_t)val;

    return 0;
}

void ksStringAppend(ksString* dst, const ksString* src) {
    ksStringAppendRaw2(dst, ksStringAsRaw((ksString*)src), ksStringLen(src));
}

void ksStringAppendView(ksString* dst, const ksStr* src) {
    ksAssert(dst && src, "dst or src is NULL");
    if (src->len == 0 || !src->ptr) {
        return;
    }

    ksStringAppendRaw2(dst, src->ptr, src->len);
}

void ksStringAppendRaw(ksString* dst, const char* src) {
    ksStringAppendRaw2(dst, src, strlen(src));
}

void ksStringAppendRaw2(ksString* dst, const char* src, size_t srclen) {
    ksAssert(dst && src, "dst or src is NULL");

    if (srclen == 0) {
        return;
    }

    size_t dstlen = ksStringLen(dst);
    size_t newlen = srclen + dstlen;

    char* dstraw = ksStringAsRaw(dst);
    bool is_overlapping = src >= dstraw && src < dstraw + dstlen;
    size_t srcoff = is_overlapping ? (size_t)(src - dstraw) : 0;

    if (ksStringCap(dst) < newlen) {
        size_t newcap = newlen <= KS_SSO_CAP ? KS_SSO_CAP : ksNextPow2(newlen + 1);
        _ksStringResize(dst, newcap);

        dstraw = ksStringAsRaw(dst);
        if (is_overlapping) {
            src = dstraw + srcoff;
        }
    }

    memcpy(ksPtrOffset(dstraw, dstlen), src, srclen);
    dstraw[newlen] = '\0';

    _ksStringSetLen(dst, newlen);
}

ksString ksStringConcat(const ksString* s1, const ksString* s2) {
    return ksStringConcatRaw2(ksStringAsRaw((ksString*)s1), ksStringLen(s1), ksStringAsRaw((ksString*)s2),
                              ksStringLen(s2));
}

ksString ksStringConcatRaw(const char* s1, const char* s2) {
    return ksStringConcatRaw2(s1, strlen(s1), s2, strlen(s2));
}

ksString ksStringConcatRaw2(const char* s1, size_t len1, const char* s2, size_t len2) {
    ksString str = ksStringNew2(s1, len1);
    ksStringAppendRaw2(&str, s2, len2);
    return str;
}

void ksStringPush(ksString* dst, char c) {
    ksAssert(dst, "dst is NULL");

    ksStringAppendRaw2(dst, &c, 1);
}

void ksStringCut(ksString* s, size_t n) {
    ksAssert(s, "s is NULL");
    if (n == 0) {
        return;
    }

    size_t len = ksStringLen(s);
    ksAssert(n <= len, "n is greater than the current length");

    size_t newlen = len - n;
    char* raw = ksStringAsRaw(s);
    raw[newlen] = '\0';

    _ksStringSetLen(s, newlen);

    size_t cap = ksStringCap(s);
    if (!ksStringIsShort(s) && newlen < cap >> 2) {
        _ksStringResize(s, cap >> 1);
    }
}

char ksStringPop(ksString* s) {
    ksAssert(s, "s is NULL");

    size_t len = ksStringLen(s);
    if (len == 0) {
        return '\0';
    }

    char* raw = ksStringAsRaw(s);
    char last = raw[len - 1];

    ksStringCut(s, 1);

    return last;
}

void ksStringTrimLeft(ksString* s) {
    ksAssert(s, "s is NULL");
    size_t len = ksStringLen(s);
    if (len == 0) {
        return;
    }

    char* raw = ksStringAsRaw(s);
    size_t start = 0;

    while (start < len && isspace(raw[start])) {
        ++start;
    }

    if (start > 0) {
        size_t newlen = len - start;
        memmove(raw, raw + start, newlen);
        raw[newlen] = '\0';
        _ksStringSetLen(s, newlen);
    }
}

void ksStringTrimRight(ksString* s) {
    ksAssert(s, "s is NULL");
    size_t len = ksStringLen(s);
    if (len == 0) {
        return;
    }

    char* raw = ksStringAsRaw(s);
    size_t end = len;

    while (end > 0 && isspace((unsigned char)raw[end - 1])) {
        --end;
    }

    if (end < len) {
        raw[end] = '\0';
        _ksStringSetLen(s, end);
    }
}

void ksStringTrim(ksString* s) {
    ksAssert(s, "s is NULL");
    size_t len = ksStringLen(s);
    if (len == 0) {
        return;
    }

    char* raw = ksStringAsRaw(s);
    size_t start = 0;
    while (start < len && isspace((unsigned char)raw[start])) {
        ++start;
    }

    if (start == len) {
        ksStringClear(s);
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
    _ksStringSetLen(s, newlen);
}

ksStr* ksStringSplit(const ksString* s, const char* separator) {
    ksAssert(s && separator, "s or separator is NULL");

    size_t seplen = strlen(separator);
    if (seplen == 0) {
        return NULL;
    }

    char* raw = ksStringAsRaw((ksString*)s);
    size_t slen = ksStringLen(s);

    size_t count = 1;
    char* temp = raw;
    while ((temp = strstr(temp, separator))) {
        ++count;
        temp += seplen;
    }

    ksStr* result = malloc((count + 1) * sizeof(ksStr));
    ksAssert(result, "OOM");

    size_t i = 0;
    char* startptr = raw;
    char* matchptr;

    while ((matchptr = strstr(startptr, separator))) {
        size_t startidx = ksPtrAbsDiff(startptr, raw);
        size_t endidx = ksPtrAbsDiff(matchptr, raw);

        result[i++] = ksStrNew(raw, startidx, endidx);
        startptr = matchptr + seplen;
    }

    result[i++] = ksStrNew(raw, (size_t)(startptr - raw), slen);

    result[i].ptr = NULL;
    result[i].len = 0;

    return result;
}

ksString ksStringJoin(const ksStr* parts, const char* separator) {
    if (!parts) {
        return ksStringEmpty(0);
    }
    size_t seplen = separator ? strlen(separator) : 0;

    size_t totlen = 0;
    size_t count = 0;

    while (parts[count].ptr) {
        totlen += parts[count].len;
        ++count;
    }

    if (count == 0) {
        return ksStringEmpty(0);
    }

    if (count > 1) {
        totlen += (count - 1) * seplen;
    }

    ksString result = ksStringEmpty(totlen);
    for (size_t i = 0; i < count; i++) {
        ksStringAppendRaw2(&result, parts[i].ptr, parts[i].len);
        if (i < count - 1 && seplen > 0) {
            ksStringAppendRaw(&result, separator);
        }
    }

    return result;
}

char* ksStringFind(const ksString* s, const ksString* needle) {
    return ksStringFindRaw2(s, ksStringAsRaw((ksString*)needle), ksStringLen(needle));
}

char* ksStringFindRaw(const ksString* s, const char* needle) {
    return ksStringFindRaw2(s, needle, strlen(needle));
}

char* ksStringFindRaw2(const ksString* s, const char* needle, size_t len) {
    char* s1 = ksStringAsRaw((ksString*)s);
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

void ksStringReplace(ksString* s, const ksString* a, const ksString* b) {
    ksStringReplaceRaw2(s, ksStringAsRaw((ksString*)a), ksStringLen(a), ksStringAsRaw((ksString*)b), ksStringLen(b));
}

void ksStringReplaceRaw(ksString* s, const char* a, const char* b) {
    ksStringReplaceRaw2(s, a, strlen(a), b, strlen(b));
}

void ksStringReplaceRaw2(ksString* s, const char* a, size_t lena, const char* b, size_t lenb) {
    ksAssert(s && a && b, "Some parameters are NULL");

    char* pos = ksStringFindRaw2(s, a, lena);
    if (!pos) {
        return;
    }

    char* haystack = ksStringAsRaw(s);
    size_t prelen = (size_t)(pos - haystack);
    size_t suflen = ksStringLen(s) - prelen - lena;
    size_t totlen = prelen + lenb + suflen;

    ksString tmp = ksStringEmpty(totlen);

    if (prelen > 0) {
        ksStringAppendRaw2(&tmp, haystack, prelen);
    }

    if (lenb > 0) {
        ksStringAppendRaw2(&tmp, b, lenb);
    }

    if (suflen > 0) {
        ksStringAppendRaw2(&tmp, pos + lena, suflen);
    }

    ksStringFree(s);

    *s = tmp;
}

ksString ksStringFormat(const char* fmt, ...) {
    ksAssert(fmt, "fmt is NULL");

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        va_end(args);
        return ksStringEmpty(0);
    }

    ksString result = ksStringEmpty((size_t)len);

    char* raw = ksStringAsRaw(&result);
    vsnprintf(raw, (size_t)len + 1, fmt, args);
    va_end(args);

    _ksStringSetLen(&result, (size_t)len);

    return result;
}

char* ksStringAsRaw(ksString* s) {
    if (!s) {
        return NULL;
    }

    return ksStringIsShort(s) ? s->s.data : s->l.data;
}

ksStr ksStringView(const ksString* s) {
    ksAssert(s, "s is NULL");
    return ksStrNew(ksStringAsRaw((ksString*)s), 0, ksStringLen(s));
}

bool ksStringIsEmpty(const ksString* s) {
    return ksStringLen(s) == 0;
}

bool ksStringIsShort(const ksString* s) {
    ksAssert(s, "s is NULL");
    return s->s.meta & 1;
}

bool ksStringIsAscii(const ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        if (!isascii(*c)) {
            return false;
        }
    }

    return true;
}

bool ksStringIsUpper(const ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        if (!isupper(*c)) {
            return false;
        }
    }

    return true;
}

bool ksStringIsLower(const ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        if (!islower(*c)) {
            return false;
        }
    }

    return true;
}

bool ksStringIsAlpha(const ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        if (!isalpha(*c)) {
            return false;
        }
    }

    return true;
}
bool ksStringIsNum(const ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        if (*c < '0' || *c > '9') {
            return false;
        }
    }

    return true;
}
bool ksStringIsAlnum(const ksString* s) {
    ksAssert(s, "s is NULL");
    ksStringForeach(c, s) {
        if (!isalnum(*c)) {
            return false;
        }
    }

    return true;
}

static inline size_t ksStringCap(const ksString* s) {
    if (!s) {
        return 0;
    }

    return ksStringIsShort(s) ? KS_SSO_CAP : s->l.cap;
}

static inline size_t ksStringLen(const ksString* s) {
    if (!s) {
        return 0;
    }

    return ksStringIsShort(s) ? s->s.meta >> 1 : s->l.len;
}

int32_t ksStringCmp(const ksString* s1, const ksString* s2) {
    ksAssert(s1 && s2, "s1 or s2 is NULL");
    return strcmp(ksStringAsRaw((ksString*)s1), ksStringAsRaw((ksString*)s2));
}

void ksStringShrink(ksString* s) {
    ksAssert(s, "s is NULL");

    size_t len = ksStringLen(s);
    _ksStringResize(s, len);
}

void ksStringClear(ksString* s) {
    ksAssert(s, "s is NULL");

    char* raw = ksStringAsRaw(s);
    raw[0] = '\0';
    _ksStringSetLen(s, 0);
}

void ksStringFree(ksString* s) {
    ksAssert(s, "s is NULL");

    if (!ksStringIsShort(s)) {
        free(s->l.data);
    }

    memset(s, 0, sizeof(ksString));
    s->s.meta = 1;
}

ksStr ksStrNew(const char* s, size_t start, size_t end) {
    ksAssert(s, "s is NULL");
    ksAssert(end >= start, "end greater or equal to start");
    return (ksStr){.ptr = (const char*)ksPtrOffset(s, start), .len = end - start};
}

ksStr ksStrFrom(const ksStr* s, size_t start, size_t end) {
    ksAssert(s, "s is NULL");
    ksAssert(end >= start && end <= s->len, "Invalid bounds");
    return (ksStr){.ptr = (const char*)ksPtrOffset(s->ptr, start), .len = end - start};
}

const char* ksStrAsRaw(const ksStr* s) {
    ksAssert(s, "s is NULL");
    return s->ptr;
}

void ksStrTrim(ksStr* s) {
    ksAssert(s, "s is NULL");

    while (s->len > 0 && isspace((unsigned char)s->ptr[0])) {
        ++s->ptr;
        --s->len;
    }

    while (s->len > 0 && isspace((unsigned char)s->ptr[s->len - 1])) {
        --s->len;
    }
}

int32_t ksStrCmp(const ksStr* s1, const ksStr* s2) {
    ksAssert(s1 && s2, "s1 or s2 is NULL");

    size_t minlen = ksMin(s1->len, s2->len);
    int32_t cmp = strncmp(s1->ptr, s2->ptr, minlen);

    if (cmp == 0) {
        if (s1->len < s2->len) {
            return -1;
        }

        if (s1->len > s2->len) {
            return 1;
        }

        return 0;
    }

    return cmp;
}

bool ksStrStartsWith(const ksStr* s, const char* prefix) {
    ksAssert(s && prefix, "s or prefix is NULL");
    size_t prelen = strlen(prefix);

    if (prelen > s->len) {
        return false;
    }

    return strncmp(s->ptr, prefix, prelen) == 0;
}

bool ksStrEndsWith(const ksStr* s, const char* suffix) {
    ksAssert(s && suffix, "s or suffix is NULL");
    size_t suflen = strlen(suffix);

    if (suflen > s->len) {
        return false;
    }

    return strncmp(s->ptr + (s->len - suflen), suffix, suflen) == 0;
}

static inline size_t ksStrLen(const ksStr* s) {
    return s ? s->len : 0;
}

static inline bool ksStrIsEmpty(const ksStr* s) {
    return ksStrLen(s) == 0;
}
