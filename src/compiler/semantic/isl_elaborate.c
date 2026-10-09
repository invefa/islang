#include "isl_compiler.h"

#define info(_fmt, _vargs...)  isl_report(rid_semantic_info, _fmt, ##_vargs)
#define note(_fmt, _vargs...)  isl_report(rid_semantic_note, _fmt, ##_vargs)
#define warn(_fmt, _vargs...)  isl_report(rid_semantic_warn, _fmt, ##_vargs)
#define error(_fmt, _vargs...) isl_report(rid_semantic_error, _fmt, ##_vargs)
#define panic(_fmt, _vargs...) isl_report(rid_semantic_panic, _fmt, ##_vargs)
#define fatal(_fmt, _vargs...) isl_report(rid_semantic_fatal, _fmt, ##_vargs)

ist_semtree elab_scope(ist_compiler* this, ist_parsent node);
ist_semtree elab_expr(ist_compiler* this, ist_parsent node);
ist_semtree elab_expr_binary(ist_compiler* this, ist_parsent node);
ist_semtree elab_expr_unary(ist_compiler* this, ist_parsent node);
ist_semtree elab_literal(ist_compiler* this, ist_parsent node);

void ist_compiler_elaborate(ist_compiler* this) {
    isl_assert(this && this->curpsent);
    switch (this->curpsent->kind) {
        case isn_astnodeKind_scope:
            this->cursemt = elab_scope(this, this->curpsent);
            break;
        case isn_astnodeKind_expr:
            this->cursemt = elab_expr(this, this->curpsent);
            break;
        case isn_astnodeKind_literal:
            this->cursemt = elab_literal(this, this->curpsent);
            break;
        case isn_astnodeKind_unk:
        default:
            isp_unreachable();
    }
}

ist_semtree elab_expr(ist_compiler* this, ist_parsent node) {
    isl_assert(this && node);
    isl_assert(node->kind == isn_astnodeKind_expr);
    switch (node->as.expr.kind) {
        case isn_psentExprKind_unary:
            return elab_expr_unary(this, node);
            break;
        case isn_psentExprKind_binary:
            return elab_expr_binary(this, node);
            break;
        default:
            isp_unreachable();
    }

    return null;
}

ist_semtree elab_expr_binary(ist_compiler* this, ist_parsent node) {
    isl_assert(this && node);
    isl_assert(node->kind == isn_astnodeKind_expr);
    isl_assert(node->as.expr.kind == isn_psentExprKind_binary);

    struct ist_astnodeAs_exprAs_binary binary = node->as.expr.as.binary;

    ist_semtree lhs = elab_expr(this, binary.lhs);
    ist_semtree rhs = elab_expr(this, binary.rhs);

    ist_semnodeAs_expr ret = {.opkind = binary.op};

    _ISL_MAYBE_UNUSED lhs;
    _ISL_MAYBE_UNUSED rhs;

    return null;
}

ist_semtree elab_expr_unary(ist_compiler* this, ist_parsent node) {
    return null;
}

ist_semtree elab_literal(ist_compiler* this, ist_parsent node) {
    return null;
}

ist_semtree elab_scope(ist_compiler* this, ist_parsent node) {
    return null;
}