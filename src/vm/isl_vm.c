#include "isl_vm.h"

ist_vm ist_vm_consby_full(ist_instream _instream, ist_value_stack* _stack) {
    return (ist_vm){
        .instream = _instream,
        .ip       = _instream.data,
        .stack    = _stack,
        .sp       = _stack->data,
        .fp       = _stack->data,
    };
}
ist_vm* ist_vm_initby_full(ist_vm* this, ist_instream _instream, ist_value_stack* _stack) {
    *this = ist_vm_consby_full(_instream, _stack);
    return this;
}
ist_vm* ist_vm_createby_full(ist_instream _instream, ist_value_stack* _stack) {
    return ist_vm_initby_full(isl_allocate(sizeof(ist_vm), false), _instream, _stack);
}

ist_vm ist_vm_consby_instream(ist_instream _instream) {
    return ist_vm_consby_full(_instream, ist_value_stack_create(64, false));
}
ist_vm* ist_vm_initby_instream(ist_vm* this, ist_instream _instream) {
    *this = ist_vm_consby_instream(_instream);
    return this;
}
ist_vm* ist_vm_createby_instream(ist_instream _instream) {
    return ist_vm_initby_instream(isl_allocate(sizeof(ist_vm), false), _instream);
}

void ist_vm_run(ist_vm vm) {
    while (*vm.ip != ist_inst_end) {
        switch (*vm.ip++) {
            case ist_inst_addi:
                vm.sp[-1].as_i64 += vm.sp[0].as_i64;
                --vm.sp;
                break;
            case ist_inst_subi:
                vm.sp[-1].as_i64 -= vm.sp[0].as_i64;
                --vm.sp;
                break;
            case ist_inst_muli:
                vm.sp[-1].as_i64 *= vm.sp[0].as_i64;
                --vm.sp;
                break;
            case ist_inst_divi:
                vm.sp[-1].as_i64 /= vm.sp[0].as_i64;
                --vm.sp;
                break;
            case ist_inst_modi:
                vm.sp[-1].as_i64 %= vm.sp[0].as_i64;
                --vm.sp;
                break;
            case ist_inst_divu:
                vm.sp[-1].as_u64 /= vm.sp[0].as_u64;
                --vm.sp;
                break;
            case ist_inst_modu:
                vm.sp[-1].as_u64 %= vm.sp[0].as_u64;
                --vm.sp;
                break;
            case ist_inst_jmp:
                vm.ip += *(ist_i16*)vm.ip - 1;
                break;
            case ist_inst_cjmp:
                if (vm.sp--[0].as_bool) vm.ip += *(ist_i16*)vm.ip - 1;
                break;
            case ist_inst_ncjmp:
                if (!vm.sp--[0].as_bool) vm.ip += *(ist_i16*)vm.ip - 1;
                break;
            case ist_inst_goto:
                vm.ip = vm.instream.data + *(ist_i32*)vm.ip;
                break;
            case ist_inst_cgoto:
                if (vm.sp--[0].as_bool) vm.ip = vm.instream.data + *(ist_i32*)vm.ip;
                break;
            case ist_inst_ncgoto:
                if (!vm.sp--[0].as_bool) vm.ip = vm.instream.data + *(ist_i32*)vm.ip;
                break;
            default:
                isl_report(rid_unreachable_brench, isp_catch_coreloc);
        }
    }
}