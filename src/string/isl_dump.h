#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"

typedef enum ist_dumpkind {
    DKIND_UNK,
    DKIND_JSON,
    DKIND_YAML,
} ist_dumpkind;

typedef ist_string (*ist_dumper)(ist_vptr, ist_strbuf, ist_usize*, ist_usize, ist_dumpkind);

ist_string ist_u8_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_u16_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_u32_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_u64_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);

ist_string ist_i8_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_i16_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_i32_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_i64_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);

ist_string ist_f32_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);
ist_string ist_f64_dump(
    void*        valp,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);

ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count);

ist_string ist_cstring_dump(
    ist_cstring* this,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);

/* name: dumper(valp) */
typedef struct ist_dumpitem {
    ist_cstring name;
    ist_vptr    dumper;
    ist_vptr    valp;
} ist_dumpitem;

typedef struct ist_dumpimage {
    ist_dumpitem* items;
    ist_usize     count;
    ist_cstring   name; // optional name for this dump object
} ist_dumpimage;


ist_string ist_dumpimage_dump(
    ist_dumpimage* this,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);

typedef struct isg_list_dumpack {
    ist_vptr  listp;    // the pointer to isg_list
    ist_usize capacity; // the capacity of isg_list
    ist_vptr  dumper;   // the dumper fn for element of list
} isg_list_dumpack;

ist_string isg_list_dumpack_dump(
    isg_list_dumpack* this,
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_usize    depth,
    ist_dumpkind style
);


#endif // ISC_DUMP_H