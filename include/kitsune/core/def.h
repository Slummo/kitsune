#pragma once

#define _ksConcat2(a, b) a##b
#define ksConcat2(a, b) _ksConcat2(a, b)
#define ksConcat3(a, b, c) ksConcat2(ksConcat2(a, b), c)
#define ksConcat4(a, b, c, d) ksConcat2(ksConcat2(a, b), ksConcat2(c, d))

#define ksType(name) ksConcat2(ks, name)
#define ksTemplatedType(name, T) ksConcat3(ksType(name), _, T)

#define ksStructDecl(name) typedef struct ksType(name) ksType(name)
#define ksStructDef(name, ...) struct ksType(name) __VA_ARGS__
#define ksStruct(name, ...) \
    ksStructDecl(name);     \
    ksStructDef(name, __VA_ARGS__)
#define ksTemplatedStruct(name, T, ...) typedef struct ksTemplatedType(name, T) __VA_ARGS__ ksTemplatedType(name, T)

#define ksUnionDecl(name) typedef union ksType(name) ksType(name)
#define ksUnionDef(name, ...) union ksType(name) __VA_ARGS__
#define ksUnion(name, ...) \
    ksUnionDecl(name);     \
    ksUnionDef(name, __VA_ARGS__)
#define ksTemplatedUnion(name, T, ...) typedef union ksTemplatedType(name, T) __VA_ARGS__ ksTemplatedType(name, T)

#define ksTemplatedMethod(name, T, action) ksConcat3(ksTemplatedType(name, T), _, action)

#define ksEnum(name, ...) typedef enum ksType(name) __VA_ARGS__ ksType(name)

#define ksAlias(oldT, newT) typedef oldT ksConcat2(ks, newT)

#define ksUsing(kitT, usingT) typedef kitT usingT
