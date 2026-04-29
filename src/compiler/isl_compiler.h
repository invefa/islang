#ifndef ISC_COMPILER_H
#define ISC_COMPILER_H

#include "isl_astnode.h"
#include "isl_context.h"
#include "isl_instream.h"
#include "isl_parser.h"

typedef struct ist_compiler {
    ist_context*  ctx;
    ist_parser*   parser;
    ist_module*   module;
    ist_astnode*  node;
    ist_compent*  cpent;
    ist_instream* instream;
} ist_compiler;
#define ist_compiler_ (ist_compiler)

void ist_compiler_consby_full();
void ist_compiler_initby_full(ist_compiler* this);

void ist_compiler_elaborate(ist_compiler* this);
void ist_compiler_evalualte(ist_compiler* this);
void ist_compiler_codegen(ist_compiler* this);
void ist_compiler_compile(ist_compiler* this);


#endif // ISC_COMPILER_H