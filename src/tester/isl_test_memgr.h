#include "isl_memgr.h"

void isl_test_memgr(void) {
    printf("test for memgr\n");
    ist_i32* list = isl_list_calloc(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    ist_i32_list* list2 = isl_calloc(ist_i32_list);
    list2->data         = isl_list_calloc(ist_i32, 10);
    list2->data[0] = 1, list2->data[1] = 2, list2->data[2] = 3, list2->data[3] = 4,
    list2->data[4] = 5;
    list2->data[5] = 6, list2->data[6] = 7, list2->data[7] = 8, list2->data[8] = 9,
    list2->data[9] = 10;
    list2->size    = 10;
    isl_list_freev(list);
    isl_list_freev(list2->data);
    isl_free(list2);
    list  = isl_emit_i32_list();
    list2 = isl_calloc(ist_i32_list);
    isl_list_resizc(isl_emit_i32_list(), 20, list2->data);
    isl_list_resizc(list, 20);
    isl_list_free(list);
    isl_list_free(list2->data);
    isl_free(list2);
    isl_list_resizc(isl_emit_i32_list(), 100, list);
    isl_list_free(list);
}
