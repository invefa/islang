#ifndef ISC_CONTEXT_H
#define ISC_CONTEXT_H

#include "isl_compent.h"
#include "isl_module.h"

typedef struct ist_context {
    ist_moduleList  modules;
    ist_compentList compents;
} ist_context;
#define ist_context_ (ist_context)

ist_context ist_context_consby_void();
_isl_declare_initby_createby_with_consby((), ist_context, void);

void ist_context_clean(ist_context* this);
_isl_declare_delete_with_clean(ist_context);

ist_compentRef ist_context_acquire_compentRef(ist_context* this);
ist_compent*   ist_context_acquire_compent(ist_context* this);
ist_compentRef ist_context_register_compent(ist_context* this, ist_compent cpent);

ist_moduleRef ist_context_acquire_moduletRef(ist_context* this);
ist_module*   ist_context_acquire_module(ist_context* this);
ist_moduleRef ist_context_register_module(ist_context* this, ist_module module);

#endif // ISC_CONTEXT_H