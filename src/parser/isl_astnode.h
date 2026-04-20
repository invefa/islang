#ifndef ISC_ASTNODE_H
#define ISC_ASTNODE_H

#include "isl_dump.h"
#include "isl_token.h"

/**
 * This enum only for hightlight the type of the astnode.
 * You can ignore it, it is unnecessary, and if there occurs some name conflict, remove it.
 */
// enum {
// #define manifest(_name, _struct) _name,
// #include "isl_astnodes.h"
// #undef manifest
// };

/* define the type of the astnode, astnode-type enum */
typedef ist_usize ist_astnode_typenum;
enum ist_astnode_typenum {
#define manifest(_name, _struct) isl_astnt_##_name,
#include "isl_astnodes.h"
#undef manifest
};

/* define the names of the astnode type */
extern ist_string ist_astnode_type_names[];


/* define the base struct of the astnode */
typedef struct ist_astnode {
    ist_astnode_typenum type;
    ist_location        location;
} ist_astnode;

/* define the list of the astnodeptr */
#define ISG_STRUCT_NAME ist_astnodeptr_list
#define ISG_VALUE_TYPE  ist_astnode*
#include "isg_list_head.h"

/* define all the astnode struct */
#define manifest(_name, _struct) typedef __ISL_MACRO_UNPACKAGER _struct ist_astnode_##_name;
#include "isl_astnodes.h"
#undef manifest


/* constructor for the astnode base */
#define ist_astnode_consby_full(_type, _location) \
    ((ist_astnode){.type = (_type), .location = (_location)})

#define __IST_ASTNODE_CREATEBY_FULL_CONS_0()
#define __IST_ASTNODE_CREATEBY_FULL_CONS_1(_cons)        _cons
#define __IST_ASTNODE_CREATEBY_FULL_CONS_2(_name, _cons) _cons
#define __IST_ASTNODE_CREATEBY_FULL_CONS(_cons...) \
    _isl_overload(__IST_ASTNODE_CREATEBY_FULL_CONS, ##_cons)

#define __IST_ASTNODE_CREATEBY_FULL_RESULT_NAME_0()             __RESULT__
#define __IST_ASTNODE_CREATEBY_FULL_RESULT_NAME_1(_cons)        __RESULT__
#define __IST_ASTNODE_CREATEBY_FULL_RESULT_NAME_2(_name, _cons) _name
#define __IST_ASTNODE_CREATEBY_FULL_RESULT_NAME(_cons...) \
    _isl_overload(__IST_ASTNODE_CREATEBY_FULL_RESULT_NAME, ##_cons)
/**
 * Create an sub of astnode by the raw type and the location and optional cons.
 * Raw type means the suffix of the type enum, for example, if you want to create IST_ASTNODE_SCOPE,
 * you should provide the raw type as SCOPE_ENT.
 *
 * Optionally, you can provide the constructor argument to initialize the astnode,
 * just pass the argument in the form of a tuple to varg:cons.
 */
#define ist_astnode_createby_full(_raw_type, _location, _cons...)              \
    ({                                                                         \
        ist_astnode_##_raw_type* __IST_ASTNODE_CREATEBY_FULL_RESULT_NAME(_cons \
        ) = isl_calloc(ist_astnode_##_raw_type);                               \
        *(ist_astnode*)__IST_ASTNODE_CREATEBY_FULL_RESULT_NAME(_cons           \
        ) = ist_astnode_consby_full(isl_astnt_##_raw_type, _location);         \
        __IST_ASTNODE_CREATEBY_FULL_CONS(_cons);                               \
        (void*)__IST_ASTNODE_CREATEBY_FULL_RESULT_NAME(_cons);                 \
    })

#define ist_astnode_defineby_full(varid, _raw_type, _location, _cons...) \
    ist_astnode_##_raw_type* varid = ist_astnode_createby_full(_raw_type, _location, _cons)


/**w
 * Make the node as pointer of the specific type of astnode.
 */
#define ist_astnode_as(_node, _raw_type) ((ist_astnode_##_raw_type*)_node)

/* just as void ptr to make it more easy to pass */
#define isl_asvp(_adr) ((void*)_adr)

/**
 * Make the address as pointer of the specific type of astnode.
 * for all the astnode, you can use the fn to convert the address to the specific type.
 */
#define manifest(_name, _struct) ist_astnode_##_name* isl_as_##_name(void* adr);
#include "isl_astnodes.h"
#undef manifest

/* just delete the astnode */
void ist_astnode_delete(void* this);

/**
 * Delete the AST by the astnode.
 * And if the astnode has sub nodes, it will delete them recursively.
 */
void ist_ast_delete(void* this);

/**
 * Dump the whole AST to the json format string.
 * Store the result to the buffer, and return the context of buffer.
 */
ist_string ist_ast_dump_old(void* this, ist_string* buffer, ist_usize* idxptr);

ist_string ist_ast_dump(ist_vptr this, ist_dumpctx dctx);

/**
 * Declares for special nodes.
 */


void ist_astnode_node_list_add(ist_astnode_node_list* this, void* node);


#endif // ISC_ASTNODE_H