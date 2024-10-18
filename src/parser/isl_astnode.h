#ifndef ISL_ASTNODE_H
#define ISL_ASTNODE_H

#include "isl_token.h"
#include "isl_types.h"
#include "isl_value.h"


typedef ist_u8 ist_astnode_type;
enum ist_astnode_type {
    ISL_ASTNT_UNKNOWN,

    /* basic components */
    ISL_ASTNT_NODE_LIST,
    ISL_ASTNT_ARG_LIST,
    ISL_ASTNT_PARAM_LIST,

    /* code structures */
    ISL_ASTNT_MODULE,
    ISL_ASTNT_SCOPE,

    /* comptime entities */
    ISL_ASTNT_REFEERENCE_ENT,
    ISL_ASTNT_NAME_ENT,
    ISL_ASTNT_LITERAL_ENT,
    ISL_ASTNT_FN_ENT,
    ISL_ASTNT_VAR_ENT,
    ISL_ASTNT_REGIST_ENT,
    ISL_ASTNT_TYPE_ENT,

    /* operations */
    ISL_ASTNT_TERNARY_OPT,
    ISL_ASTNT_BINARY_OPT,
    ISL_ASTNT_LUNARY_OPT,
    ISL_ASTNT_RUNARY_OPT,

};

// pre-declaration for ist_astnode_side
struct ist_astnode;

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

typedef struct ist_astnode {
    ist_astnode_type type;
    ist_value        value;
    ist_astnode_side left;
    ist_astnode_side right;
    ist_location     location;
} ist_astnode;


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

ist_string* ist_astnode_dump(ist_astnode* this, ist_string* _buffer, ist_bool _dump_children);


#endif