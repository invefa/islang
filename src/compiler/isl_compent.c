#include "isl_compent.h"

#define ISG_STRUCT_NAME             ist_compentptr_list
#define ISG_VALUE_TYPE              ist_compent*
#define ISG_VALUE_FN_CLEAN(cpentpp) ist_compent_clean(*(cpentpp))
#include "isg_list_code.h"

void ist_compent_clean(ist_compent* this) {
    switch (0 [(ist_compent_kindenum*)this]) {
        case isl_cpentkind_unknown:
        case isl_cpentkind_entref:
        case isl_cpentkind_name:
        case isl_cpentkind_expr:
            break;
        default:
            isp_unreachable();
    }
}