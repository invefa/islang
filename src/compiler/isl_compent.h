#ifndef ISC_COMPENT_H
#define ISC_COMPENT_H
#include "isl_string.h"
#include "isl_types.h"


typedef struct ist_ref_compent {
    ist_usize ref_index;
} ist_ref_compent;

typedef struct ist_name_compent {
    ist_string name;
    ist_usize  index;
    void*      space;
} ist_name_compent;

typedef struct ist_fn_compent {
    ist_usize index;
} ist_fn_compent;



#endif // ISC_COMPENT_H