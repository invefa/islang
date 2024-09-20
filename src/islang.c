/*
    This c source file is the main source file for islang and it is the entrance of islang.
    This project is compiled with -std=gnu11, because it will use some extension functions
    which are suplied by GNU standard extensions.

    There are a list of extensions are used:
        1. typeof keyword.
        2. u8 prefix for string literal.
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
#include "isl_list.h"
#include "isl_string.h"

void isl_test_overload(void);
void isl_test_xssert(void);
void isl_test_list(void);
void isl_test_memgr(void);
void isl_test_string(void);

int main(void) {

    isl_test_overload();
    isl_test_xssert();
    isl_test_list();
    isl_test_memgr();
    isl_test_string();

    return 0;
}

#define pal() printf("allocated length = %u\n", isl_allocated_length)

void isl_test_string(void) {
    ist_string* str1 = ist_string_create(u8"你好", 6);
    ist_string* str2 = ist_string_create(u8"world", 5);

    ist_string str3;
    ist_string_init(&str3, u8"hello", 5);

    printf("str1 = %s\n", *str1);
    printf("str2 = %s\n", *str2);
    printf("str3 = %s\n", str3);
    pal();

    ist_string_delete(str1);
    ist_string_delete(str2);
    ist_string_clean(&str3);
    pal();
}



typedef struct ist_i32_list {
    ist_i32* data;
    ist_usize size;
} ist_i32_list;



ist_i32* isl_emit_i32_list() {
    ist_i32* list = isl_malloc_list(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    return list;
}

void isl_test_memgr(void) {
    printf("test for memgr\n");
    pal();
    ist_i32* list = isl_calloc_list(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    pal();
    ist_i32_list* list2 = isl_calloc(ist_i32_list);
    pal();
    list2->data = isl_calloc_list(ist_i32, 10);
    list2->data[0] = 1, list2->data[1] = 2, list2->data[2] = 3, list2->data[3] = 4, list2->data[4] = 5;
    list2->data[5] = 6, list2->data[6] = 7, list2->data[7] = 8, list2->data[8] = 9, list2->data[9] = 10;
    list2->size = 10;
    pal();
    isl_freev_list(list);
    pal();
    isl_freev_list(list2->data);
    pal();
    isl_free(list2);
    pal();
    list = isl_emit_i32_list();
    pal();
    list2 = isl_calloc(ist_i32_list);
    pal();
    ist_list_resizec(isl_emit_i32_list(), 20, list2->data);
    pal();
    ist_list_resizec(list, 20);
    pal();
    isl_free_list(list);
    pal();
    isl_free_list(list2->data);
    pal();
    isl_free(list2);
    pal();
    ist_list_resizec(isl_emit_i32_list(), 100, list);
    pal();
    isl_free_list(list);
    pal();
}


void isl_test_list(void) {

    ist_i32* list = isl_calloc_list(ist_i32, 10);
    ist_i32* new_list = NULL;
    printf("list = %p\n", list);
    printf("length of list = %u\n", isl_list_catch_length(list));
    printf("capacity of list = %u\n", isl_list_ptr_get_capacity(list));
    pal();
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5,
        list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;

    ist_list_resizec(list, 20);
    pal();
    ist_list_resizec(list, 30);
    pal();
    ist_list_resizem(list, 6);
    pal();
    isl_freev_list(list);
    pal();
    ist_list_resizem(isl_emit_i32_list(), 5, list);
    pal();
    isl_freev_list(list);
    pal();
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

