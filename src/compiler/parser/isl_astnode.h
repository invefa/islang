#ifndef ISC_ASTNODE_H
#define ISC_ASTNODE_H

#include "isl_dump.h"
#include "isl_genmacros.h"
#include "isl_token.h"


typedef enum isn_astnodeKind isn_astnodeKind;
typedef union isu_astnodeAs  isu_astnodeAs;
typedef struct ist_astnode   ist_astnode;
typedef struct ist_astnode*  ist_parsent;

// /* define the list of the astnode */
// #define ISG_STRUCT_NAME ist_astnodeList
// #define ISG_VALUE_TYPE  ist_astnode
// #include "isg_list_head.h"

/* define the list of the parsent */
#define ISG_STRUCT_NAME ist_parsentList
#define ISG_VALUE_TYPE  ist_parsent
#include "isg_list_head.h"


/* the automatic definition of all the payload kind of astnodes */
#define manifest(_name, _struct) typedef _ISL_MACRO_UNPACK _struct isu_astnodeAs_##_name;
#include "isl_astnodes.h"
#undef manifest

typedef enum isn_astnodeKind {
#define manifest(_name, _struct) isn_astnodeKind_##_name,
#include "isl_astnodes.h"
#undef manifest
} isn_astnodeKind;
#define isn_astnodeKind_ (isn_astnodeKind)
#define isn_parsentKind_ (isn_astnodeKind)

extern ist_string ist_astnodeKindNames[];


typedef union isu_astnodeAs {
#define manifest(_name, _struct) isu_astnodeAs_##_name _name;
#include "isl_astnodes.h"
#undef manifest
} isu_astnodeAs;
#define isu_astnodeAs_ (isu_astnodeAs)

/**
 * `kind` means the ASTNode kind.
 * `location` is the source begin location of this ASTNode.
 * `as` is the variable struct for ASTNode, it is the payload
 * that contain the various infomation of this ASTNode.
 */
typedef struct ist_astnode {
    isn_astnodeKind kind;
    ist_location    location;
    isu_astnodeAs   as;
} ist_astnode;
#define ist_astnode_ (ist_astnode)

ist_astnode ist_astnode_consby_location(ist_location loc);
ist_astnode ist_astnode_consby_full(isn_astnodeKind kind, ist_location loc, isu_astnodeAs as);

_isl_declare_initby_createby_with_consby((ist_location loc), ist_astnode, location);
_isl_declare_initby_createby_with_consby(
    (isn_astnodeKind kind, ist_location loc, isu_astnodeAs as),
    ist_astnode,
    full
);

void ist_astnode_clean(ist_astnode* this);
void ist_astnode_delete(ist_astnode* this);


/**
 * `parsent` is a alias for `AST(AbstractSyntaxTree)`
 * it also is the abbreviation of `parse-time-entity`.
 * means the yield of parser, a full AST.
 */
typedef struct ist_astnode* ist_parsent;


void       ist_parsent_delete(ist_parsent this);
ist_string ist_parsent_dump(ist_parsent this, ist_dumpctx dctx);



#endif // ISC_ASTNODE_H