#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"

typedef ist_u32 ist_dumpstyle;
enum ist_dumpstyle {
    DKIND_MASK   = 0x000000FF,
    DKIND_UNK    = 0,
    DKIND_JSON   = 1,
    DKIND_INDENT = 2,
    DKIND_STRUCT = 3,

    DFLAG_MASK           = 0xFFFFFF00,
    DFLAG_SPREAD         = 1 << 8,
    DFLAG_HEAD_INDENT    = 1 << 9,
    DFLAG_HEAD_BREAK     = 1 << 10,
    DFLAG_HEAD_NONAME    = 1 << 11,
    DFLAG_BODY_AFT2SPACE = 1 << 12,
};

typedef ist_string (*ist_dumper)(ist_vptr, ist_strbuf, ist_usize*, ist_usize, ist_dumpstyle);

#define _ist_x_dump_decl(_x)                                                                      \
    ist_string ist_##_x##_dump(                                                                   \
        ist_vptr valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth, ist_dumpstyle style \
    )
_ist_x_dump_decl(u8);
_ist_x_dump_decl(u16);
_ist_x_dump_decl(u32);
_ist_x_dump_decl(u64);
_ist_x_dump_decl(i8);
_ist_x_dump_decl(i16);
_ist_x_dump_decl(i32);
_ist_x_dump_decl(i64);

ist_string ist_f32_dump(
    ist_vptr      valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
);
ist_string ist_f64_dump(
    ist_vptr      valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
);

ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count);

ist_string ist_cstring_dump(
    ist_cstring* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
);
ist_string ist_cstring_dump_ident(
    ist_cstring* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
);

/* name: dumper(valp) */
typedef struct ist_dumpitem {
    ist_cstring name;
    ist_vptr    dumper;
    ist_vptr    valp;
} ist_dumpitem;

typedef struct ist_dumpimage {
    ist_cstring   name;
    ist_dumpitem* items;
    ist_usize     count;
} ist_dumpimage;


ist_string ist_dumpimage_dump(
    ist_dumpimage* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
);

/**
 * listp   :pointer to isg_list.
 * capacity:capacity of isg_list.
 * dumper  :dumper fn for element of list.
 * header  :head text, if set, the head name of element will be ignore.
 * withidx :do head with index, when `header` is not null, it require `header` contains `PRIuPTR`.
 */
typedef struct isg_list_dumpack {
    ist_vptr    listp;
    ist_usize   capacity;
    ist_vptr    dumper;
    ist_cstring header;
    ist_bool    withidx;
} isg_list_dumpack;

ist_string isg_list_dumpack_dump(
    isg_list_dumpack* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
);


#endif // ISC_DUMP_H