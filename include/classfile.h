#ifndef JVM_CLASSFILE_H
#define JVM_CLASSFILE_H


#include "jr_utils.h"
#include "conpool.h"

typedef struct jvm_header jvm_header;
typedef struct jvm_member jvm_member;
typedef struct jvm_class_file jvm_class_file;


struct jvm_header {
    u32 magic_number;
    u16 minor;
    u16 major;
    u16 constant_pool_count;
};

struct jvm_member {
    u16 m_access_flags;
    u16 name_index;
    u16 descriptor_info;
    u16 attribute_count;
};

jvm_member make_member(jvm_class_file* cf);
u16 parse_m_acc_flags(jvm_class_file* cf); 


struct jvm_class_file {
    usz pos;
    jvm_header header;
    u16 this_class;
    u16 super_class;
    u16 interface_count;
    u16 field_count;
    u16 method_count;
    u16 cf_access_flags;
    u16* interface_indices;
    jvm_member* fields;
    jvm_member* methods;
    jr_string_view bytecode;
    jvm_constant_pool pool;    
};



u16 parse_u16(jvm_class_file* cf);
u32 parse_u32(jvm_class_file* cf);

void cf_init(jvm_class_file* cf, jr_string_view bytecode);
void cf_parse_access_flags(jvm_class_file* cf);
void cf_parse_header(jvm_class_file* cf);
void cf_parse_this_super(jvm_class_file* cf);
void cf_parse_interfaces(jvm_class_file* cf);
void cf_parse_fields(jvm_class_file* cf);
void cf_parse_methods(jvm_class_file* cf);
void cf_parse(jvm_class_file* cf);
void cf_destroy(jvm_class_file* cf);


#endif

