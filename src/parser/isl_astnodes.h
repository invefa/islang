
/* unknown */
manifest(unknown, (ist_astnode))


/* tool node */
manifest(node_list, (struct {
             ist_astnode         base;
             ist_astnodeptr_list list;
         }))

/* basic components, aka pattern */
manifest(arg_list_patt, (ist_astnode_node_list))
manifest(params_list_patt, (ist_astnode_node_list))


/* parsetime entities -> parsent */

/* literal parsent */
manifest(literal, (struct {
             ist_astnode    base;
             ist_token_type litype: 8;
             ist_value      value;
         }))

/* expression parsents */
manifest(unexpr, (struct {
             ist_astnode    base;
             ist_token_type optype: 8;
             ist_bool       onlhs;
             ist_astnode*   sub;
         }))

manifest(binexpr, (struct {
             ist_astnode    base;
             ist_token_type optype: 8;
             ist_astnode*   lhs;
             ist_astnode*   rhs;
         }))

manifest(ternexpr, (struct {
             ist_astnode    base;
             ist_token_type optype: 8;
             ist_astnode*   first;
             ist_astnode*   second;
             ist_astnode*   third;
         }))

manifest(fncall_expr, (struct {
             ist_astnode         base;
             ist_astnode*        fn;
             ist_astnodeptr_list arglist;
         }))

/* name parsent */
manifest(name, (struct {
             ist_astnode base;
             ist_cstring name;
         }))

/* reference parsent */
manifest(entref, (struct {
             ist_astnode base;
             ist_usize   index;
         }))

/* statement parsents */

manifest(import_stmt, (struct {
             ist_astnode base;
             ist_string  path;
         }))

manifest(use_stmt, (struct {
             ist_astnode  base;
             ist_astnode* lhs;
             ist_astnode* rhs;
         }))

manifest(do_stmt, (struct {
             ist_astnode  base;
             ist_astnode* expr;
         }))

/* other parsents */
manifest(module, (ist_astnode_node_list))
manifest(scope, (ist_astnode_node_list))
manifest(space, (ist_astnode_node_list))
manifest(fn, (ist_astnode_entref))
manifest(span, (ist_astnode_entref))
manifest(regist, (ist_astnode_entref))
manifest(type, (ist_astnode_entref))
