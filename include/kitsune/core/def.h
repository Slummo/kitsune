#pragma once

#define _KS_CONCAT2(a, b) a##b
#define KS_CONCAT2(a, b) _KS_CONCAT2(a, b)
#define KS_CONCAT3(a, b, c) KS_CONCAT2(KS_CONCAT2(a, b), c)
#define KS_CONCAT4(a, b, c, d) KS_CONCAT2(KS_CONCAT2(a, b), KS_CONCAT2(c, d))
#define KS_CONCAT5(a, b, c, d, e) KS_CONCAT2(KS_CONCAT4(a, b, c, d), e)

#define KS_TYPE(name) KS_CONCAT3(ks, _, name)
#define KS_TEMPLATED_TYPE(name, T) KS_CONCAT3(KS_TYPE(name), _, T)

#define KS_STRUCT_DECL(name) typedef struct KS_TYPE(name) KS_TYPE(name)
#define KS_STRUCT_DEF(name, ...) struct KS_TYPE(name) __VA_ARGS__
#define KS_STRUCT(name, ...) \
    KS_STRUCT_DECL(name);    \
    KS_STRUCT_DEF(name, __VA_ARGS__)
#define KS_TEMPLATED_STRUCT(name, T, ...) \
    typedef struct KS_TEMPLATED_TYPE(name, T) __VA_ARGS__ KS_TEMPLATED_TYPE(name, T)

#define KS_UNION_DECL(name) typedef union KS_TYPE(name) KS_TYPE(name)
#define KS_UNION_DEF(name, ...) union KS_TYPE(name) __VA_ARGS__
#define KS_UNION(name, ...) \
    KS_UNION_DECL(name);    \
    KS_UNION_DEF(name, __VA_ARGS__)
#define KS_TEMPLATED_UNION(name, T, ...) typedef union KS_TEMPLATED_TYPE(name, T) __VA_ARGS__ KS_TEMPLATED_TYPE(name, T)

#define KS_TEMPLATED_METHOD(name, action, T) KS_CONCAT5(KS_TYPE(name), _, action, _, T)

#define KS_ENUM(name, ...) typedef enum KS_TYPE(name) __VA_ARGS__ KS_TYPE(name)

#define KS_ALIAS(oldT, newT) typedef oldT KS_CONCAT2(ks, newT)

#define KS_USING(T, usingT) typedef T usingT
