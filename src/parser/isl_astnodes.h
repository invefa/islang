
/* unknown */
manifest(UNKNOWN, (struct {}))

manifest(NODE_LIST, (struct {
             ist_astnode_type type;
             ist_location     location;
             ist_value        value;
             ist_astnode*     node_list;
             ist_usize        node_count;
         }))

manifest(BINARY_OPT, (struct {
             ist_astnode_type type;
             ist_location     location;
             ist_value_uint   opt_type;
             ist_astnode*     left_node;
             ist_astnode*     right_node;
         }))
manifest(LITERAL_ENT, (struct {
             ist_astnode_type type;
             ist_location     location;
             ist_value        value;
             ist_usize        literal_type;
             ist_astnode*     right;
         }))

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
