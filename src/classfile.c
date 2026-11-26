#include "classfile.h"
#include "conpool.h"
#include "jr_utils.h"
#include <stdlib.h>
#include <stdio.h>



void cf_parse(jvm_class_file* cf) {
    cf_parse_header(cf);
    cf->pool = (jvm_constant_pool) {
       .entries = (constant_pool_entry*)malloc(sizeof(constant_pool_entry) 
                                    * cf->header.constant_pool_count + 1),
        .pos = 1
    };
    cf->pool.entries[0].tag = TYPE_IND0; 
}


u16 parse_u16(jvm_class_file* cf) {
    u16 result = ((cf->bytecode.data[cf->pos] & 0xFFFF) << 8)
                | (cf->bytecode.data[cf->pos + 1] & 0xFFFF);
    cf->pos += 2;
    return result;
}

u32 parse_u32(jvm_class_file* cf) {
    return ((parse_u16(cf) & 0xFFFFFFF) << 16)
            | (parse_u16(cf) & 0xFFFFFFFF);
}

void cf_parse_header(jvm_class_file* cf) {
    cf->header.magic_number = parse_u32(cf);
    ERROR_CHECK(cf->header.magic_number != 0xCAFEBABE, "Invalid magic number", exit(EXIT_FAILURE););
    cf->header.minor = parse_u16(cf);
    cf->header.major = parse_u16(cf);
    cf->header.constant_pool_count = parse_u16(cf);
}

void cf_parse_class_acc(jvm_class_file* cf) {
    u16 acc_flags = parse_u16(cf);
    if ((acc_flags >> 9) & 1) {
        ERROR_CHECK((!((acc_flags >> 10) & 1) 
            || ((acc_flags >> 4) & 1)
            || ((acc_flags >> 5) & 1)
            || ((acc_flags >> 12) & 1)
            || ((acc_flags >> 14) & 1)), "Invalid access flags", exit(EXIT_FAILURE););
 
    } else ERROR_CHECK(((acc_flags >> 10) & 1) && 
                    ((acc_flags >> 4) & 1),"Invalid access flags", exit(EXIT_FAILURE););        
    
    cf->cf_access_flags = acc_flags;
}

void cf_parse_this_super(jvm_class_file* cf) {
    u16 this_class = parse_u16(cf);
    if (cf->pool.entries[this_class].tag != TYPE_CLASS) {
        exit(1);
    }
    u16 super_class = parse_u16(cf);
    if (super_class == 0 || ((cf->cf_access_flags >> 9) & 1)) {
        usz class_name_index = cf->pool.entries[super_class].val_16;
        jr_string_view class_name = 
                    cf->pool.entries[class_name_index].val_utf8;
        ERROR_CHECK(!sv_equals(class_name, SV_LITERAL("java/lang/Object")),
            "Object class not marked as super", exit(EXIT_FAILURE););
        }
    cf->this_class = this_class;
    cf->super_class = super_class;
}


void cf_parse_interfaces(jvm_class_file* cf) {
    u16 interface_count = parse_u16(cf);
    u16* interface_indices = (u16 *)malloc(sizeof(u16) * interface_count);
    for (usz i = 0; i < interface_count; i++) {
        interface_indices[i] = parse_u16(cf);
        ERROR_CHECK(cf->pool.entries[interface_indices[i]].tag != TYPE_CLASS,
                    "Interface pool entry not marked as a type_class", 
                    exit(EXIT_FAILURE););
    }
    cf->interface_count = interface_count;
    cf->interface_indices = interface_indices;

}

void cf_parse_fields(jvm_class_file* cf) {
    u16 field_count = parse_u16(cf);
    cf->field_count = field_count;
    cf->fields = (jvm_member *)malloc(sizeof(jvm_member) * cf->field_count);
    for (usz i = 0; i < cf->field_count; i++) {
                
    }


}


void conpool_append(jvm_constant_pool* conpool, constant_pool_entry entry) {
    conpool->entries[conpool->pos++] = entry;
}



void conpool_parse(jvm_class_file* cf) {
   for (usz i = 1; i < cf->header.constant_pool_count; i++) {
        u8 tag = cf->bytecode.data[i];
        cf->pos++;
        ConstantType entag = get_type(tag);
        constant_pool_entry entry = { .tag = entag };
        bool is64 = false;
        switch (entag) {
            case TYPE_UTF8: {
                u16 length = parse_u16(cf);
                jr_string_view parsed_utf8 = 
                    sv_substring(cf->bytecode, cf->pos, cf->pos + length - 1);  
                cf->pos += length;
                entry.val_utf8 = parsed_utf8;
                break;
            }

            case TYPE_FIELDREF:
            case TYPE_METHODREF:
            case TYPE_INTERFACE_METHODREF:
            case TYPE_NAMEANDTYPE:
            case TYPE_INTEGER:
            case TYPE_FLOAT: {
                u32 parsed_32 = parse_u32(cf);
                entry.val_32 = parsed_32;
                break;
            }
            case TYPE_LONG:
            case TYPE_DOUBLE: {
                u32 top_bytes = parse_u32(cf);
                u32 bottom_bytes = parse_u32(cf);
                u64 parsed_64 = ((top_bytes & 0xFFFFFFFFFFFFFFFF) << 32) 
                                | ((bottom_bytes & 0xFFFFFFFFFFFFFFFF));
                entry.val_64 = parsed_64;
                i++;
                is64 = true;
                break;
            }
            case TYPE_CLASS:
            case TYPE_STRING:
            case TYPE_METHODTYPE:
            case TYPE_MODUL:
            case TYPE_PACKAGE: {
                u16 parsed_16 = parse_u16(cf);
                entry.val_16 = parsed_16;
                break;
            }
            case TYPE_METHODHANDLE: {
                jvm_method_handle handle = {
                    .kind = cf->bytecode.data[cf->pos++],
                    .index = parse_u16(cf)
                };
                
                entry.method_handle = handle;
                break;
            }
            default:
                ERROR_NCONDITION("Invalid tag", exit(EXIT_FAILURE););    

                
        } 
        conpool_append(&cf->pool, entry);
        if (is64) {
            conpool_append(&cf->pool, 
                (constant_pool_entry){ .tag = TYPE_AFTER64 });
        }

    } 
}

