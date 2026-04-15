#include "isl_context.h"

inline ist_usize ist_context_register_compent(ist_context* this, ist_compent* _cpent) {
    return ist_compentptr_list_addm(&this->compent_list, _cpent);
}
