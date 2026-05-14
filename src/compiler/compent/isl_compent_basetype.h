#ifndef ISC_COMPENT_BASETYPE_H
#define ISC_COMPENT_BASETYPE_H

/**
 * ISlang_CompEnt_Type::basetype
 *  => iscet_basetype
 * 
 * ISlang_CompEnt_eNum::basetype::X
 *  => iscen_basetype_X
 */
typedef enum iscet_basetype {
    iscen_basetype_void,

    iscen_basetype_bool,

    iscen_basetype_i8,
    iscen_basetype_i16,
    iscen_basetype_i32,
    iscen_basetype_i64,
    iscen_basetype_isize,

    iscen_basetype_u8,
    iscen_basetype_u16,
    iscen_basetype_u32,
    iscen_basetype_u64,
    iscen_basetype_usize,

    iscen_basetype_f32,
    iscen_basetype_f64,

    iscen_basetype_str,
    iscen_basetype_ptr,

} iscen_basetype;



#endif // ISC_COMPENT_BASETYPE_H