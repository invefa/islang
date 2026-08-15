#include "isl_compiler.h"

#define sem_info(_fmt, _vargs...)  isl_report(rid_semantic_info, _fmt, ##_vargs)
#define sem_note(_fmt, _vargs...)  isl_report(rid_semantic_note, _fmt, ##_vargs)
#define sem_warn(_fmt, _vargs...)  isl_report(rid_semantic_warn, _fmt, ##_vargs)
#define sem_error(_fmt, _vargs...) isl_report(rid_semantic_error, _fmt, ##_vargs)
#define sem_panic(_fmt, _vargs...) isl_report(rid_semantic_panic, _fmt, ##_vargs)
#define sem_fatal(_fmt, _vargs...) isl_report(rid_semantic_fatal, _fmt, ##_vargs)

ist_semtree elab_scope(ist_compiler* this, ist_parsent node);
ist_semtree elab_expr(ist_compiler* this, ist_parsent node);
ist_semtree elab_binexpr(ist_compiler* this, ist_parsent node);
ist_semtree elab_unexpr(ist_compiler* this, ist_parsent node);
ist_semtree elab_literal(ist_compiler* this, ist_parsent node);

void ist_compiler_elaborate(ist_compiler* this) {}

ist_semtree elab_expr(ist_compiler* this, ist_parsent node) {
    return null;
}

ist_semtree elab_binexpr(ist_compiler* this, ist_parsent node) {
    isl_assert(
        node && node->kind == ist_astnodeKind_expr && node->as.expr.kind == isn_psentExprKind_binary
    );

    struct ist_astnodeAs_exprAs_binary expr = node->as.expr.as.binary;

    ist_semtree lhs = elab_expr(this, expr.lhs);
    ist_semtree rhs = elab_expr(this, expr.rhs);

    return null;
}