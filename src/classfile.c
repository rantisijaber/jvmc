#include "classfile.h"
#include "jr_utils.h"
#include "member.h"
#include <stdlib.h>
#include <stdio.h>


u16 parse_u16(jvm_class_file* cf) {
    u16 result = ((cf->bytecode.data[cf->pos] & 0xFFFF) << 8)
                | (cf->bytecode.data[cf->pos + 1] & 0xFFF);
    cf->pos += 2;
    return result;
}

u32 parse_u32(jvm_class_file* cf) {
    return ((parse_u16(cf) & 0xFFFFFFF) << 16)
            | (parse_u16(cf) & 0xFFFFFFFF);
}

void parse_header(jvm_class_file* cf) {
    cf->header.magic_number = parse_u32(cf);
    if (cf->header.magic_number != 0xCAFEBABE) {
        exit(1);
    }
    cf->header.minor = parse_u16(cf);
    cf->header.major = parse_u16(cf);
    cf->header.constant_pool_count = parse_u16(cf);
}

void parse_class_acc(jvm_class_file* cf) {
    u16 acc_flags = parse_u16(cf);
    if ((acc_flags >> 9) & 1) {
        if (!((acc_flags >> 10) & 1) 
            || ((acc_flags >> 4) & 1)
            || ((acc_flags >> 5) & 1)
            || ((acc_flags >> 12) & 1)
            || ((acc_flags >> 14) & 1)) {
            
            exit(1);
        } else if (((acc_flags >> 10) & 1) && 
                    ((acc_flags >> 4) & 1)) {
            exit(1);
        }
    }
    
    cf->cf_access_flags = acc_flags;
}

void parse_this_super(jvm_class_file* cf) {
    u16 this_class = parse_u16(cf);
    if (cf->pool.entries[this_class].tag != TYPE_CLASS) {
        exit(1);
    }
    u16 super_class = parse_u16(cf);
    if (super_class == 0 || ((cf->cf_access_flags >> 9) & 1)) {
        usz class_name_index = cf->pool.entries[super_class].val_16;
        jr_string_view class_name = 
                    cf->pool.entries[class_name_index].val_utf8;
        if (!sv_equals(class_name, SV_LITERAL("java/lang/Object"))) {
           exit(1); 
        }
    }
    cf->this_class = this_class;
    cf->super_class = super_class;
}


void parse_interfaces(jvm_class_file* cf) {
    u16 interface_count = parse_u16(cf);
    u16* interface_indices = (u16 *)malloc(sizeof(u16) * interface_count);
    for (usz i = 0; i < interface_count; i++) {
        interface_indices[i] = parse_u16(cf);
        if (cf->pool.entries[interface_indices[i]].tag != TYPE_CLASS) {
            exit(1);
        }
    }
    cf->interface_count = interface_count;
    cf->interface_indices = interface_indices;

}

void parse_fields(jvm_class_file* cf) {
    u16 field_count = parse_u16(cf);
    cf->field_count = field_count;
    cf->fields = (jvm_member *)malloc(sizeof(jvm_member) * cf->field_count);
    for (usz i = 0; i < cf->field_count; i++) {
        
    }


}




