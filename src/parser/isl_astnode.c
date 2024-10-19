#include "isl_astnode.h"
#include "isl_memgr.h"

const ist_string ist_astnode_type_names[] = {
#define manifest(_name) [ISL_ASTNT_##_name] = #_name,
#include "isl_astnodes.h"
#undef manifest
};

ist_astnode* ist_astnode_initby_full(
    ist_astnode* this,
    ist_astnode_type _type,
    ist_value        _value,
    ist_astnode_side _left,
    ist_astnode_side _right,
    ist_location     _location
) {
    *this = ist_astnode_consby_full(_type, _value, _left, _right, _location);
    return this;
}
ist_astnode* ist_astnode_createby_full(
    ist_astnode_type _type,
    ist_value        _value,
    ist_astnode_side _left,
    ist_astnode_side _right,
    ist_location     _location
) {
    return ist_astnode_initby_full(
        isl_malloc(ist_astnode), _type, _value, _left, _right, _location
    );
}


ist_string* ist_astnode_dump(ist_astnode* this, ist_string* _buffer, ist_bool _deep) {
    return NULL;
}