#ifndef ISC_ASTNODE_H
#define ISC_ASTNODE_H

#include "isl_macros.h"
#include "isl_memgr.h"
#include "isl_token.h"
#include "isl_types.h"
#include "isl_value.h"


/**
 * This enum only for hightlight the type of the astnode.
 * You can ignore it, it is unnecessary, and if there occurs some name conflict, remove it.
 */
enum {
#define manifest(_name, _struct) _name,
#include "isl_astnodes.h"
#undef manifest
};

/* define the type of the astnode */
typedef ist_usize ist_astnode_type;
enum ist_astnode_type {
#define manifest(_name, _struct) ISL_ASTNT_##_name,
#include "isl_astnodes.h"
#undef manifest
};

/* define the names of the astnode type */
extern const ist_string ist_astnode_type_names[];


/* define the base struct of the astnode */
typedef struct ist_astnode {
    ist_astnode_type type;
    ist_location     location;
} ist_astnode;


/* define all the astnode struct */
#define manifest(_name, _struct) typedef __ISL_MACRO_UNPACKAGER _struct IST_ASTNODE_##_name;
#include "isl_astnodes.h"
#undef manifest


/* constructor for the astnode base */
#define ist_astnode_consby_full(_type, _location) \
    ((ist_astnode){.type = (_type), .location = (_location)})

/**
 * Create an sub of astnode by the raw type and the location and optional cons.
 * Raw type means the suffix of the type enum, for example, if you want to create IST_ASTNODE_SCOPE,
 * you should provide the raw type as SCOPE.
 *
 * Optionally, you can provide the constructor argument to initialize the astnode,
 * just pass the argument in the form of a tuple to varg:cons.
 */
#define ist_astnode_createby_full(_raw_type, _location, _cons...)                              \
    ({                                                                                         \
        IST_ASTNODE_##_raw_type* __result__ = isl_calloc(IST_ASTNODE_##_raw_type);             \
        *(ist_astnode*)__result__ = ist_astnode_consby_full(ISL_ASTNT_##_raw_type, _location); \
        _cons;                                                                                 \
        (void*)__result__;                                                                     \
    })

/**
 * Make the node as pointer of the specific type of astnode.
 */
#define ist_astnode_as(_node, _raw_type) ((IST_ASTNODE_##_raw_type*)_node)

/* just as void ptr to make it more easy to pass */
#define isl_asvp(_adr) ((void*)_adr)

/**
 * Make the address as pointer of the specific type of astnode.
 * for all the astnode, you can use the fn to convert the address to the specific type.
 */
#define manifest(_name, _struct) IST_ASTNODE_##_name* ISL_AS_##_name(void* adr);
#include "isl_astnodes.h"
#undef manifest


void ist_astnode_delete(void* this);

ist_string* ist_astnode_dump(ist_astnode* this, ist_string* _buffer, ist_bool _deep);


#endif // ISC_ASTNODE_H