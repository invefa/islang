/*
    This c source file is the main source file for islang and it is the entrance of islang.
    This project is compiled with -std=gnu11, because it will use some extension functions
    which are suplied by GNU standard extensions.

    There are a list of extensions are used:
        1. typeof keyword.
    And some features are used:
        1. special macro expansion.

    Anyway, you had better compile this project with gcc -std=gnu11.
    Written by invefa.
*/

#include <stdio.h>
#include <stdlib.h>

#include "isl_xssert.h"
#include "isl_overload.h"
#include "isl_ansictrl.h"
#include "isl_memgr.h"

void isl_test_overload(void);
void isl_test_xssert(void);
void isl_test_list(void);

int main(void) {

    isl_test_overload();
    isl_test_xssert();
    isl_test_list();

    return 0;
}

#define pal() printf("allocated length = %u\n", isl_allocated_length)

typedef struct ist_i32_list {
    ist_i32* data;
    ist_usize size;
} ist_i32_list;

ist_i32* ist_i32_list_resize(ist_i32* _list, ist_usize _size) {
    isl_assert(_list, "list should not be NULL.");
    isl_assert(_size, "size should not be 0.");
    ist_i32* new_list = isl_calloc_list(ist_i32, _size);
    memcpy(new_list, _list, isl_list_catch_length(_list));
    isl_list_catch_length(new_list) = sizeof(ist_i32) * _size;
    isl_free_list(_list);
    return new_list;
}



void isl_test_list(void) {

    ist_i32* list = isl_calloc_list(ist_i32, 10);
    printf("list = %p\n", list);
    printf("length of list = %u\n", isl_list_catch_length(list));
    printf("capacity of list = %u\n", isl_list_ptr_get_capacity(list));
    pal();
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5,
        list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;

    ist_i32* new_list;
    ist_list_resizec(list, 20);
    ist_list_resizec(list, 30);
    ist_list_resizem(list, 6, new_list);


    pal();
    // ist_i32_list* list2 = isl_calloc(ist_i32_list);
    // list2->data = list;
    // list2->size = 10;
    // printf("list2 = %p\n", list2);
    // pal();

    isl_freev_list(new_list);
    pal();
    // isl_freev(list2);
    // pal();
}


void isl_test_xssert(void) {
    /*
        __ISL_XSSERT(w, num != 1);
        __ISL_XSSERT(w, num != 1, "num should not be 1.");
        __ISL_XSSERT(w, num != 1, "num should not be 1.", "this is just a test.");
        __ISL_XSSERT(a, num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));
    */

    int num = 1;
    isl_wssert(num != 1);
    isl_wssert(num != 1, "num should not be 1.");
    isl_wssert(num != 1, "num should not be 1.", "this is just a test.");
    isl_wssert(num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));

}


void isl_test_overload(void) {

#define test_overload(_vargs...) _isl_overload(test_overload,##_vargs)

#define test_overload_0()       printf("test overload: 0 args\n")
#define test_overload_1(_1)     printf("test overload: 1 args\nfirst arg = %d\n", _1)
#define test_overload_2(_1, _2) printf("test overload: 2 args\nfirst arg = %d, and second arg = %d\n", _1, _2)

    test_overload();
    test_overload(123456);
    test_overload(123, 456);

}

