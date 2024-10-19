#define node      node
#define extractor extractor
#define protocol


/* unknown */
manifest(UNKNOWN, (struct {}), (protocol{}))

manifest(
    NODE_LIST,
    (struct {
        ist_astnode* node_list;
        ist_usize    node_count;
    }),
    (protocol {
        extractor.node_list  = node->left.node;
        extractor.node_count = node->value.uint_value;
    })
)

manifest(
    BINARY_OPT,
    (struct {
        ist_token_type optype;
        ist_astnode*   left;
        ist_astnode*   right;
    }),
    (protocol {
        extractor.optype = node->value.uint_value;
        extractor.left   = node->left.node;
        extractor.right  = node->right.node;
    })
)
manifest(
    LITERAL_ENT,
    (struct {
        ist_token_type type;
        ist_value      value;
    }),
    (protocol {
        extractor.type  = node->left.type;
        extractor.value = node->value;
    })
)

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

#undef node
#undef extractor
#undef protocol
