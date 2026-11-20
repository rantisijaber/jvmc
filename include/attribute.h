#ifndef JVM_ATTRIBUTE_H
#define JVM_ATTRIBUTE_H

#include <jr_utils.h>

typedef struct {
    u16 name_index;
    u32 length;
    u8 info[];
} jvm_attribute;



#endif
