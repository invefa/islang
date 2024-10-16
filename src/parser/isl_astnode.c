#include "isl_astnode.h"

ist_astnode ist_astnode_consby_full(
    ist_astnode_type    _type,
    ist_value           _value,
    struct ist_astnode* _left,
    struct ist_astnode* _right
) {
    return (ist_astnode){.type = _type, .value = _value, .left = _left, .right = _right};
}
ist_astnode* ist_astnode_initby_full(
    ist_astnode* this,
    ist_astnode_type    _type,
    ist_value           _value,
    struct ist_astnode* _left,
    struct ist_astnode* _right
);
ist_astnode* ist_astnode_createby_full(
    ist_astnode_type    _type,
    ist_value           _value,
    struct ist_astnode* _left,
    struct ist_astnode* _right
);