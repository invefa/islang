#include "isl_vm.h"

#include "isl_genmacros.h"

ist_vm ist_vm_consby_full(ist_instream _instream, ist_value_stack* _stack) {
    return (ist_vm){
        .instream = _instream,
        .ip       = _instream.data,
        .stack    = _stack,
        .sp       = _stack->data,
        .fp       = _stack->data,
        .px       = {},
    };
}
_isl_define_initby_createby_with_consby(
    (ist_instream _instream, ist_value_stack* _stack),
    (_instream, _stack),
    ist_vm,
    full
);

ist_vm ist_vm_consby_instream(ist_instream _instream) {
    return ist_vm_consby_full(_instream, ist_value_stack_create(64, false));
}
_isl_define_initby_createby_with_consby((ist_instream _instream), (_instream), ist_vm, instream);

void ist_vm_clean(ist_vm* this) {
    if (!this) isp_unreachable();
    if (this->stack) ist_value_stack_delete(this->stack);
}
void ist_vm_delete(ist_vm* this) {
    if (!this) isp_unreachable();
    ist_vm_clean(this);
    isl_free(this);
}


void ist_vm_run(ist_vm* _vm) {
    ist_vm vm = *_vm;
    /** just a code highlight provider */
    enum __isl_typesign { i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, bool, str, strbuf };

#define signtype(_typesign)    ist_##_typesign
#define ip_as(ipv, _typesign)  ((ist_##_typesign*)(ipv))
#define ip_asv(ipv, _typesign) (*ip_as(ipv, _typesign))
#define ip_read_operand(ipv, _typesign)                               \
    ({                                                                \
        signtype(_typesign) __operand  = ip_asv(ipv, _typesign);      \
        (ipv)                         += sizeof(signtype(_typesign)); \
        __operand;                                                    \
    })
#define ip_read_opcode(ipv)          (*(ipv)++)
#define sp_pop(spv)                  (*(spv)--)
#define sp_psh(spv, _value)          (*++(spv) = (_value))
#define sp_pshas(spv, _as, _asvalue) ((++(spv))->as._as = (_asvalue))


    while (*vm.ip != ist_inst_end) {
        switch (ip_read_opcode(vm.ip)) {
            case ist_inst_pass:
                break;
            case ist_inst_pshc:
                break;
            case ist_inst_pshi:
                sp_pshas(vm.sp, i64, ip_read_operand(vm.ip, i64));
                break;
            case ist_inst_pshu:
                sp_pshas(vm.sp, u64, ip_read_operand(vm.ip, u64));
                break;
            case ist_inst_pshf:
                sp_pshas(vm.sp, f32, ip_read_operand(vm.ip, f32));
                break;
            case ist_inst_pshd:
                sp_pshas(vm.sp, f64, ip_read_operand(vm.ip, f64));
                break;
            case ist_inst_pop:
                vm.px = sp_pop(vm.sp);
                break;
            case ist_inst_ldv:
                sp_psh(vm.sp, vm.fp[ip_read_operand(vm.ip, i16)]);
                break;
            case ist_inst_stv:
                vm.fp[ip_read_operand(vm.ip, i16)] = sp_pop(vm.sp);
                break;

            case ist_inst_addi:
                vm.sp[-1].as.i64 += vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_subi:
                vm.sp[-1].as.i64 -= vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_muli:
                vm.sp[-1].as.i64 *= vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_divi:
                vm.sp[-1].as.i64 /= vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_modi:
                vm.sp[-1].as.i64 %= vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_divu:
                vm.sp[-1].as.u64 /= vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_modu:
                vm.sp[-1].as.u64 %= vm.sp[0].as.u64;
                --vm.sp;
                break;

            case ist_inst_addf:
                vm.sp[-1].as.f32 += vm.sp[0].as.f32;
                --vm.sp;
                break;
            case ist_inst_subf:
                vm.sp[-1].as.f32 -= vm.sp[0].as.f32;
                --vm.sp;
                break;
            case ist_inst_mulf:
                vm.sp[-1].as.f32 *= vm.sp[0].as.f32;
                --vm.sp;
                break;
            case ist_inst_divf:
                vm.sp[-1].as.f32 /= vm.sp[0].as.f32;
                --vm.sp;
                break;

            case ist_inst_addd:
                vm.sp[-1].as.f64 += vm.sp[0].as.f64;
                --vm.sp;
                break;
            case ist_inst_subd:
                vm.sp[-1].as.f64 -= vm.sp[0].as.f64;
                --vm.sp;
                break;
            case ist_inst_muld:
                vm.sp[-1].as.f64 *= vm.sp[0].as.f64;
                --vm.sp;
                break;
            case ist_inst_divd:
                vm.sp[-1].as.f64 /= vm.sp[0].as.f64;
                --vm.sp;
                break;

            case ist_inst_i64t32:
                vm.sp[0].as.i64 = vm.sp[0].as.i32;
                break;
            case ist_inst_i64t16:
                vm.sp[0].as.i64 = vm.sp[0].as.i16;
                break;
            case ist_inst_i64t8:
                vm.sp[0].as.i64 = vm.sp[0].as.i8;
                break;
            case ist_inst_u64t32:
                vm.sp[0].as.u64 = vm.sp[0].as.u32;
                break;
            case ist_inst_u64t16:
                vm.sp[0].as.u64 = vm.sp[0].as.u16;
                break;
            case ist_inst_u64t8:
                vm.sp[0].as.u64 = vm.sp[0].as.u8;
                break;

            case ist_inst_band:
                vm.sp[-1].as.u64 &= vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_bor:
                vm.sp[-1].as.u64 |= vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_bxor:
                vm.sp[-1].as.u64 ^= vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_bnot:
                vm.sp[0].as.u64 = ~vm.sp[0].as.u64;
                break;

            case ist_inst_land:
                vm.sp[-1].as.bool = vm.sp[-1].as.bool && vm.sp[0].as.bool;
                --vm.sp;
                break;
            case ist_inst_lor:
                vm.sp[-1].as.bool = vm.sp[-1].as.bool && vm.sp[0].as.bool;
                --vm.sp;
                break;
            case ist_inst_lnot:
                vm.sp[0].as.bool = !vm.sp[0].as.bool;
                break;

            case ist_inst_eq:
                vm.sp[-1].as.bool = vm.sp[-1].as.u64 == vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_neq:
                vm.sp[-1].as.bool = vm.sp[-1].as.u64 != vm.sp[0].as.u64;
                --vm.sp;
                break;

            case ist_inst_lti:
                vm.sp[-1].as.bool = vm.sp[-1].as.i64 < vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_gti:
                vm.sp[-1].as.bool = vm.sp[-1].as.i64 > vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_leti:
                vm.sp[-1].as.bool = vm.sp[-1].as.i64 <= vm.sp[0].as.i64;
                --vm.sp;
                break;
            case ist_inst_geti:
                vm.sp[-1].as.bool = vm.sp[-1].as.i64 >= vm.sp[0].as.i64;
                --vm.sp;
                break;

            case ist_inst_ltu:
                vm.sp[-1].as.bool = vm.sp[-1].as.u64 < vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_gtu:
                vm.sp[-1].as.bool = vm.sp[-1].as.u64 > vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_letu:
                vm.sp[-1].as.bool = vm.sp[-1].as.u64 <= vm.sp[0].as.u64;
                --vm.sp;
                break;
            case ist_inst_getu:
                vm.sp[-1].as.bool = vm.sp[-1].as.u64 >= vm.sp[0].as.u64;
                --vm.sp;
                break;

            case ist_inst_ltf:
                vm.sp[-1].as.bool = vm.sp[-1].as.f32 < vm.sp[0].as.f32;
                --vm.sp;
                break;
            case ist_inst_gtf:
                vm.sp[-1].as.bool = vm.sp[-1].as.f32 > vm.sp[0].as.f32;
                --vm.sp;
                break;
            case ist_inst_letf:
                vm.sp[-1].as.bool = vm.sp[-1].as.f32 <= vm.sp[0].as.f32;
                --vm.sp;
                break;
            case ist_inst_getf:
                vm.sp[-1].as.bool = vm.sp[-1].as.f32 >= vm.sp[0].as.f32;
                --vm.sp;
                break;

            case ist_inst_ltd:
                vm.sp[-1].as.bool = vm.sp[-1].as.f64 < vm.sp[0].as.f64;
                --vm.sp;
                break;
            case ist_inst_gtd:
                vm.sp[-1].as.bool = vm.sp[-1].as.f64 > vm.sp[0].as.f64;
                --vm.sp;
                break;
            case ist_inst_letd:
                vm.sp[-1].as.bool = vm.sp[-1].as.f64 <= vm.sp[0].as.f64;
                --vm.sp;
                break;
            case ist_inst_getd:
                vm.sp[-1].as.bool = vm.sp[-1].as.f64 >= vm.sp[0].as.f64;
                --vm.sp;
                break;

            case ist_inst_jmp:
                vm.ip += ip_asv(vm.ip, i16) - 1;
                break;
            case ist_inst_cjmp:
                if (sp_pop(vm.sp).as.bool) vm.ip += ip_asv(vm.ip, i16) - 1;
                break;
            case ist_inst_ncjmp:
                if (!sp_pop(vm.sp).as.bool) vm.ip += ip_asv(vm.ip, i16) - 1;
                break;
            case ist_inst_goto:
                vm.ip = vm.instream.data + ip_asv(vm.ip, i32);
                break;
            case ist_inst_cgoto:
                if (sp_pop(vm.sp).as.bool) vm.ip = vm.instream.data + ip_asv(vm.ip, i32);
                break;
            case ist_inst_ncgoto:
                if (!sp_pop(vm.sp).as.bool) vm.ip = vm.instream.data + ip_asv(vm.ip, i32);
                break;

            default:
                isp_unreachable();
        }
    }
    *_vm = vm;
}