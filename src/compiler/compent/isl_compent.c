#include "isl_compent.h"

#define ISG_STRUCT_NAME ist_compentRefList
#define ISG_VALUE_TYPE  ist_compentRef
#define ISG_VALUE_DONT_CLEAN
#include "isg_list_code.h"

#define ISG_STRUCT_NAME              ist_compentPtrList
#define ISG_VALUE_TYPE               ist_compent*
#define ISG_VALUE_FN_CLEAN(cpent_pp) ist_compent_clean(*(cpent_pp))
#include "isg_list_code.h"

#define ISG_STRUCT_NAME             ist_compentList
#define ISG_VALUE_TYPE              ist_compent
#define ISG_VALUE_FN_CLEAN(cpent_p) ist_compent_clean(cpent_p)
#include "isg_list_code.h"

ist_compent ist_compent_consby_full(ist_compentKind kind, ist_location loc, ist_compentAs as) {
    return ist_compent_{.kind = kind, .location = loc, .as = as};
}
_isl_define_initby_createby_with_consby(
    (ist_compentKind kind, ist_location loc, ist_compentAs as),
    (kind, loc, as),
    ist_compent,
    full
);

void ist_compent_clean(ist_compent* this) {
    switch (this->kind) {
        case ist_compentKind_unk:
            break;
        case ist_compentKind_entref:
        case ist_compentKind_name:
        case ist_compentKind_expr:
        default:
            isp_unreachable();
    }
}

void ist_compent_delete(ist_compent* this) {
    if (!this) return;
    ist_compent_clean(this);
    isl_free(this);
}
