#include "isl_vm.h"

ist_vm ist_vm_consby_full(ist_instream instream) {
    return (ist_vm){.instream = instream, .regist = {}, .stack = ist_value_stack_create(64, true)};
}

ist_vm* ist_vm_initby_full(ist_vm* this, ist_instream instream) {
    *this = ist_vm_consby_full(instream);
    return this;
}
ist_vm* ist_vm_createby_full(ist_instream instream) {
    return ist_vm_initby_full(isl_allocate(sizeof(ist_vm), false), instream);
}

void ist_vm_run(ist_vm vm) {
    ist_instruction* ip = vm.instream.data;
    ist_value        ltmp, rtmp;
    while (*ip != ist_inst_end) {
        switch (*ip++) {
            case ist_inst_add:
                vm.stack->data[vm.stack->size - 2].int_value =
                    vm.stack->data[vm.stack->size - 1].int_value
                    + vm.stack->data[vm.stack->size - 2].int_value;
                --vm.stack->size;
                break;
            case ist_inst_sub:
                vm.stack->data[vm.stack->size - 2].int_value =
                    vm.stack->data[vm.stack->size - 1].int_value
                    - vm.stack->data[vm.stack->size - 2].int_value;
                --vm.stack->size;
                break;
            case ist_inst_mul:
                vm.stack->data[vm.stack->size - 2].int_value =
                    vm.stack->data[vm.stack->size - 1].int_value
                    * vm.stack->data[vm.stack->size - 2].int_value;
                --vm.stack->size;
                break;
            case ist_inst_div:
                vm.stack->data[vm.stack->size - 2].int_value =
                    vm.stack->data[vm.stack->size - 1].int_value
                    / vm.stack->data[vm.stack->size - 2].int_value;
                --vm.stack->size;
                break;

            case ist_inst_addr:
                vm.regist[*ip++].int_value += vm.regist[*ip++].int_value;
                break;
            case ist_inst_subr:
                vm.regist[*ip++].int_value -= vm.regist[*ip++].int_value;
                break;
            case ist_inst_mulr:
                vm.regist[*ip++].int_value *= vm.regist[*ip++].int_value;
                break;
            case ist_inst_divr:
                vm.regist[*ip++].int_value /= vm.regist[*ip++].int_value;
                break;
            default:
                isl_report(rid_unreachable_brench, isp_catch_coreloc);
        }
    }
}