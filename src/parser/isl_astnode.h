#ifndef ISC_ASTNODE_H
#define ISC_ASTNODE_H

#include "isl_macros.h"
#include "isl_token.h"
#include "isl_types.h"
#include "isl_value.h"


typedef ist_usize ist_astnode_type;
enum ist_astnode_type {
#define manifest(_name, _struct) ISL_ASTNT_##_name,
#include "isl_astnodes.h"
#undef manifest
};

extern const ist_string ist_astnode_type_names[];

typedef struct ist_astnode {
    ist_astnode_type type;
    ist_location     location;
} ist_astnode;

#define manifest(_name, _struct) typedef __ISL_MACRO_UNPACKAGER _struct IST_ASTNODE_##_name;
#include "isl_astnodes.h"
#undef manifest

void ist_astnode_delete(void* this);

ist_string* ist_astnode_dump(ist_astnode* this, ist_string* _buffer, ist_bool _deep);


#endif // ISC_ASTNODE_H