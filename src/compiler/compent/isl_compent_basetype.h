#ifndef ISC_COMPENT_BASETYPE_H
#define ISC_COMPENT_BASETYPE_H

#include "isl_value.h"


/**
 * `iscn_basetype` is a mapping for `isn_valueType`,
 * ISlang_CompEnt_sTruct::basetype  => isct_basetype
 * ISlang_CompEnt_eNum::basetype::X => iscn_basetype_X
 */


typedef void isct_basetype_void;

typedef ist_valueAs_bool isct_basetype_bool;

typedef ist_valueAs_i8  isct_basetype_i8;
typedef ist_valueAs_i16 isct_basetype_i16;
typedef ist_valueAs_i32 isct_basetype_i32;
typedef ist_valueAs_i64 isct_basetype_i64;

typedef ist_valueAs_u8    isct_basetype_u8;
typedef ist_valueAs_u16   isct_basetype_u16;
typedef ist_valueAs_u32   isct_basetype_u32;
typedef ist_valueAs_u64   isct_basetype_u64;
typedef ist_valueAs_usize isct_basetype_usize;

typedef ist_valueAs_f32 isct_basetype_f32;
typedef ist_valueAs_f64 isct_basetype_f64;

typedef ist_valueAs_str isct_basetype_str;
typedef ist_valueAs_adr isct_basetype_adr;


typedef enum _isn_valueTypeCons {

    iscn_basetypeMask_kind = 0xF0,
    iscn_basetypeMask_size = 0x0F,

    iscn_basetypeSize_0    = 0x00,
    iscn_basetypeSize_8    = 0x01,
    iscn_basetypeSize_16   = 0x02,
    iscn_basetypeSize_32   = 0x04,
    iscn_basetypeSize_64   = 0x08,
    iscn_basetypeSize_arch = 0x0F,

    iscn_basetypeKind_o = 0x10, // bottom islang-expression: `()!`,  zero inhabitant
    iscn_basetypeKind_n = 0x20, // unit   islang-expression: `()`,   one  inhabitant
    iscn_basetypeKind_b = 0x30, // bool   islang-expression: `bool`, two  inhabitant
    iscn_basetypeKind_i = 0x40, // int
    iscn_basetypeKind_u = 0x50, // unsinged int
    iscn_basetypeKind_f = 0x60, // float
    iscn_basetypeKind_s = 0xE0, // str
    iscn_basetypeKind_p = 0xF0, // ptr
} _iscn_basetypeCons;

typedef enum iscn_basetype {
    iscn_basetype_unk,
    iscn_basetype_bottom = iscn_basetypeKind_o | iscn_basetypeSize_0,
    iscn_basetype_unit   = iscn_basetypeKind_n | iscn_basetypeSize_0,

    iscn_basetype_bool = iscn_basetypeKind_b | iscn_basetypeSize_8,

    iscn_basetype_i8    = iscn_basetypeKind_i | iscn_basetypeSize_8,
    iscn_basetype_i16   = iscn_basetypeKind_i | iscn_basetypeSize_16,
    iscn_basetype_i32   = iscn_basetypeKind_i | iscn_basetypeSize_32,
    iscn_basetype_i64   = iscn_basetypeKind_i | iscn_basetypeSize_64,
    iscn_basetype_isize = iscn_basetypeKind_i | iscn_basetypeSize_arch,

    iscn_basetype_u8    = iscn_basetypeKind_u | iscn_basetypeSize_8,
    iscn_basetype_u16   = iscn_basetypeKind_u | iscn_basetypeSize_16,
    iscn_basetype_u32   = iscn_basetypeKind_u | iscn_basetypeSize_32,
    iscn_basetype_u64   = iscn_basetypeKind_u | iscn_basetypeSize_64,
    iscn_basetype_usize = iscn_basetypeKind_u | iscn_basetypeSize_arch,

    iscn_basetype_f32 = iscn_basetypeKind_f | iscn_basetypeSize_32,
    iscn_basetype_f64 = iscn_basetypeKind_f | iscn_basetypeSize_64,

    iscn_basetype_adr = iscn_basetypeKind_p | iscn_basetypeSize_arch,
    iscn_basetype_str = iscn_basetypeKind_s | iscn_basetypeSize_arch,

} iscn_basetype;

isn_valueType iscn_basetype_to_isn_valueType(iscn_basetype v);
ist_u8        iscn_basetype_sizeof(iscn_basetype v);



#endif // ISC_COMPENT_BASETYPE_H