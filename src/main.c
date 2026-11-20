#include "jr_utils.h"
#include "classfile.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args) { 
    if (argc != 2) return 1;

    const char* filename = args[1];

    FILE* file = fopen(filename, "rb");
    
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open .class file");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    u64 size = ftell(file);
    rewind(file);

    u8* buffer = (u8 *)malloc(size);
    
    
    usz bytes_read = fread(buffer, 1, size, file);
    if (bytes_read < size) return 1;

    fclose(file);

    jr_string_view sv = {
        .data = (const u8 *)buffer,
        .size = size
    };

    jvm_class_file cf = { .bytecode = sv };
    
    cf_parse(&cf);


    return 0; 

}
