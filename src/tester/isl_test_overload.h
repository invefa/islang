#include "isl_overload.h"

void isl_test_overload(void) {

#define test_overload(_vargs...) _isl_overload(test_overload, ##_vargs)

#define test_overload_0()   printf("test overload: 0 args\n")
#define test_overload_1(_1) printf("test overload: 1 args\nfirst arg = %d\n", _1)
#define test_overload_2(_1, _2) \
    printf("test overload: 2 args\nfirst arg = %d, and second arg = %d\n", _1, _2)

    test_overload();
    test_overload(123456);
    test_overload(123, 456);
}
