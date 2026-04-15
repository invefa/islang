#ifndef ISC_CONTEXT_H
#define ISC_CONTEXT_H

#include "isl_compent.h"

typedef struct ist_context {
    ist_compentptr_list compent_list;

} ist_context;

ist_usize ist_context_register_compent(ist_context* this, ist_compent* _cpent);

#endif // ISC_CONTEXT_H