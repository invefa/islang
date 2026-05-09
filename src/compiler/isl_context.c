#include "isl_context.h"

ist_context ist_context_consby_void() {
    return ist_context_{
        .modules  = ist_moduleList_consm(1),
        .compents = ist_compentList_consm(1),
    };
}
_isl_define_initby_createby_with_consby((), (), ist_context, void);

void ist_context_clean(ist_context* this) {
    if (!this) return;
    ist_moduleList_clean(&this->modules);
    ist_compentList_clean(&this->compents);
}
_isl_define_delete_with_clean(ist_context);


inline ist_compentRef ist_context_register_compent(ist_context* this, ist_compent cpent) {
    //TODO: add a compare.
    return ist_compentList_addm(&this->compents, cpent);
}
