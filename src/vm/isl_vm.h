#ifndef ISC_VM_H
#define ISC_VM_H

#include "isl_value.h"

typedef struct ist_vm {
    ist_value_stack* stack;

} ist_vm;

#endif // ISC_VM_H