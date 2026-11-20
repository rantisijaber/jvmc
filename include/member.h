#ifndef JVM_MEMBER_H
#define JVM_MEMBER_H


#define JVM_MEMBER_H
#include <jr_utils.h>

typedef struct {
    u16 m_access_flags;
    u16 name_index;
    u16 descriptor_info;
    u16 attribute_count;
    
} jvm_member;


#endif
