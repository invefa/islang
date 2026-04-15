#ifndef ISC_COMPENT_H
#define ISC_COMPENT_H
#include "isl_astnode.h"
#include "isl_macros.h"
#include "isl_string.h"
#include "isl_types.h"


typedef ist_u8 ist_compent_kindenum;
enum ist_compent_kindenum {
#define manifest(_name, _struct) isl_cpentkind_##_name,
#include "isl_compents.h"
#undef manifest
};

typedef struct ist_compent {
    ist_compent_kindenum kind;
    ist_location         location;
} ist_compent;

#define manifest(_name, _struct) typedef __ISL_MACRO_UNPACKAGER _struct ist_compent_##_name;
#include "isl_compents.h"
#undef manifest

#define ISG_STRUCT_NAME ist_compentptr_list
#define ISG_VALUE_TYPE  ist_compent*
#include "isg_list_head.h"

void ist_compent_clean(ist_compent* this);

/* rkind means raw kind name, for example ist_compent_expr (kind) => expr (rkind) */
#define ist_compent_createm(_rkind, _cons) (void*)isl_malloc_cons(ist_compent_##_rkind, _cons)
#define ist_compent_createc(_rkind, _cons) (void*)isl_calloc_cons(ist_compent_##_rkind, _cons)

#define ist_compent_consby_full(_kind, _location) \
    ((ist_compent){.kind = (_kind), .location = (_location)})


#define __IST_COMPENT_CREATEBY_FULL_CONS_0()
#define __IST_COMPENT_CREATEBY_FULL_CONS_1(_cons)        _cons
#define __IST_COMPENT_CREATEBY_FULL_CONS_2(_name, _cons) _cons
#define __IST_COMPENT_CREATEBY_FULL_CONS(_cons...) \
    _isl_overload(__IST_COMPENT_CREATEBY_FULL_CONS, ##_cons)

#define __IST_COMPENT_CREATEBY_FULL_RESULT_NAME_0()             __RESULT__
#define __IST_COMPENT_CREATEBY_FULL_RESULT_NAME_1(_cons)        __RESULT__
#define __IST_COMPENT_CREATEBY_FULL_RESULT_NAME_2(_name, _cons) _name
#define __IST_COMPENT_CREATEBY_FULL_RESULT_NAME(_cons...) \
    _isl_overload(__IST_COMPENT_CREATEBY_FULL_RESULT_NAME, ##_cons)

/**
 * Create an sub of compent by the raw type and the location and optional cons.
 * Raw type means the suffix of the type enum, for example, if you want to create `ist_compent_scope`,
 * you should provide the raw type as `scope`.
 *
 * Optionally, you can provide the constructor argument to initialize the compent,
 * just pass the argument in the form of a tuple to varg:cons.
 */
#define ist_compent_createby_full(_rkind, _location, _cons...)              \
    ({                                                                      \
        ist_compent_##_rkind* __IST_COMPENT_CREATEBY_FULL_RESULT_NAME(_cons \
        ) = isl_calloc(ist_compent_##_rkind);                               \
        *(ist_compent*)__IST_COMPENT_CREATEBY_FULL_RESULT_NAME(_cons        \
        ) = ist_compent_consby_full(isl_cpentkind_##_rkind, _location);     \
        __IST_COMPENT_CREATEBY_FULL_CONS(_cons);                            \
        (void*)__IST_COMPENT_CREATEBY_FULL_RESULT_NAME(_cons);              \
    })

#endif // ISC_COMPENT_H