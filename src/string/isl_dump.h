#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"

typedef ist_string (*ist_dump_fn)(ist_vptr, ist_strbuf, ist_usize*, ist_usize);

ist_string dump_u8(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_u16(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_u32(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_u64(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);

ist_string dump_i8(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_i16(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_i32(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_i64(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);

ist_string dump_f32(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);
ist_string dump_f64(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth);

typedef enum ist_dumpkind {
    DKIND_UNK,
    DKIND_OBJECT,
    DKIND_ARRAY,
    DKIND_STRING,
    DKIND_INT,
    DKIND_FLOAT,
    DKIND_BOOL,
    DKIND_TRUE,
    DKIND_FALSE,
} ist_dumpkind;

typedef struct ist_dumpitem {
    ist_cstring  name;
    ist_dumpkind kind;
    ist_vptr     valp;
    ist_vptr     fn;
} ist_dumpitem;

typedef struct ist_dumpimage {
    ist_dumpitem* items;
    ist_usize     count;
    ist_cstring   name;
} ist_dumpimage;

typedef struct ist_dumplistpack {
    ist_vptr  listp;    // the pointer to isg_list
    ist_usize capacity; // the capacity of isg_list
} ist_dumplistpack;

ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count);

ist_string ist_dumpimage_dump_json(
    ist_dumpimage* image,
    ist_strbuf     buffer,
    ist_usize*     idxptr,
    ist_usize      depth
);

ist_string ist_dumpimage_dump_indent(
    ist_dumpimage* this,
    ist_strbuf buffer,
    ist_usize* idxptr,
    ist_usize  depth
);


#endif // ISC_DUMP_H