#ifndef JVM_CONPOOL_H
#define JVM_CONPOOL_H

#include "jr_utils.h"



enum ConstantType {
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

    TYPE_AFTER64
};


typedef struct {
    u8 kind;
    u16 ref_kind;
} jvm_method_handle;

typedef struct {
   enum ConstantType tag;
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
