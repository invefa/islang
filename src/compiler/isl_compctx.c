#include "isl_compctx.h"

ist_compctx ist_compctx_consby_void() {
    return ist_compctx_{
        .modules  = ist_moduleList_consm(1),
        .compents = ist_compentList_consm(1),
    };
}
_isl_define_initby_createby_with_consby((), (), ist_compctx, void);

void ist_compctx_clean(ist_compctx* this) {
    if (!this) return;
    ist_moduleList_clean(&this->modules);
    ist_compentList_clean(&this->compents);
}
_isl_define_delete_with_clean(ist_compctx);


inline ist_compentRef ist_compctx_register_compent(ist_compctx* this, ist_compent cpent) {
    //TODO: add a compare.
    return ist_compentList_addm(&this->compents, cpent);
}
