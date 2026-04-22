#ifndef ISC_TYPES_H
#define ISC_TYPES_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>


typedef size_t ist_usize;
static_assert(
    sizeof(ist_usize) == sizeof(void*),
    "ist_usize(aka size_t) must be the same size as pointer."
);


typedef int8_t  ist_i8;
typedef int16_t ist_i16;
typedef int32_t ist_i32;
typedef int64_t ist_i64;

typedef uint8_t  ist_u8;
typedef uint16_t ist_u16;
typedef uint32_t ist_u32;
typedef uint64_t ist_u64;

typedef float  ist_f32;
typedef double ist_f64;

typedef ist_u8 ist_byte;
typedef char   ist_char;
typedef void*  ist_vptr; /* void pointer, aka ptr */

typedef ist_i8 ist_bool;
#define ist_bool_(_expr) !!(_expr)

#define true  1
#define false 0

// declaration of the compiler structures

union ist_value;

struct ist_module;
struct ist_context;

struct ist_location;
struct ist_token;
struct ist_codepage;
struct ist_lexer;
struct ist_astnode;
struct ist_parser;
struct ist_compent;
struct ist_compiler;


#endif // ISC_TYPES_H