#ifndef ISC_COMPILER_H
#define ISC_COMPILER_H

#include "isl_astnode.h"
#include "isl_context.h"

typedef struct ist_compiler {
    ist_context*    ctx;
    ist_module_list module_list;
    ist_module*     module;
    ist_astnode*    node;
} ist_compiler;

void ist_compiler_elaborate(ist_compiler* this);
void ist_compiler_compile(ist_compiler* this);


#endif // ISC_COMPILER_H