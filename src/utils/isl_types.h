#ifndef ISL_TYPES_H
#define ISL_TYPES_H

#include <stdint.h>


typedef size_t      ist_usize;

typedef int8_t      ist_i8;
typedef int16_t     ist_i16;
typedef int32_t     ist_i32;
typedef int64_t     ist_i64;

typedef uint8_t     ist_u8;
typedef uint16_t    ist_u16;
typedef uint32_t    ist_u32;
typedef uint64_t    ist_u64;

typedef float       ist_f32;
typedef double      ist_f64;

/* To avoid complement, we set its type to u8 instead of i8. */
typedef ist_u8      ist_byte;
typedef ist_i8      ist_bool;


#define true  1
#define false 0

#endif