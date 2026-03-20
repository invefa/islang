#ifndef ISC_VM_H
#define ISC_VM_H

#include "isl_instream.h"
#include "isl_value.h"


typedef struct ist_vm {
    ist_instream     instream;
    ist_instruction* ip;
    ist_value*       sp;
    ist_value*       fp;
    ist_value        px;
    ist_value_stack* stack;
} ist_vm;

ist_vm  ist_vm_consby_full(ist_instream _instream, ist_value_stack* _stack);
ist_vm* ist_vm_initby_full(ist_vm* this, ist_instream _instream, ist_value_stack* _stack);
ist_vm* ist_vm_createby_full(ist_instream _instream, ist_value_stack* _stack);

ist_vm  ist_vm_consby_instream(ist_instream _instream);
ist_vm* ist_vm_initby_instream(ist_vm* this, ist_instream _instream);
ist_vm* ist_vm_createby_instream(ist_instream _instream);

#endif // ISC_VM_H