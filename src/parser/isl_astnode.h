#ifndef ISL_ASTNODE_H
#define ISL_ASTNODE_H

#include "isl_types.h"
#include "isl_value.h"

typedef ist_u8 ist_astnode_type;
enum ist_astnode_type {
    ISL_ASTNT_UNKNOWN,
    ISL_ASTNT_ROOT,

    ISL_ASTNT_VL_INT,
    ISL_ASTNT_VL_REAL,
    ISL_ASTNT_VL_STRING,
    ISL_ASTNT_VL_BYTE,
    ISL_ASTNT_VL_BOOL,
    ISL_ASTNT_VL_NULL,

    ISL_ASTNT_STMT_IMPORT,
    ISL_ASTNT_STMT_USING,

    ISL_ASTNT_OP_ADD,
    ISL_ASTNT_OP_SUB,
    ISL_ASTNT_OP_MUL,
    ISL_ASTNT_OP_DIV,
    ISL_ASTNT_OP_MOD,
    ISL_ASTNT_OP_ASSIGN,
};


typedef struct ist_astnode {
    ist_astnode_type    type;
    ist_value           value;
    struct ist_astnode* left;
    struct ist_astnode* right;
} ist_astnode;

ist_astnode ist_astnode_consby_full(
        ist_astnode_type    _type,
        ist_value           _value,
        struct ist_astnode* _left,
        struct ist_astnode* _right);
ist_astnode* ist_astnode_initby_full(
        ist_astnode* this,
        ist_astnode_type    _type,
        ist_value           _value,
        struct ist_astnode* _left,
        struct ist_astnode* _right);
ist_astnode* ist_astnode_createby_full(
        ist_astnode_type    _type,
        ist_value           _value,
        struct ist_astnode* _left,
        struct ist_astnode* _right);

#endif // ISL_ASTNODE_H