#ifndef ISL_DBGUTILS_H
#define ISL_DBGUTILS_H

#include <stdio.h>

#include "isl_types.h"

void print_byte_binary_aline(ist_byte _val) {
    char buffer[9];
    buffer[8] = '\0';
    for (ist_u8 i = 0; i < 8; ++i) {
        buffer[7 - i] = 48 + ((_val >> i) & 1);
    }
    printf("%s\n", buffer);
}


#endif