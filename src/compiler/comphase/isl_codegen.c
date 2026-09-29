#include "isl_compiler.h"



#define _INST_UNWRAPER_IMPL(_size, _data) (ist_instream_append_##_size(this->instream, _data))
#define _INST_UNWRAPER(_pack)             _INST_UNWRAPER_IMPL _pack
#define _INST_GEN(_args...)               _ISL_UNPACK_FORWARD(_INST_UNWRAPER, ##_args)

/* codegen macro for emit instruction easily. */
#define inst(_inst, _vargs...) ist_instream_append_ins(this->instream, _inst), _INST_GEN(_vargs)



void codegen_ast_expr(ist_compiler* this);
void codegen_ast_unexpr(ist_compiler* this);
void codegen_ast_binexpr(ist_compiler* this);
void codegen_ast_ternexpr(ist_compiler* this);
void codegen_ast_literal(ist_compiler* this);


void codegen_ast_unexpr(ist_compiler* this) {
    if (!this->curpsent) return;
    ist_astnode* node = this->curpsent;

    if (node->as.expr.as.unary.lhs) this->curpsent = node->as.expr.as.unary.lhs;
    else this->curpsent = node->as.expr.as.unary.rhs;
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
    if (!this->curpsent) return;
    ist_astnode* node = this->curpsent;

    this->curpsent = node->as.expr.as.binary.lhs;
    codegen_ast_expr(this);
    this->curpsent = node->as.expr.as.binary.rhs;
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
    if (!this->curpsent) return;
    ist_astnode* node = this->curpsent;

    switch (node->as.literal.this.type) {
        case isn_valueType_i64:
            inst(ist_inst_pshi, (i64, node->as.literal.this.as.i64));
            break;
        case isn_valueType_f64:
            inst(ist_inst_pshd, (f64, node->as.literal.this.as.f64));
            break;
        default:
            isp_unreachable();
            break;
    }
}

void codegen_ast_expr(ist_compiler* this) {
    if (!this->curpsent) return;
    switch (this->curpsent->kind) {
        case ist_astnodeKind_expr:
            switch (this->curpsent->as.expr.kind) {
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
