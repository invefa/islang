

/* unknown */
manifest(unk, (ist_usize))


/* tool node */
manifest(list, (struct { ist_parsentList this; }))

/* basic components, aka pattern parsent */
manifest(fnarg_list, (ist_astnodeAs_list))
manifest(fnparam_list, (ist_astnodeAs_list))
manifest(formparam_list, (ist_astnodeAs_list))
manifest(formarg_list, (ist_astnodeAs_list))

/* some basic parsent */
manifest(scope, (struct { ist_parsentList stmts; }))
manifest(literal, (struct { ist_tvalue this; }))
manifest(name, (struct { ist_cstring name; }))

/* expr parsent */
manifest(expr, (struct {
             enum ist_astnodeAs_exprKind {
                 isn_psentExprKind_unary,
                 isn_psentExprKind_binary,
                 isn_psentExprKind_ternary,
                 isn_psentExprKind_fncall,
             } kind;
             union {
                 struct ist_astnodeAs_exprAs_unary {
                     ist_tokenType op;
                     ist_astnode*  lhs;
                     ist_astnode*  rhs;
                 } unary;
                 struct ist_astnodeAs_exprAs_binary {
                     ist_tokenType op;
                     ist_astnode*  lhs;
                     ist_astnode*  rhs;
                 } binary;
                 struct ist_astnodeAs_exprAs_ternary {
                     ist_tokenType op;
                     ist_astnode*  st;
                     ist_astnode*  nd;
                     ist_astnode*  rd;
                 }* ternary;
                 struct ist_astnodeAs_exprAs_fncall {
                     ist_astnode*    fn;
                     ist_parsentList args;
                 } fncall;
             } as;
         }))

manifest(use_stmt, (struct {
             ist_astnode* lhs;
             ist_astnode* rhs;
         }))

manifest(do_stmt, (struct { ist_astnode* expr; }))
