#include "isl_astnode.h"
#include "isl_memgr.h"

const ist_string ist_astnode_type_names[] = {
#define manifest(_name, _struct) [ISL_ASTNT_##_name] = #_name,
#include "isl_astnodes.h"
#undef manifest
};

void ist_astnode_delete(void* this) {
    switch (*(ist_astnode_type*)this) {
#define manifest(_name, _struct)              \
    case ISL_ASTNT_##_name:                   \
        isl_free((IST_ASTNODE_##_name*)this); \
        break;
#include "isl_astnodes.h"
#undef manifest
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