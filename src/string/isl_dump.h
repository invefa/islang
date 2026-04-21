#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"

typedef ist_u32 ist_dumpstyle;
enum ist_dumpstyle {
    DKIND_MASK   = 0x000000FF,
    DKIND_UNK    = 0x00000000,
    DKIND_JSON   = 0x00000001,
    DKIND_INDENT = 0x00000002,
    DKIND_STRUCT = 0x00000003,

    DFLAG_MASK           = 0xFFFFFF00,
    DFLAG_SPREAD         = 1 << 8,
    DFLAG_HEAD_NL        = 1 << 9,
    DFLAG_HEAD_TAB       = 1 << 10,
    DFLAG_HEAD_DOWRAP    = 1 << 11,
    DFLAG_BODY_AFT2SPACE = 1 << 12,
    DFLAG_THIS_ONVALSIDE = 1 << 13,

    DFLAG_HEAD_NLTAB = DFLAG_HEAD_NL | DFLAG_HEAD_TAB,
};


typedef struct ist_dumpctx {
    ist_string*   buffer;
    ist_usize*    idxptr;
    ist_usize     indent;
    ist_dumpstyle style;
} ist_dumpctx;
/**
 * dumping context
 * buffer : the buffer for dumping
 * idxptr : pointer of index
 * indent : indent level
 * style  : store the format and flag info
 */
#define ist_dumpctx_ (ist_dumpctx)


typedef ist_string (*ist_dumper)(ist_vptr, ist_dumpctx);



typedef struct ist_dumpitem {
    ist_cstring key;
    ist_vptr    dumper;
    ist_vptr    valp;
} ist_dumpitem;
/**
 * name -> dumper(valp)
 * name   : name of dump item
 * dumper : the dumper for valp
 * valp   : pointer of value
 */
#define ist_dumpitem_   (ist_dumpitem)
#define ist_dumpitemar_ (ist_dumpitem[]) // dumpitem array


typedef struct ist_dumpimage {
    ist_cstring   name;
    ist_cstring   wrapkey;
    ist_usize     count;
    ist_dumpitem* items;
} ist_dumpimage;
/**
 * name    : name of this object
 * wrapkey : if do wrap, the name will become the value of this key
 * count   : count of items
 * items   : entries of this object
 */
#define ist_dumpimage_ (ist_dumpimage)


ist_string ist_dumpimage_dump(ist_dumpimage* this, ist_dumpctx dctx);



typedef struct isg_list_dumpack {
    ist_cstring name;
    ist_cstring idxtag;
    ist_bool    dowrap;
    ist_vptr    dumper;
    ist_vptr    listp;
    ist_usize   capacity;
} isg_list_dumpack;
/**
 * name     : name tag of list
 * idxtag   : index tag for element
 * dowarp   : optional, if true, the name of element will be warping
 * dumper   : dumper fn for all element of list
 * listp    : pointer to isg_list
 * capacity : capacity of isg_list
 */
#define isg_list_dumpack_ (isg_list_dumpack)

ist_string isg_list_dumpack_dump(isg_list_dumpack* this, ist_dumpctx dctx);


ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count);

ist_string ist_i8_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_i16_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_i32_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_i64_dump(ist_vptr this, ist_dumpctx dctx);

ist_string ist_u8_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_u16_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_u32_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_u64_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_usize_dump(ist_vptr this, ist_dumpctx dctx);

ist_string ist_bool_dump(ist_vptr this, ist_dumpctx dctx);

ist_string ist_f32_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_f64_dump(ist_vptr this, ist_dumpctx dctx);

ist_string ist_cstring_dump(ist_vptr this, ist_dumpctx dctx);
ist_string ist_cstring_dump_ident(ist_vptr this, ist_dumpctx dctx);


#endif // ISC_DUMP_H