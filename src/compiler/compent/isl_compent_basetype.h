#ifndef ISC_COMPENT_BASETYPE_H
#define ISC_COMPENT_BASETYPE_H

#include "isl_value.h"


/**
 * `iscet_baseType` is a mapping for `ist_valueType`,
 * ISlang_CompEnt_Type::basetype    => iscet_basetype
 * ISlang_CompEnt_eNum::basetype::X => iscen_basetype_X */
typedef enum iscet_basetype {
    iscen_basetype_unk  = isn_valueType_unk,
    iscen_basetype_void = isn_valueType_void,

    iscen_basetype_bool = isn_valueType_bool,

    iscen_basetype_i8    = isn_valueType_i8,
    iscen_basetype_i16   = isn_valueType_i16,
    iscen_basetype_i32   = isn_valueType_i32,
    iscen_basetype_i64   = isn_valueType_i64,
    iscen_basetype_isize = isn_valueType_isize,

    iscen_basetype_u8    = isn_valueType_u8,
    iscen_basetype_u16   = isn_valueType_u16,
    iscen_basetype_u32   = isn_valueType_u32,
    iscen_basetype_u64   = isn_valueType_u64,
    iscen_basetype_usize = isn_valueType_usize,

    iscen_basetype_f32 = isn_valueType_f32,
    iscen_basetype_f64 = isn_valueType_f64,

    iscen_basetype_str = isn_valueType_str,
    iscen_basetype_ptr = isn_valueType_ptr,

} iscen_basetype;



#endif // ISC_COMPENT_BASETYPE_H