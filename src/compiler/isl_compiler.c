#include "isl_compiler.h"
#include "isl_memgr.h"
#include "isl_overload.h"


#define _INST_UNWRAPER(_size, _data) (ist_instream_append_##_size(this->instream, _data))
#define _INST_GEN_0()                NULL
#define _INST_GEN_1(_1)              _INST_UNWRAPER _1
#define _INST_GEN_2(_1, _2)          _INST_UNWRAPER _1, _INST_UNWRAPER _2
#define _INST_GEN_3(_1, _2, _3)      _INST_UNWRAPER _1, _INST_UNWRAPER _2, _INST_UNWRAPER _3
#define _INST_GEN(_args...)          _isl_overload(_INST_GEN, ##_args)
/**
 * codegen macro for emit instruction easily.
 */
#define inst(_inst, _vargs...)                          \
    do {                                                \
        ist_instream_append_ins(this->instream, _inst); \
        _INST_GEN(_vargs);                              \
    } while (0)

#define nodeas(_type, _name) _type* _name = (_type*)(this->node)


ist_value_type typecheck_ast_expr(ist_compiler* this, ist_astnode* node) {
    switch (node->type) {
        case isl_astnt_unexpr: {
            ist_astnode_unexpr* expr = (ist_astnode_unexpr*)node;
            return typecheck_ast_expr(this, expr->sub);
        }
        case isl_astnt_binexpr: {
            ist_astnode_binexpr* expr     = (ist_astnode_binexpr*)node;
            ist_value_type       lhs_type = typecheck_ast_expr(this, expr->lhs);
            ist_value_type       rhs_type = typecheck_ast_expr(this, expr->rhs);
            if (lhs_type != rhs_type) isl_report(rid_binexpr_type_unmatch);
            else return lhs_type;
        }

        case isl_astnt_literal: {
            ist_astnode_literal* literal = (ist_astnode_literal*)this->node;
            return isl_toklitype_to_valtype[literal->litype];
        }
        default:
            isp_unreachable();
    }
    return isl_valtype_void;
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



void ist_compiler_evalualte(ist_compiler* this) {}



void codegen_ast_expr(ist_compiler* this);
void codegen_ast_unexpr(ist_compiler* this);
void codegen_ast_binexpr(ist_compiler* this);
void codegen_ast_ternexpr(ist_compiler* this);
void codegen_ast_literal(ist_compiler* this);


void codegen_ast_unexpr(ist_compiler* this) {
    if (!this->node) return;
    nodeas(ist_astnode_unexpr, expr);

    this->node = expr->sub;
    codegen_ast_expr(this);

    switch (expr->optype) {
        case ISL_TOKENT_ADD:
            break;
        case ISL_TOKENT_SUB:
            // TODO: ist_instream_append_ins(this->instream, ist_inst_neg);
            break;
        default:
            isp_unreachable();
            break;
    }
}


void codegen_ast_binexpr(ist_compiler* this) {
    if (!this->node) return;
    nodeas(ist_astnode_binexpr, expr);

    this->node = expr->lhs;
    codegen_ast_expr(this);
    this->node = expr->rhs;
    codegen_ast_expr(this);

    switch (expr->optype) {
        case ISL_TOKENT_ADD:
            inst(ist_inst_addi);
            break;
        case ISL_TOKENT_SUB:
            inst(ist_inst_subi);
            break;
        case ISL_TOKENT_MUL:
            inst(ist_inst_muli);
            break;
        case ISL_TOKENT_DIV:
            inst(ist_inst_divi);
            break;
        default:
            isp_unreachable();
            break;
    }
}

void codegen_ast_literal(ist_compiler* this) {
    if (!this->node) return;
    nodeas(ist_astnode_literal, literal);

    switch (literal->litype) {
        case ISL_TOKENT_VL_INT:
            inst(ist_inst_pshi, (i64, literal->value.as_i64));
            break;
        case ISL_TOKENT_VL_REAL:
            inst(ist_inst_pshd, (i64, literal->value.as_f64));
            break;
        default:
            isp_unreachable();
            break;
    }
}

void codegen_ast_expr(ist_compiler* this) {
    if (!this->node) return;
    switch (this->node->type) {
        case isl_astnt_unexpr:
            codegen_ast_unexpr(this);
            break;
        case isl_astnt_binexpr:
            codegen_ast_binexpr(this);
            break;
        case isl_astnt_ternexpr:
            // TODO: ternary expression
            break;
        case isl_astnt_literal:
            codegen_ast_literal(this);
            break;
        default:
            isp_unreachable();
            break;
    }
}

void ist_compiler_codegen(ist_compiler* this) {}


void ist_compiler_compile(ist_compiler* this) {
    if (!this->instream) this->instream = ist_instream_calloc(16);
    if (!this->node) return;
    codegen_ast_expr(this);
}
