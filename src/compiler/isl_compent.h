#ifndef ISC_COMPENT_H
#define ISC_COMPENT_H
#include "isl_string.h"
#include "isl_types.h"


typedef struct ist_compent_ref {
    ist_usize ref_index;
}

typedef struct ist_compent_name {
    ist_string name;
    ist_usize  index;
    void*      space;
}

typedef struct ist_compent_fn {
    ist_usize ;
}



#endif // ISC_COMPENT_H