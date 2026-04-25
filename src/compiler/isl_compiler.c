#include "isl_compiler.h"
#include "isl_memgr.h"

ist_value_type _ast_expr_typecheck(ist_compiler* this) {
    switch (this->node->type) {
        case isl_astnt_unexpr: {
            ist_astnode_unexpr* expr = (ist_astnode_unexpr*)this->node;
        }
        case isl_astnt_binexpr:
        case isl_astnt_ternexpr: {
            ist_compent_defineby_full(res, expr, this->node->location);
            res->is_const = false;
            res->node     = this->node;
            ist_context_register_compent(this->ctx, (void*)res);
            break;
        }
        case isl_astnt_name: {
        }
        case isl_astnt_literal: {
            ist_astnode_literal* literal = (ist_astnode_literal*)this->node;
            return isl_toklitype_to_valtype[literal->litype];
        }
        default:
            isp_unreachable();
    }
    return isl_valtype_unk;
}


void elaborate_expr(ist_compiler* this) {
    switch (this->node->type) {
        case isl_astnt_unexpr:
        case isl_astnt_binexpr:
        case isl_astnt_ternexpr: {
            ist_compent_defineby_full(res, expr, this->node->location);
            res->is_const = false;
            res->node     = this->node;
            ist_context_register_compent(this->ctx, (void*)res);
            break;
        }
        default:
            isp_unreachable();
    }
}


void ist_compiler_elaborate(ist_compiler* this) {
    switch (this->node->type) {
        case isl_astnt_unexpr:
        case isl_astnt_binexpr:
        case isl_astnt_ternexpr:
            elaborate_expr(this);
            break;
        default:
            isp_unreachable();
            break;
    }
}

void ist_compiler_compile(ist_compiler* this) {}
