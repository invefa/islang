#ifndef ISC_SEMNODE_H
#define ISC_SEMNODE_H

#include "isl_dump.h"
#include "isl_genmacros.h"
#include "isl_token.h"


/**
 * Why not directly reuse the astnode instead of designing a separate set of semnode?
 *
 * This is because our astnode is a direct mapping of the grammatical structure,
 * which results in it having strong grammatical noise. Without an intermediate layer,
 * during certain compilation processes, we would need to make special handling for a
 * large number of astnode types.
 *
 * The significance of semnode lies in the fact that it is a simplified semantic primitive,
 * which can reduce various grammatical forms to a small number of primitive combinations.
 * During this process, it is also possible to do some work on the type system,
 * which is where the value of semnode lies.
 *
 * Additionally, it will exist as a component of compent.
 */


typedef enum ist_semnodeKind ist_semnodeKind;
typedef union ist_semnodeAs  ist_semnodeAs;
typedef struct ist_semnode   ist_semnode;
typedef struct ist_semnode*  ist_semtree;

/* semantical tree array, must be `isl_list` */
typedef struct ist_semnode* ist_semtrees;

/* define the list of the parsent */
#define ISG_STRUCT_NAME ist_semtreeList
#define ISG_VALUE_TYPE  ist_semtree
#include "isg_list_head.h"


/* the automatic definition of all the payload kind of semnodes */
#define manifest(_name, _struct) typedef _ISL_MACRO_UNPACK _struct ist_semnodeAs_##_name;
#include "isl_semnodes.h"
#undef manifest

typedef enum ist_semnodeKind {
#define manifest(_name, _struct) ist_semnodeKind_##_name,
#include "isl_semnodes.h"
#undef manifest
} ist_semnodeKind;

extern ist_string ist_semnodeKindNames[];


typedef union ist_semnodeAs {
#define manifest(_name, _struct) ist_semnodeAs_##_name _name;
#include "isl_semnodes.h"
#undef manifest
} ist_semnodeAs;
#define ist_semnodeAs_ (ist_semnodeAs)

/**
 * `kind` means the semnode kind.
 * `location` is the source begin location of this semnode.
 * `as` is the variable struct for semnode, it is the payload
 * that contain the various infomation of this semnode.
 */
typedef struct ist_semnode {
    ist_semnodeKind kind     : 8;
    ist_bool        evaluated: 1;
    ist_bool        errored  : 1;
    ist_location    location;
    ist_semnodeAs   as;
} ist_semnode;
#define ist_semnode_ (ist_semnode)

ist_semnode ist_semnode_consby_location(ist_location loc);
ist_semnode ist_semnode_consby_full(ist_semnodeKind kind, ist_location loc, ist_semnodeAs as);

_isl_declare_initby_createby_with_consby((ist_location loc), ist_semnode, location);
_isl_declare_initby_createby_with_consby(
    (ist_semnodeKind kind, ist_location loc, ist_semnodeAs as),
    ist_semnode,
    full
);

void ist_semnode_clean(ist_semnode* this);
void ist_semnode_delete(ist_semnode* this);

ist_string ist_semnode_dump(ist_semnode* this, ist_dumpctx dctx);


typedef struct ist_semnode* ist_semtree;

void       ist_semtree_delete(ist_semtree this);
ist_string ist_semtree_dump(ist_semtree this, ist_dumpctx dctx);



#endif // ISC_SEMNODE_H