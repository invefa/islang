#ifndef ISG_LIST_H
#define ISG_LIST_H

#include "isl_types.h"


typedef struct isg_list {
    ist_usize size;
    ist_vptr  data;
} isg_list;


#define ISG_VALUE_TYPE ist_usize
#include "isg_list_head.h"


#endif // ISG_LIST_H