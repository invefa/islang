#include "isl_compiler.h"
#include "isl_memgr.h"

void elaborate_expr(ist_compiler* this) {
    switch (this->node->type) {
        case isl_astnt_unary_expr:
        case isl_astnt_binary_expr:
        case isl_astnt_ternary_expr:
            ist_context_register_compent(
                this->ctx, ist_compent_createby_full(expr, this->node->location, res, ({
                                                         res->is_const = false;
                                                         res->node     = this->node;
                                                     }))
                // ist_compent_createm(
                //     expr,

                //     ((ist_compent_expr){
                //         .base.kind     = isl_cpentkind_expr,
                //         .base.location = this->node->location,
                //         .is_const      = false,
                //         .node          = this->node,
                //     })
                // )
            );
            break;
        default:
            isp_unreachable();
    }
}


void ist_compiler_elaborate(ist_compiler* this) {
    switch (this->node->type) {
        case isl_astnt_binary_expr:

            break;
        default:
            isp_unreachable();
            break;
    }
}

void ist_compiler_compile(ist_compiler* this) {}
