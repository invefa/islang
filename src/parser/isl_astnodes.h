
/* unknown */
manifest(UNKNOWN, (ist_astnode))


/* tool node */
manifest(NODE_LIST, (struct {
             ist_astnode         base;
             ist_astnodeptr_list nodeptr_list;
         }))

/* basic components, aka pattern */
manifest(ARG_LIST_PATT, (IST_ASTNODE_NODE_LIST))
manifest(PARAM_LIST_PATT, (IST_ASTNODE_NODE_LIST))

/* expressions */
manifest(UNARY_EXPR, (struct {
             ist_astnode    base;
             ist_token_type optype: 8;
             ist_bool       onlhs;
             ist_astnode*   sub_node;
         }))

manifest(BINARY_EXPR, (struct {
             ist_astnode    base;
             ist_token_type optype: 8;
             ist_astnode*   lhs_node;
             ist_astnode*   rhs_node;
         }))

manifest(TERNARY_EXPR, (struct {
             ist_astnode    base;
             ist_token_type optype: 8;
             ist_astnode*   first_node;
             ist_astnode*   second_node;
             ist_astnode*   third_node;
         }))

manifest(FNCALL_EXPR, (struct {
             ist_astnode         base;
             ist_astnode*        fn_entity;
             ist_astnodeptr_list arglist;
         }))

/* comptime entities */
manifest(LITERAL_ENT, (struct {
             ist_astnode    base;
             ist_token_type litype: 8;
             ist_value      value;
         }))

manifest(NAME_ENT, (struct {
             ist_astnode base;
             ist_string  name;
         }))

manifest(REFERENCE_ENT, (struct {
             ist_astnode base;
             ist_usize   index;
         }))

manifest(SCOPE_ENT, (IST_ASTNODE_NODE_LIST))
manifest(MODULE_ENT, (IST_ASTNODE_NODE_LIST))

manifest(FN_ENT, (IST_ASTNODE_REFERENCE_ENT))
manifest(VAR_ENT, (IST_ASTNODE_REFERENCE_ENT))
manifest(REGIST_ENT, (IST_ASTNODE_REFERENCE_ENT))
manifest(TYPE_ENT, (IST_ASTNODE_REFERENCE_ENT))
