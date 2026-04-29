

/* unknown */
manifest(unk, (ist_usize))


/* tool node */
manifest(list, (struct { ist_parsentList this; }))

/* basic components, aka pattern parsent */
manifest(fnarg_list, (ist_astnodeAs_list))
manifest(fnparam_list, (ist_astnodeAs_list))
manifest(formparam_list, (ist_astnodeAs_list))
manifest(formarg_list, (ist_astnodeAs_list))

/* literal parsent */
manifest(literal, (struct { ist_tvalue this; }))

/* expr parsent */
manifest(expr, (struct {
             enum {
                 isn_psentExprKind_unary,
                 isn_psentExprKind_binary,
                 isn_psentExprKind_ternary,
                 isn_psentExprKind_fncall,
             } kind;
             union {
                 struct {
                     ist_tokenType op;
                     ist_astnode*  lhs;
                     ist_astnode*  rhs;
                 } unary;
                 struct {
                     ist_tokenType op;
                     ist_astnode*  lhs;
                     ist_astnode*  rhs;
                 } binary;
                 struct {
                     ist_tokenType op;
                     ist_astnode*  st;
                     ist_astnode*  nd;
                     ist_astnode*  rd;
                 } ternary;
                 struct {
                     ist_astnode*    fn;
                     ist_parsentList args;
                 } fncall;
             } as;
         }))

manifest(scope, (struct { ist_parsentList stmts; }))

// /* unknown */
// manifest(unknown, (ist_astnode))


// /* tool node */
// manifest(node_list, (struct {
//              ist_astnode         base;
//              ist_astnodeptr_list list;
//          }))

// /* basic components, aka pattern */
// manifest(arg_list_patt, (ist_astnode_node_list))
// manifest(params_list_patt, (ist_astnode_node_list))


// /* parsetime entities -> parsent */

// /* literal parsent */
// manifest(literal, (struct {
//              ist_astnode    base;
//              ist_tokenType litype: 8;
//              ist_value      value;
//          }))

// /* expression parsents */
// manifest(unexpr, (struct {
//              ist_astnode    base;
//              ist_tokenType optype: 8;
//              ist_bool       onlhs;
//              ist_astnode*   sub;
//          }))

// manifest(binexpr, (struct {
//              ist_astnode    base;
//              ist_tokenType optype: 8;
//              ist_astnode*   lhs;
//              ist_astnode*   rhs;
//          }))

// manifest(ternexpr, (struct {
//              ist_astnode    base;
//              ist_tokenType optype: 8;
//              ist_astnode*   first;
//              ist_astnode*   second;
//              ist_astnode*   third;
//          }))

// manifest(fncall_expr, (struct {
//              ist_astnode         base;
//              ist_astnode*        fn;
//              ist_astnodeptr_list arglist;
//          }))

// /* name parsent */
// manifest(name, (struct {
//              ist_astnode base;
//              ist_cstring name;
//          }))

// /* reference parsent */
// manifest(entref, (struct {
//              ist_astnode base;
//              ist_usize   index;
//          }))

// /* statement parsents */

// manifest(import_stmt, (struct {
//              ist_astnode base;
//              ist_string  path;
//          }))

// manifest(use_stmt, (struct {
//              ist_astnode  base;
//              ist_astnode* lhs;
//              ist_astnode* rhs;
//          }))

// manifest(do_stmt, (struct {
//              ist_astnode  base;
//              ist_astnode* expr;
//          }))

// /* other parsents */
// manifest(module, (ist_astnode_node_list))
// manifest(scope, (ist_astnode_node_list))
// manifest(space, (ist_astnode_node_list))
// manifest(fn, (ist_astnode_entref))
// manifest(span, (ist_astnode_entref))
// manifest(regist, (ist_astnode_entref))
// manifest(type, (ist_astnode_entref))
