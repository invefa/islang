#ifndef ISC_VALUE_H
#define ISC_VALUE_H

#include "isl_dump.h"
#include "isl_string.h"
#include "isl_types.h"

/**
 * If you want to reduce the size of the ist_value, you can change below typedefs to the smaller
 * ones, make sure the size be same as another any one. why we set below typedefs? Because we want
 * to make a protocol for ist_value.
 */
typedef ist_bool ist_valueAs_bool;

typedef ist_i8  ist_valueAs_i8;
typedef ist_i16 ist_valueAs_i16;
typedef ist_i32 ist_valueAs_i32;
typedef ist_i64 ist_valueAs_i64;

typedef ist_u8    ist_valueAs_u8;
typedef ist_u16   ist_valueAs_u16;
typedef ist_u32   ist_valueAs_u32;
typedef ist_u64   ist_valueAs_u64;
typedef ist_usize ist_valueAs_usize;

typedef ist_f32 ist_valueAs_f32;
typedef ist_f64 ist_valueAs_f64;

typedef ist_vptr   ist_valueAs_adr;
typedef ist_string ist_valueAs_str;
typedef ist_strbuf ist_valueAs_strbuf;
typedef ist_value* ist_valueAs_valptr;



typedef union isu_valueAs {
    ist_valueAs_bool bool;

    ist_valueAs_i8  i8;
    ist_valueAs_i16 i16;
    ist_valueAs_i32 i32;
    ist_valueAs_i64 i64;

    ist_valueAs_u8    u8;
    ist_valueAs_u16   u16;
    ist_valueAs_u32   u32;
    ist_valueAs_u64   u64;
    ist_valueAs_usize usize;

    ist_valueAs_f32 f32;
    ist_valueAs_f64 f64;

    ist_valueAs_adr    adr;
    ist_valueAs_str    str;
    ist_valueAs_strbuf strbuf;
    ist_valueAs_valptr valptr;

} isu_valueAs;
#define isu_valueAs_ (isu_valueAs)


typedef enum _isn_valueTypeCons {

    isn_valueTypeMask_kind = 0xF0,
    isn_valueTypeMask_size = 0x0F,

    isn_valueTypeSize_0    = 0x00,
    isn_valueTypeSize_8    = 0x01,
    isn_valueTypeSize_16   = 0x02,
    isn_valueTypeSize_32   = 0x04,
    isn_valueTypeSize_64   = 0x08,
    isn_valueTypeSize_arch = 0x0F,

    isn_valueTypeKind_v = 0x10, // void
    isn_valueTypeKind_b = 0x20, // bool
    isn_valueTypeKind_i = 0x30, // int
    isn_valueTypeKind_u = 0x40, // unsinged int
    isn_valueTypeKind_f = 0x50, // float
    isn_valueTypeKind_p = 0xF0, // ptr
} _isn_valueTypeCons;
typedef enum isn_valueType {

    isn_valueType_unk,

    isn_valueType_bool = isn_valueTypeKind_b | isn_valueTypeSize_8,

    isn_valueType_i8    = isn_valueTypeKind_i | isn_valueTypeSize_8,
    isn_valueType_i16   = isn_valueTypeKind_i | isn_valueTypeSize_16,
    isn_valueType_i32   = isn_valueTypeKind_i | isn_valueTypeSize_32,
    isn_valueType_i64   = isn_valueTypeKind_i | isn_valueTypeSize_64,
    isn_valueType_isize = isn_valueTypeKind_i | isn_valueTypeSize_arch,

    isn_valueType_u8    = isn_valueTypeKind_u | isn_valueTypeSize_8,
    isn_valueType_u16   = isn_valueTypeKind_u | isn_valueTypeSize_16,
    isn_valueType_u32   = isn_valueTypeKind_u | isn_valueTypeSize_32,
    isn_valueType_u64   = isn_valueTypeKind_u | isn_valueTypeSize_64,
    isn_valueType_usize = isn_valueTypeKind_u | isn_valueTypeSize_arch,

    isn_valueType_f32 = isn_valueTypeKind_f | isn_valueTypeSize_32,
    isn_valueType_f64 = isn_valueTypeKind_f | isn_valueTypeSize_64,

    isn_valueType_adr    = isn_valueTypeKind_p | 0,
    isn_valueType_valptr = isn_valueTypeKind_p | 1,
    isn_valueType_str    = isn_valueTypeKind_p | 2,
    isn_valueType_strbuf = isn_valueTypeKind_p | 3,

} isn_valueType;

extern ist_cstring         ist_valueTypeNames[];
extern const isn_valueType isl_toklitype_to_valueType[];

ist_u8 isn_valueType_sizeof(isn_valueType v);

// islang typed value
typedef struct ist_tvalue {
    isn_valueType type;
    isu_valueAs   as;
} ist_tvalue;
#define ist_tvalue_ (ist_tvalue)

ist_string ist_tvalue_dump(ist_tvalue* this, ist_dumpctx dctx);

typedef struct ist_value {
    isu_valueAs as;
} ist_value;
#define ist_value_ (ist_value)

ist_string ist_value_dump_old(
    ist_value* this,
    ist_tokenType type,
    ist_string*   buffer,
    ist_usize*    idxptr
);


#define ISG_VALUE_TYPE ist_value
#include "isg_list_head.h"
#define ISG_VALUE_TYPE ist_value
#include "isg_stack_head.h"


#endif // ISC_VALUE_H