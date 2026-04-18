#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"

typedef ist_string (*ist_dump_fn)(void*, ist_strbuf, ist_usize*);

ist_string dump_u8(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_u16(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_u32(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_u64(void* valp, ist_strbuf buffer, ist_usize* idxptr);

ist_string dump_i8(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_i16(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_i32(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_i64(void* valp, ist_strbuf buffer, ist_usize* idxptr);

ist_string dump_f32(void* valp, ist_strbuf buffer, ist_usize* idxptr);
ist_string dump_f64(void* valp, ist_strbuf buffer, ist_usize* idxptr);

typedef enum ist_json_kind {
    JKIND_UNK,
    JKIND_OBJECT,
    JKIND_ARRAY,
    JKIND_STRING,
    JKIND_INT,
    JKIND_FLOAT,
    JKIND_BOOL,
    JKIND_TRUE,
    JKIND_FALSE,
} ist_json_kind;

typedef struct ist_dumpitem {
    ist_cstring   name;
    void*         valp;
    ist_json_kind kind;
    void*         fn;
} ist_dumpitem;

typedef struct ist_dumpimage {
    ist_dumpitem* items;
    ist_usize     count;
} ist_dumpimage;

ist_string ist_dumpimage_dump_json(ist_dumpimage* image, ist_strbuf buffer, ist_usize* idxptr);



#endif // ISC_DUMP_H