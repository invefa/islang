#ifndef ISC_ASTNODE_H
#define ISC_ASTNODE_H

#include "isl_dump.h"
#include "isl_token.h"

typedef enum ist_astnodeKind   ist_astnodeKind;
typedef union ist_astnodeAs    ist_astnodeAs;
typedef struct ist_astnode     ist_astnode;
typedef struct ist_astnodeList ist_astnodeList;
typedef struct ist_astnode*    ist_parsent;

// /* define the list of the astnode */
// #define ISG_STRUCT_NAME ist_astnodeList
// #define ISG_VALUE_TYPE  ist_astnode
// #include "isg_list_head.h"

/* define the list of the parsent */
#define ISG_STRUCT_NAME ist_parsentList
#define ISG_VALUE_TYPE  ist_parsent
#include "isg_list_head.h"


/* the automatic definition of all the payload kind of astnodes */
#define manifest(_name, _struct) typedef __ISL_MACRO_UNPACKAGER _struct ist_astnodeAs_##_name;
#include "isl_astnodes.h"
#undef manifest

typedef enum ist_astnodeKind {
#define manifest(_name, _struct) ist_astnodeKind_##_name,
#include "isl_astnodes.h"
#undef manifest
} ist_astnodeKind;
#define ist_parsentKind_ (ist_astnodeKind)

extern ist_string ist_astnodeKindNames[];


typedef union ist_astnodeAs {
#define manifest(_name, _struct) ist_astnodeAs_##_name _name;
#include "isl_astnodes.h"
#undef manifest
} ist_astnodeAs;
#define ist_astnodeAs_ (ist_astnodeAs)

/**
 * `kind` means the ASTNode kind.
 * `location` is the source begin location of this ASTNode.
 * `as` is the variable struct for ASTNode, it is the payload
 * that contain the various infomation of this ASTNode.
 */
typedef struct ist_astnode {
    ist_astnodeKind kind;
    ist_location    location;
    ist_astnodeAs   as;
} ist_astnode;
#define ist_astnode_ (ist_astnode)

ist_astnode ist_astnode_consby_location(ist_location loc);
ist_astnode ist_astnode_consby_full(ist_astnodeKind kind, ist_location loc, ist_astnodeAs as);

_isl_declare_initby_createby_with_consby((ist_location loc), ist_astnode, location);
_isl_declare_initby_createby_with_consby(
    (ist_astnodeKind kind, ist_location loc, ist_astnodeAs as),
    ist_astnode,
    full
);

void ist_astnode_clean(ist_astnode* this);
void ist_astnode_delete(ist_astnode* this);

ist_string ist_astnode_dump(ist_astnode* this, ist_dumpctx dctx);


/**
 * `parsent` is a alias for `AST(AbstractSyntaxTree)`
 * it also is the abbreviation of `parse-time-entity`.
 * means the yield of parser, a full AST.
 */
typedef struct ist_astnode* ist_parsent;


void       ist_parsent_delete(ist_parsent this);
ist_string ist_parsent_dump(ist_parsent this, ist_dumpctx dctx);



#endif // ISC_ASTNODE_H


// #ifndef ISC_ASTNODE_H
// #define ISC_ASTNODE_H

// #include "isl_dump.h"
// #include "isl_token.h"

// /**
//  * This enum only for hightlight the type of the astnode.
//  * You can ignore it, it is unnecessary, and if there occurs some name conflict, remove it.
//  */
// // enum {
// // #define manifest(_name, _struct) _name,
// // #include "isl_astnodes.h"
// // #undef manifest
// // };



// /* define the names of the astnode type */
// extern ist_string ist_astnode_type_names[];

// /* define the type of the astnode, astnode-type enum */
// typedef ist_u8 ist_astnode_typenum;
// enum ist_astnode_typenum {
// #define manifest(_name, _struct) isl_astnt_##_name,
// #include "isl_astnodes.h"
// #undef manifest
// };

// /* define the base struct of the astnode */
// typedef struct ist_astnode {
//     ist_astnode_typenum type;
//     ist_location        location;
// } ist_astnode;
// #define ist_astnodeptr_ (ist_astnode*)

// /* define the list of the astnodeptr */
// #define ISG_STRUCT_NAME ist_astnodeptr_list
// #define ISG_VALUE_TYPE  ist_astnode*
// #include "isg_list_head.h"

// /* define all the astnode struct */
// #define manifest(_name, _struct) typedef __ISL_MACRO_UNPACKAGER _struct ist_astnode_##_name;
// #include "isl_astnodes.h"
// #undef manifest



// /**w
//  * Make the node as pointer of the specific type of astnode.
//  */
// #define ist_astnode_as(_node, _raw_type) ((ist_astnode_##_raw_type*)_node)

// /* just as void ptr to make it more easy to pass */
// #define isl_asvp(_adr) ((void*)_adr)

// /**
//  * Make the address as pointer of the specific type of astnode.
//  * for all the astnode, you can use the fn to convert the address to the specific type.
//  */
// #define manifest(_name, _struct) ist_astnode_##_name* isl_as_##_name(void* adr);
// #include "isl_astnodes.h"
// #undef manifest

// /* just delete the astnode */
// void ist_astnode_delete(void* this);

// /**
//  * Delete the AST by the astnode.
//  * And if the astnode has sub nodes, it will delete them recursively.
//  */
// void ist_ast_delete(void* this);

// /**
//  * Dump the whole AST to the json format string.
//  * Store the result to the buffer, and return the context of buffer.
//  */
// // ist_string ist_ast_dump_old(void* this, ist_string* buffer, ist_usize* idxptr);

// ist_string ist_ast_dump(ist_vptr this, ist_dumpctx dctx);

// /**
//  * Declares for special nodes.
//  */


// void ist_astnode_node_list_add(ist_astnode_node_list* this, void* node);


// #endif // ISC_ASTNODE_H