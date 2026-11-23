#include <attribute.h>


jvm_attribute make_attribute(jr_string_view info, u16 index, u32 length) {
    return (jvm_attribute) {
        .name_index = index,
        .length = length,
        .info = info
    };
}
