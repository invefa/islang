#include "isg_list.h"
#include "isl_list.h"

#define ISL_TEST_ITEM isg_list
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    ist_usize_list* list = ist_usize_list_createc(128);

    isg_list_foreach (itemp, *list, index) *itemp = index;
    isg_list_foreach (itemp, *list, index)
        isl_assert(*itemp == index, "something worong within impl of isg_usize_list!");

    ist_usize_list_addc(list, 32);
    isl_assert(list->data[list->size - 1] == 32, "something worong within ist_usize_list_addc.");

    ist_usize_list_delete(list);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"