#include "isl_xssert.h"

#define ISL_TEST_ITEM xssert
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    int num = 1;
    isl_wssert(num != 1);
    isl_wssert(num != 1, "num should not be 1.");
    isl_wssert(num != 1, "num should not be 1.", "this is just a test.");
    isl_wssert(num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));
    num = 2;
    (void)num;
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
