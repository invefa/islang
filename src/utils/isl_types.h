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
#define ist_vptr_ (ist_vptr)

typedef ist_u8 ist_bool;
#define ist_bool_(_expr) !!(_expr)

#define true  1
#define false 0

// declaration of the compiler structures

typedef union ist_value     ist_value;
typedef struct ist_module   ist_module;
typedef struct ist_compctx  ist_compctx;
typedef struct ist_location ist_location;
typedef struct ist_token    ist_token;
typedef struct ist_codepage ist_codepage;
typedef struct ist_lexer    ist_lexer;
typedef struct ist_astnode  ist_astnode;
typedef struct ist_parser   ist_parser;
typedef struct ist_compent  ist_compent;
typedef struct ist_compiler ist_compiler;


/**
 * compent reference is depend on `ist_compctx`,
 * it is the index of compent in `ist_compctx::compents` */
typedef ist_usize ist_compentRef;



/* This is using to make a temp variable for somewhere need a data pointer */
#define isl_span(_type) (_type[1])


#endif // ISC_TYPES_H