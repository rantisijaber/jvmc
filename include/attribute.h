#ifndef JVM_ATTRIBUTE_H
#define JVM_ATTRIBUTE_H

#include <jr_utils.h>

typedef struct {
    u16 name_index;
    u32 length;
    jr_string_view info;
} jvm_attribute;

jvm_attribute make_attribute(jr_string_view info, u16 index, u32 length);


typedef struct {
    char start_pc[2];
    char end_pc[2];
    char catch_type[2];
} exception_table;

typedef struct {
    u16 max_stack;
    u16 max_locals;
    u16 et_length;
    u32 code_length;
    exception_table* tables;
    jr_string_view code_bytes;
} code_attribute;

code_attribute parse_code_attribute();

#endif
