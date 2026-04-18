#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"

typedef ist_string (*ist_dump_fn)(ist_strbuf, void*);

ist_string dump_u8(ist_strbuf tbuffer, void* valp);
ist_string dump_u16(ist_strbuf tbuffer, void* valp);
ist_string dump_u32(ist_strbuf tbuffer, void* valp);
ist_string dump_u64(ist_strbuf tbuffer, void* valp);

ist_string dump_i8(ist_strbuf tbuffer, void* valp);
ist_string dump_i16(ist_strbuf tbuffer, void* valp);
ist_string dump_i32(ist_strbuf tbuffer, void* valp);
ist_string dump_i64(ist_strbuf tbuffer, void* valp);

ist_string dump_f32(ist_strbuf tbuffer, void* valp);
ist_string dump_f64(ist_strbuf tbuffer, void* valp);

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

typedef struct ist_dump_item {
    ist_cstring   name;
    void*         valp;
    ist_json_kind kind;
    void*         fn;
} ist_dump_item;

typedef struct ist_dump_item_clist {
    ist_dump_item* data;
    ist_usize      count;
} ist_dump_item_clist;

ist_string dump_json(
    ist_strbuf     buffer,
    ist_usize*     idxptr,
    ist_dump_item* item_list,
    ist_usize      count
);



#endif // ISC_DUMP_H