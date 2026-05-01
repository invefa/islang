#include "isl_compiler.h"
#include "isl_memgr.h"
#include "isl_overload.h"


#define _INST_UNWRAPER(_size, _data) (ist_instream_append_##_size(this->instream, _data))
#define _INST_GEN_0()                _ISL_MAYBE_UNUSED NULL
#define _INST_GEN_1(_1)              _INST_UNWRAPER _1
#define _INST_GEN_2(_1, _2)          _INST_UNWRAPER _1, _INST_UNWRAPER _2
#define _INST_GEN_3(_1, _2, _3)      _INST_UNWRAPER _1, _INST_UNWRAPER _2, _INST_UNWRAPER _3
#define _INST_GEN(_args...)          _isl_overload(_INST_GEN, ##_args)
/**
 * codegen macro for emit instruction easily.
 */
#define inst(_inst, _vargs...) ist_instream_append_ins(this->instream, _inst), _INST_GEN(_vargs)


void codegen_ast_expr(ist_compiler* this);
void codegen_ast_unexpr(ist_compiler* this);
void codegen_ast_binexpr(ist_compiler* this);
void codegen_ast_ternexpr(ist_compiler* this);
void codegen_ast_literal(ist_compiler* this);


void codegen_ast_unexpr(ist_compiler* this) {
    if (!this->node) return;
    ist_astnode* node = this->node;

    if (node->as.expr.as.unary.lhs) this->node = node->as.expr.as.unary.lhs;
    else this->node = node->as.expr.as.unary.rhs;
    codegen_ast_expr(this);

    switch (node->as.expr.as.unary.op) {
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
    ist_astnode* node = this->node;

    this->node = node->as.expr.as.binary.lhs;
    codegen_ast_expr(this);
    this->node = node->as.expr.as.binary.rhs;
    codegen_ast_expr(this);

    switch (node->as.expr.as.binary.op) {
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
    ist_astnode* node = this->node;

    switch (node->as.literal.this.type) {
        case isl_valtype_i64:
            inst(ist_inst_pshi, (i64, node->as.literal.this.data.as_i64));
            break;
        case isl_valtype_f64:
            inst(ist_inst_pshd, (f64, node->as.literal.this.data.as_f64));
            break;
        default:
            isp_unreachable();
            break;
    }
}

void codegen_ast_expr(ist_compiler* this) {
    if (!this->node) return;
    switch (this->node->kind) {
        case ist_astnodeKind_expr:
            switch (this->node->as.expr.kind) {
                case isn_psentExprKind_unary:
                    codegen_ast_unexpr(this);
                    break;
                case isn_psentExprKind_binary:
                    codegen_ast_binexpr(this);
                    break;
                default:
                    isp_unreachable();
            }
            break;
        case ist_astnodeKind_literal:
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
    isg_list_foreach (psent_p, this->node->as.scope.stmts) {
        this->node = *psent_p;
        codegen_ast_expr(this);
    }
}
