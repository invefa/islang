#include "isg_list.h"
#include "isl_module.h"
#include "isl_value.h"
#include <inttypes.h>
#include <time.h>

#define ISL_TEST_ITEM generic
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    // isp_report_option_enable(ISP_ROPTM_NO_CORE_INFO);

    ist_value_list* value_list = ist_value_list_createc(10);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for (ist_usize i = 0; i < 123456; ++i)
        ist_value_list_addm(value_list, ist_value_{.as.i64 = i * 13});

    clock_gettime(CLOCK_REALTIME, &end);

    printf(
        "time cost: %lfs\n",
        (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.
    );

    ist_value_list_addc(value_list, ist_value_{.as.i64 = 1234567890});

    // isg_list_foreach (iterp, *value_list, idx)
    //     printf("value[%zu] = %" PRId64 "\n", idx, iterp->int_value);


    ist_moduleList* module_list = ist_moduleList_createc(10);
    ist_moduleList_addc(
        module_list, ist_module_consby_filepath(ist_string_consby_raw("./scripts/test.is"))
    );
    ist_moduleList_addc(
        module_list, ist_module_consby_filepath(ist_string_consby_raw("./scripts/test.is"))
    );
    ist_moduleList_addc(
        module_list, ist_module_consby_filepath(ist_string_consby_raw("./scripts/test.is"))
    );

    ist_strbuf buffer = ist_strbuf_cons(142);

    isg_list_foreach (iterp, *module_list, idx) {
        printf("module[%zu] = %s\n", idx, ist_module_dump(iterp, ist_dumpctx_{buffer, DKIND_JSON}));
    }

    ist_strbuf_clean(&buffer);
    ist_moduleList_delete(module_list);
    ist_value_list_delete(value_list);

    // isp_report_option_disable(ISP_ROPTM_NO_CORE_INFO);

    ist_value_stack* stack = ist_value_stack_calloc(16);
    for (ist_usize i = 0; i < 18; ++i) {
        ist_value_stack_pushc(stack, ist_value_{.as.u64 = i});
    }

    isg_list_foreach (vp, *stack) {
        printf("%" PRIu64 " ", vp->as.u64);
    }
    printf("\n");

    for (ist_usize i = 0; i < 18; ++i) {
        isl_assert(ist_value_stack_pop(stack).as.u64 == 17 - i);
    }
    // ist_value_stack_pop(stack);

    ist_value_stack_delete(stack);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
