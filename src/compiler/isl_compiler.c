#include "isl_compiler.h"
#include "isl_memgr.h"
#include "isl_overload.h"

void codegen_ast_expr(ist_compiler* this);


void ist_compiler_compile(ist_compiler* this) {
    if (!this->instream) this->instream = ist_instream_createc(16);
    if (!this->curpsent) return;
    isg_list_foreach (psent_p, this->curpsent->as.scope.stmts) {
        this->curpsent = *psent_p;
        codegen_ast_expr(this);
    }
}
