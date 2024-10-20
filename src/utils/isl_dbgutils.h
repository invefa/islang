#ifndef ISC_DBGUTILS_H
#define ISC_DBGUTILS_H

/*
    include this header and use at islnag.c only.
*/

#include <stdio.h>

#include "isl_types.h"
#include "isl_string.h"
#include "isl_xssert.h"

const char* digit_table = "0123456789ABCDEF";

void u64_to_string(ist_u64 _val, ist_string* _buffer, ist_u8 _base) {

    isl_assert(_buffer);
    isl_assert(_buffer[0]);
    isl_assert(_base >= 2 && _base <= 16);

    ist_strbuf_ensure(_buffer, 0, 65);

    char buffer[65] = {0};
    ist_u8 index = 0;

    do
        buffer[index++] = digit_table[_val % _base],
        _val /= _base;
    while (_val);

    for (ist_u8 i = 0; i < index; ++i)
        _buffer[0][index - 1 - i] = buffer[i];

    _buffer[0][index] = '\0';
}

void u32_to_string(ist_u32 _val, ist_string* _buffer, ist_u8 _base) {

    isl_assert(_buffer);
    isl_assert(_buffer[0]);
    isl_assert(_base >= 2 && _base <= 16);

    ist_strbuf_ensure(_buffer, 0, 33);

    char buffer[33] = {0};
    ist_u8 index = 0;

    do
        buffer[index++] = digit_table[_val % _base],
        _val /= _base;
    while (_val);

    for (ist_u8 i = 0; i < index; ++i)
        _buffer[0][index - 1 - i] = buffer[i];

    _buffer[0][index] = '\0';
}

void u16_to_string(ist_u16 _val, ist_string* _buffer, ist_u8 _base) {

    isl_assert(_buffer);
    isl_assert(_buffer[0]);
    isl_assert(_base >= 2 && _base <= 16);

    ist_strbuf_ensure(_buffer, 0, 17);

    char buffer[17] = {0};
    ist_u8 index = 0;

    do
        buffer[index++] = digit_table[_val % _base],
        _val /= _base;
    while (_val);

    for (ist_u8 i = 0; i < index; ++i)
        _buffer[0][index - 1 - i] = buffer[i];

    _buffer[0][index] = '\0';
}

void u8_to_string(ist_u8 _val, ist_string* _buffer, ist_u8 _base) {

    isl_assert(_buffer);
    isl_assert(_buffer[0]);
    isl_assert(_base >= 2 && _base <= 16);

    ist_strbuf_ensure(_buffer, 0, 9);

    char buffer[9] = {0};
    ist_u8 index = 0;

    do
        buffer[index++] = digit_table[_val % _base],
        _val /= _base;
    while (_val);

    for (ist_u8 i = 0; i < index; ++i)
        _buffer[0][index - 1 - i] = buffer[i];

    _buffer[0][index] = '\0';
}

void print_u8_binary_aline(ist_u8 _val) {
    char buffer[9];
    buffer[8] = '\0';
    for (ist_u8 i = 0; i < 8; ++i) {
        buffer[7 - i] = 48 + ((_val >> i) & 1);
    }
    printf("%s\n", buffer);
}

void print_u16_binary_aline(ist_u16 _val) {
    char buffer[17];
    buffer[16] = '\0';
    for (ist_u8 i = 0; i < 16; ++i) {
        buffer[15 - i] = 48 + ((_val >> i) & 1);
    }
    printf("%s\n", buffer);
}

void print_u32_binary_aline(ist_u32 _val) {
    char buffer[33];
    buffer[32] = '\0';
    for (ist_u8 i = 0; i < 32; ++i) {
        buffer[31 - i] = 48 + ((_val >> i) & 1);
    }
    printf("%s\n", buffer);
}

void print_u64_binary_aline(ist_u32 _val) {
    char buffer[65];
    buffer[64] = '\0';
    for (ist_u8 i = 0; i < 64; ++i) {
        buffer[63 - i] = 48 + ((_val >> i) & 1);
    }
    printf("%s\n", buffer);
}

#endif // ISC_DBGUTILS_H