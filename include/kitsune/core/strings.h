#pragma once

#include <kitsune/core/def.h>
#include <kitsune/core/assert.h>
#include <kitsune/core/utils.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

ksString ksStringNew(const char* s);
ksString ksStringNew2(const char* s, size_t len);
ksString ksStringFrom(const ksString* s);
ksString ksStringEmpty(size_t len);
void ksStringReserve(ksString* s, size_t newcap);
void ksStringAppend(ksString* dst, const ksString* src);
void ksStringAppendRaw(ksString* dst, const char* src);
void ksStringAppendRaw2(ksString* dst, const char* src, size_t srclen);
ksString ksStringConcat(const ksString* s1, const ksString* s2);
ksString ksStringConcatRaw(const char* s1, size_t len1, const char* s2, size_t len2);
void ksStringPush(ksString* dst, char c);
void ksStringCut(ksString* s, size_t n);
char ksStringPop(ksString* s);
size_t ksStringFind(const ksString* s, const ksString* needle);
size_t ksStringFindRaw(const ksString* s, const char* needle, size_t len);
void ksStringReplace(const ksString* s, const ksString* a, const ksString* b);
void ksStringReplaceRaw(const char* s, size_t lens, const char* a, size_t lena, const char* b, size_t lenb);
char* ksStringAsRaw(ksString* s);
bool ksStringIsEmpty(const ksString* s);
bool ksStringIsShort(const ksString* s);
size_t ksStringCap(const ksString* s);
size_t ksStringLen(const ksString* s);
int32_t ksStringCmp(const ksString* s1, const ksString* s2);
void ksStringShrink(ksString* s);
void ksStringClear(ksString* s);
void ksStringFree(ksString* s);

ksStruct(Str, {
    const char* ptr;
    size_t len;
});

ksStr ksStrViewNew(const char* s, size_t start, size_t end);
ksStr ksStrViewFromString(const ksString* s, size_t start, size_t end);
ksStr ksStrViewFromOther(const ksStr* s, size_t start, size_t end);
const char* ksStrViewAsRaw(const ksStr* s);
int32_t ksStrViewCmp(const ksStr* s1, const ksStr* s2);

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

ksString ksStringNew(const char* s) {
    return ksStringNew2(s, strlen(s));
}

ksString ksStringNew2(const char* s, size_t len) {
    ksAssert(s, "s is NULL");

    ksString string;
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

ksString ksStringEmpty(size_t len) {
    return ksStringNew2("", len);
}

void ksStringReserve(ksString* s, size_t newcap) {
    ksAssert(s, "s is NULL");

    size_t oldcap = ksStringCap(s);
    if (newcap <= oldcap) {
        return;
    }

    _ksStringResize(s, newcap);
}

void ksStringAppend(ksString* dst, const ksString* src) {
    ksStringAppendRaw2(dst, ksStringAsRaw((ksString*)src), ksStringLen(src));
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

    if (ksStringCap(dst) < newlen) {
        _ksStringResize(dst, newlen);
    }

    char* dstraw = ksStringAsRaw(dst);
    memcpy(ksPtrOffset(dstraw, dstlen), src, srclen);
    dstraw[newlen] = '\0';

    if (ksStringIsShort(dst)) {
        dst->s.meta = newlen << 1 | 1;
    } else {
        dst->l.len = newlen;
    }
}

ksString ksStringConcat(const ksString* s1, const ksString* s2);
ksString ksStringConcatRaw(const char* s1, size_t len1, const char* s2, size_t len2);
void ksStringPush(ksString* dst, char c);
void ksStringCut(ksString* s, size_t n);
char ksStringPop(ksString* s);
size_t ksStringFind(const ksString* s, const ksString* needle);
size_t ksStringFindRaw(const ksString* s, const char* needle, size_t len);
void ksStringReplace(const ksString* s, const ksString* a, const ksString* b);
void ksStringReplaceRaw(const char* s, size_t lens, const char* a, size_t lena, const char* b, size_t lenb);

char* ksStringAsRaw(ksString* s) {
    if (!s) {
        return NULL;
    }

    return ksStringIsShort(s) ? s->s.data : s->l.data;
}

bool ksStringIsEmpty(const ksString* s) {
    return ksStringLen(s) == 0;
}

bool ksStringIsShort(const ksString* s) {
    ksAssert(s, "s is NULL");
    return s->s.meta & 1;
}

size_t ksStringCap(const ksString* s) {
    if (!s) {
        return 0;
    }
    return ksStringIsShort(s) ? KS_SSO_CAP : s->l.cap;
}

size_t ksStringLen(const ksString* s) {
    if (!s) {
        return 0;
    }
    return ksStringIsShort(s) ? s->s.meta >> 1 : s->l.len;
}

int32_t ksStringCmp(const ksString* s1, const ksString* s2);
void ksStringShrink(ksString* s);
void ksStringClear(ksString* s);
void ksStringFree(ksString* s);

ksStr ksStrViewNew(const char* s, size_t start, size_t end);
ksStr ksStrViewFromString(const ksString* s, size_t start, size_t end);
ksStr ksStrViewFromOther(const ksStr* s, size_t start, size_t end);
const char* ksStrViewAsRaw(const ksStr* s);
int32_t ksStrViewCmp(const ksStr* s1, const ksStr* s2);
