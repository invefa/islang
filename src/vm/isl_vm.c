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
                rtmp = ist_value_stack_pop(vm.stack);
                ltmp = ist_value_stack_pop(vm.stack);
                ist_value_stack_pushm(
                    vm.stack, ist_value_consby_i64(ltmp.int_value + rtmp.int_value)
                );
                break;
            case ist_inst_sub:
                rtmp = ist_value_stack_pop(vm.stack);
                ltmp = ist_value_stack_pop(vm.stack);
                ist_value_stack_pushm(
                    vm.stack, ist_value_consby_i64(ltmp.int_value - rtmp.int_value)
                );
                break;
            case ist_inst_mul:
                rtmp = ist_value_stack_pop(vm.stack);
                ltmp = ist_value_stack_pop(vm.stack);
                ist_value_stack_pushm(
                    vm.stack, ist_value_consby_i64(ltmp.int_value * rtmp.int_value)
                );
                break;
            case ist_inst_div:
                rtmp = ist_value_stack_pop(vm.stack);
                ltmp = ist_value_stack_pop(vm.stack);
                ist_value_stack_pushm(
                    vm.stack, ist_value_consby_i64(ltmp.int_value / rtmp.int_value)
                );
                break;

            default:
                isl_report(rid_unreachable_brench, isp_catch_coreloc);
        }
    }
}