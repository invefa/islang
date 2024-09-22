#ifndef ISL_VALUE_H
#define ISL_VALUE_H

#include "isl_types.h"
#include "isl_string.h"

typedef union ist_value {
    ist_i64 int_value;
    ist_f64 real_value;
    ist_bool bool_value;
    ist_string* str_value;
} ist_value;



#endif