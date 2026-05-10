#ifndef ISC_COMPENT_H
#define ISC_COMPENT_H

#include "isl_astnode.h"
#include "isl_genmacros.h"
#include "isl_string.h"
#include "isl_types.h"

#include "isl_compent_basetype.h"


// TODO: make the enum name form more concord!
typedef enum ist_compentKind {
#define manifest(_name, _struct) ist_compentKind_##_name,
#include "isl_compents.h"
#undef manifest
} ist_compentKind;


#define manifest(_name, _struct) typedef _ISL_MACRO_UNPACK _struct ist_compentAs_##_name;
#include "isl_compents.h"
#undef manifest

typedef union ist_compentAs {
#define manifest(_name, _struct) ist_compentAs_##_name _name;
#include "isl_compents.h"
#undef manifest
} ist_compentAs;

typedef struct ist_compent {
    ist_compentKind kind;
    ist_location    location;
    ist_compentAs   as;
} ist_compent;
#define ist_compent_ (ist_compent)

ist_compent ist_compent_consby_full(ist_compentKind kind, ist_location loc, ist_compentAs as);
_isl_declare_initby_createby_with_consby(
    (ist_compentKind kind, ist_location loc, ist_compentAs as),
    ist_compent,
    full
);

void ist_compent_clean(ist_compent* this);
void ist_compent_delete(ist_compent* this);


/**
 * compent reference is depend on `ist_compctx`,
 * it is the index of compent in `ist_compctx::compents` */
typedef ist_usize ist_compentRef;

#define ISG_STRUCT_NAME ist_compentRefList
#define ISG_VALUE_TYPE  ist_compentRef
#include "isg_list_head.h"

#define ISG_STRUCT_NAME ist_compentPtrList
#define ISG_VALUE_TYPE  ist_compent*
#include "isg_list_head.h"

#define ISG_STRUCT_NAME ist_compentList
#define ISG_VALUE_TYPE  ist_compent
#include "isg_list_head.h"

#endif // ISC_COMPENT_H