#ifndef ISC_CONTEXT_H
#define ISC_CONTEXT_H

#include "isl_compent.h"
#include "isl_module.h"

typedef struct ist_compctx {
    ist_moduleList  modules;
    ist_compentList compents;
} ist_compctx;
#define ist_compctx_ (ist_compctx)

ist_compctx ist_compctx_consby_void();
_isl_declare_initby_createby_with_consby((), ist_compctx, void);

void ist_compctx_clean(ist_compctx* this);
_isl_declare_delete_with_clean(ist_compctx);

ist_compentRef ist_compctx_acquire_compentRef(ist_compctx* this);
ist_compent*   ist_compctx_acquire_compent(ist_compctx* this);
ist_compentRef ist_compctx_register_compent(ist_compctx* this, ist_compent cpent);

ist_moduleRef ist_compctx_acquire_moduletRef(ist_compctx* this);
ist_module*   ist_compctx_acquire_module(ist_compctx* this);
ist_moduleRef ist_compctx_register_module(ist_compctx* this, ist_module module);

#endif // ISC_CONTEXT_H