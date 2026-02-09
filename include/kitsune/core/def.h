#pragma once

#define __kit_concat2(a, b) a##b
#define kit_concat2(a, b) __kit_concat2(a, b)
#define kit_concat3(a, b, c) kit_concat2(kit_concat2(a, b), c)
#define kit_concat4(a, b, c, d) kit_concat2(kit_concat2(a, b), kit_concat2(c, d))

#define __kit_type(name) kit_concat2(kit, name)

#define kit_struct_decl(name) typedef struct __kit_type(name) __kit_type(name)
#define kit_struct_def(name, ...) struct __kit_type(name) __VA_ARGS__
#define kit_struct(name, ...) \
    kit_struct_decl(name);    \
    kit_struct_def(name, __VA_ARGS__)

#define kit_union_decl(name) typedef union __kit_type(name) __kit_type(name)
#define kit_union_def(name, ...) union __kit_type(name) __VA_ARGS__
#define kit_union(name, ...) \
    kit_union_decl(name);    \
    kit_union_def(name, __VA_ARGS__)

#define kit_enum(name, ...) typedef enum __kit_type(name) __VA_ARGS__ __kit_type(name)

#define kit_alias(OLD_T, NEW_T) typedef OLD_T kit_concat2(kit, NEW_T)

#define kit_using(KIT_T, USING_T) typedef KIT_T USING_T
