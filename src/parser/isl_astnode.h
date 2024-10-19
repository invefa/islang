#ifndef ISC_ASTNODE_H
#define ISC_ASTNODE_H

#include "isl_token.h"
#include "isl_types.h"
#include "isl_value.h"


typedef ist_u8 ist_astnode_type;
enum ist_astnode_type {
#define manifest(_name) ISL_ASTNT_##_name,
#include "isl_astnodes.h"
#undef manifest
};

extern const ist_string ist_astnode_type_names[];

// pre-declaration for ist_astnode_side
struct ist_astnode;
typedef struct ist_astnode ist_astnode;

typedef union ist_astnode_side {
    struct ist_astnode* node;
    ist_usize           size;
    ist_usize           type;
    ist_usize           index;
} ist_astnode_side;

#define ist_astnode_side_consby_null()        ((ist_astnode_side){.node = NULL})
#define ist_astnode_side_consby_node(_node)   ((ist_astnode_side){.node = (_node)})
#define ist_astnode_side_consby_size(_size)   ((ist_astnode_side){.size = (_size)})
#define ist_astnode_side_consby_type(_type)   ((ist_astnode_side){.type = (_type)})
#define ist_astnode_side_consby_index(_index) ((ist_astnode_side){.index = (_index)})

struct ist_astnode {
    ist_astnode_type type;
    ist_value        value;
    ist_astnode_side left;
    ist_astnode_side right;
    ist_location     location;
};


#define ist_astnode_consby_full(_type, _value, _left, _right, _location) \
    ((ist_astnode                                                        \
    ){.type     = (_type),                                               \
      .value    = (_value),                                              \
      .left     = (_left),                                               \
      .right    = (_right),                                              \
      .location = (_location)})

#define ist_astnode_consby_null()       \
    ist_astnode_consby_full(            \
        ISL_ASTNT_UNKNOWN,              \
        ist_value_consby_null(),        \
        ist_astnode_side_consby_null(), \
        ist_astnode_side_consby_null(), \
        ist_location_consby_null()      \
    )

ist_astnode* ist_astnode_initby_full(
    ist_astnode* this,
    ist_astnode_type _type,
    ist_value        _value,
    ist_astnode_side _left,
    ist_astnode_side _right,
    ist_location     _location
);
ist_astnode* ist_astnode_createby_full(
    ist_astnode_type _type,
    ist_value        _value,
    ist_astnode_side _left,
    ist_astnode_side _right,
    ist_location     _location
);


ist_string* ist_astnode_dump(ist_astnode* this, ist_string* _buffer, ist_bool _deep);


#endif // ISC_ASTNODE_H