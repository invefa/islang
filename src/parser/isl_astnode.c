#include "isl_astnode.h"

#include "isl_list.h"
#include "isl_memgr.h"


const ist_string ist_astnode_type_names[] = {
#define manifest(_name, _struct) [ISL_ASTNT_##_name] = #_name,
#include "isl_astnodes.h"
#undef manifest
};

#define manifest(_name, _struct)                            \
    inline IST_ASTNODE_##_name* ISL_AS_##_name(void* adr) { \
        isl_assert(adr);                                    \
        return adr;                                         \
    }
#include "isl_astnodes.h"
#undef manifest

void ist_astnode_delete(void* this) {

    if (this == NULL) return;

    ist_astnode_type type = ((ist_astnode_type*)this)[0];

    switch (type) {
        case ISL_ASTNT_SCOPE:
        case ISL_ASTNT_MODULE:
        case ISL_ASTNT_ARG_LIST:
        case ISL_ASTNT_PARAM_LIST:
        case ISL_ASTNT_NODE_LIST: {
            IST_ASTNODE_NODE_LIST* node_list = (IST_ASTNODE_NODE_LIST*)this;
            isl_list_foreach (nodepp, node_list->nodeptr_list) ist_astnode_delete(*nodepp);
            isl_freev_list(node_list->nodeptr_list);
            break;
        }
        case ISL_ASTNT_UNARY_OPT:
            ist_astnode_delete(((IST_ASTNODE_UNARY_OPT*)this)->sub_node);
            break;
        case ISL_ASTNT_BINARY_OPT: {
            IST_ASTNODE_BINARY_OPT* binary_opt = (IST_ASTNODE_BINARY_OPT*)this;
            ist_astnode_delete(binary_opt->left_node);
            ist_astnode_delete(binary_opt->right_node);
            break;
        }
        case ISL_ASTNT_TERNARY_OPT: {
            IST_ASTNODE_TERNARY_OPT* ternary_opt = (IST_ASTNODE_TERNARY_OPT*)this;
            ist_astnode_delete(ternary_opt->first_node);
            ist_astnode_delete(ternary_opt->second_node);
            ist_astnode_delete(ternary_opt->third_node);
            break;
        }
    }

    switch (type) {

#define manifest(_name, _struct)              \
    case ISL_ASTNT_##_name:                   \
        isl_free((IST_ASTNODE_##_name*)this); \
        break;
#include "isl_astnodes.h"
#undef manifest

        default:
            isl_report(rid_unknown_enum_value, isp_catch_coreloc, type);
            break;
    }
}


// inline ist_astnode* ist_astnode_initby_full(
//     ist_astnode* this,
//     ist_astnode_type _type,
//     ist_location     _location,
//     ist_value        _value,
//     ist_astnode_side _left,
//     ist_astnode_side _right
// ) {
//     *this = ist_astnode_consby_full(_type, _location, _value, _left, _right);
//     return this;
// }
// inline ist_astnode* ist_astnode_createby_full(
//     ist_astnode_type _type,
//     ist_location     _location,
//     ist_value        _value,
//     ist_astnode_side _left,
//     ist_astnode_side _right
// ) {
//     return ist_astnode_initby_full(
//         isl_malloc(ist_astnode), _type, _location, _value, _left, _right
//     );
// }


// ist_string* ist_astnode_dump(ist_astnode* this, ist_string* _buffer, ist_bool _deep) {
//     return NULL;
// }