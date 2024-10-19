
/* unknown */
manifest(UNKNOWN, (ist_astnode))


/* tool node */
manifest(NODE_LIST, (struct {
             ist_astnode  base;
             ist_astnode* node_list;
             ist_usize    node_count;
         }))

manifest(ARG_LIST, (IST_ASTNODE_NODE_LIST))
manifest(PARAM_LIST, (IST_ASTNODE_NODE_LIST))

/* basic components */
manifest(SCOPE, (IST_ASTNODE_NODE_LIST))
manifest(MODULE, (IST_ASTNODE_NODE_LIST))

/* operations */
manifest(UNARY_OPT, (struct {
             ist_astnode    base;
             ist_token_type operator_type;
             ist_astnode*   sub_node;
         }))
manifest(BINARY_OPT, (struct {
             ist_astnode    base;
             ist_token_type operator_type;
             ist_astnode*   left_node;
             ist_astnode*   right_node;
         }))
manifest(TERNARY_OPT, (struct {
             ist_astnode    base;
             ist_token_type operator_type;
             ist_astnode*   first_node;
             ist_astnode*   second_node;
             ist_astnode*   third_node;
         }))

/* comptime entities */
manifest(LITERAL_ENT, (struct {
             ist_astnode    base;
             ist_token_type literal_type;
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
manifest(FN_ENT, (IST_ASTNODE_REFERENCE_ENT))
manifest(VAR_ENT, (IST_ASTNODE_REFERENCE_ENT))
manifest(REGIST_ENT, (IST_ASTNODE_REFERENCE_ENT))
manifest(TYPE_ENT, (IST_ASTNODE_REFERENCE_ENT))

/*  unknown
manifest(UNKNOWN)

 basic components
manifest(NODE_LIST)
manifest(ARG_LIST)
manifest(PARAM_LIST)

 code structures
manifest(MODULE)
manifest(SCOPE)

 comptime entities
manifest(REFEERENCE_ENT)
manifest(NAME_ENT)
manifest(LITERAL_ENT)
manifest(FN_ENT)
manifest(VAR_ENT)
manifest(REGIST_ENT)
manifest(TYPE_ENT)

 operations
manifest(TERNARY_OPT)
manifest(BINARY_OPT)
manifest(LUNARY_OPT)
manifest(RUNARY_OPT)
*/
