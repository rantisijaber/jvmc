#ifndef JVM_CONPOOL_H
#define JVM_CONPOOL_H

#include "jr_utils.h"



typedef enum ConstantType {
    TYPE_IND0,
    TYPE_UTF8,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_CLASS,
    TYPE_STRING,
    TYPE_FIELDREF,
    TYPE_METHODREF,
    TYPE_INTERFACE_METHODREF,
    TYPE_NAMEANDTYPE,
    TYPE_METHODHANDLE,
    TYPE_METHODTYPE,
    TYPE_DYNAMIC,
    TYPE_INVOKEDYNAMIC,
    TYPE_MODUL,
    TYPE_PACKAGE,

    TYPE_AFTER64,
    TYPE_NONEXIST
} ConstantType; 





ConstantType get_type(u8 tag)  {
    switch (tag) {
        case 1: return TYPE_UTF8;
        case 3: return TYPE_INTEGER;
        case 4: return TYPE_FLOAT;
        case 5: return TYPE_LONG;
        case 6: return TYPE_DOUBLE;
        case 7: return TYPE_CLASS;
        case 8: return TYPE_STRING;
        case 9: return TYPE_FIELDREF;
        case 10: return TYPE_METHODREF;
        case 11: return TYPE_INTERFACE_METHODREF;
        case 12: return TYPE_NAMEANDTYPE;
        case 15: return TYPE_METHODHANDLE;
        case 16: return TYPE_METHODTYPE;
        case 17: return TYPE_DYNAMIC;
        case 18: return TYPE_INVOKEDYNAMIC;
        case 19: return TYPE_MODUL;
        case 20: return TYPE_PACKAGE;
        default: return TYPE_NONEXIST;
    }
}

typedef struct {
    u8 kind;
    u16 index;
} jvm_method_handle;

typedef struct {
   ConstantType tag;
   union {
        u16 val_16;
        jvm_method_handle method_handle;
        u32 val_32;
        u64 val_64;
        jr_string_view val_utf8;

   };
} constant_pool_entry;

typedef struct {
    constant_pool_entry* entries;
    usz pos;
} jvm_constant_pool;


void conpool_append(jvm_constant_pool* conpool, constant_pool_entry entry);



#endif
