#ifndef ISC_VM_H
#define ISC_VM_H

#include "isl_instream.h"
#include "isl_value.h"


typedef enum ist_vm_regist {
    rax,
    rbx,
    rcx,
    rdx,
    rex,
    rfx,
    rgx,
    rhx,
    rix,
    rsp,
    rbp,
    rip,
    rss,
    rcs,
    rvs,
    rsi,
    rdi
} ist_vm_regist;

typedef struct ist_vm {
    ist_instream     instream;
    ist_value        regist[16];
    ist_value_stack* stack;
} ist_vm;

ist_vm  ist_vm_consby_full(ist_instream instream);
ist_vm* ist_vm_initby_full(ist_vm* this, ist_instream instream);
ist_vm* ist_vm_createby_full(ist_instream instream);

#endif // ISC_VM_H