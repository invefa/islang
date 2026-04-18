#ifndef ISC_DUMP_H
#define ISC_DUMP_H

#include "isl_string.h"


typedef enum json_kind {
    JKIND_UNK,
    JKIND_ENTRY,
    JKIND_OBJECT,
    JKIND_ARRAY,
    JKIND_STRING,
    JKIND_INT,
    JKIND_FLOAT,
    JKIND_BOOL,
    JKIND_TRUE,
    JKIND_FALSE,
} json_kind;


ist_string dump_json(
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_cstring* name_list,
    void**       valp_list,
    json_kind*   kind_list,
    void*        fn_list,
    ist_usize    count
);


#endif // ISC_DUMP_H