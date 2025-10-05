#include "isl_list.h"
#define ISL_TEST_ITEM isl_list
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    ist_i32* isl_emit_i32_list() {
        ist_i32* list = isl_list_malloc(ist_i32, 10);
        list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
        list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
        return list;
    }

    ist_i32* list = isl_list_calloc(ist_i32, 10);
    // ist_i32* new_list = NULL;
    printf("list = 0x%zX\n", (ist_usize)list);
    printf("length of list = %zu\n", isl_list_catch_length(list));
    printf("capacity of list = %zu\n", isl_list_ptr_get_capacity(list));

    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    ist_usize size = 10;

    isl_list_ensurec(list, size, 5);

    isl_list_resizc(list, 20);
    isl_list_resizc(list, 30);
    isl_list_resizc(list, 6);

    isl_list_freev(list);
    isl_wssert(0);
    isl_list_ensurec(isl_emit_i32_list(), 9, 1, list);
    size = 10;

    for (ist_usize i = 0; i < isl_list_ptr_get_capacity(list); ++i)
        printf("list[%zu] = %d\n", i, list[i]);

    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);

    for (ist_usize i = 0; i < isl_list_ptr_get_capacity(list); ++i)
        printf("list[%zu] = %d\n", i, list[i]);

    isl_list_freev(list);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
