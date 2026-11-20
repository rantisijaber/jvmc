#include "classfile.h"



u16 parse_u16(jvm_class_file* cf) {
    u16 res;    
    if (cf->pos + 1 < cf->bytecode.size) {
        res = ((cf->bytecode.data[cf->pos] & 0xFFFF) << 8)
                | (cf->bytecode.data[cf->pos + 1] & 0xFFF);
    } 
    
    return res;
}

u32 parse_u32(jvm_class_file* cf) {

}


