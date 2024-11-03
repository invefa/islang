#define ISG_STRUCT_KIND stack
#include "isg_defines.h"

#include "isl_list.h"

typedef struct ISG_STRUCT_NAME {
    ist_usize       size;
    ISG_VALUE_TYPE* data;
} ISG_STRUCT_NAME;

ISG_STRUCT_NAME  ISG_FN_NAME(consm)(ist_usize _capacity);
ISG_STRUCT_NAME  ISG_FN_NAME(consc)(ist_usize _capacity);
ISG_STRUCT_NAME* ISG_FN_NAME(initm)(ISG_STRUCT_NAME* this, ist_usize _capacity);
ISG_STRUCT_NAME* ISG_FN_NAME(initc)(ISG_STRUCT_NAME* this, ist_usize _capacity);
ISG_STRUCT_NAME* ISG_FN_NAME(malloc)(ist_usize _capacity);
ISG_STRUCT_NAME* ISG_FN_NAME(calloc)(ist_usize _capacity);
ISG_STRUCT_NAME* ISG_FN_NAME(create)(ist_usize _capacity, ist_bool _doclean);
void             ISG_FN_NAME(resizm)(ISG_STRUCT_NAME* this, ist_usize _newcap);
void             ISG_FN_NAME(resizc)(ISG_STRUCT_NAME* this, ist_usize _newcap);
ist_usize        ISG_FN_NAME(pushm)(ISG_STRUCT_NAME* this, ISG_VALUE_TYPE _value);
ist_usize        ISG_FN_NAME(pushc)(ISG_STRUCT_NAME* this, ISG_VALUE_TYPE _value);
ISG_VALUE_TYPE   ISG_FN_NAME(pop)(ISG_STRUCT_NAME* this);
ISG_VALUE_TYPE*  ISG_FN_NAME(poptr)(ISG_STRUCT_NAME* this);
void             ISG_FN_NAME(clean)(ISG_STRUCT_NAME* this);
void             ISG_FN_NAME(delete)(ISG_STRUCT_NAME* this);
ISG_VALUE_TYPE*  ISG_FN_NAME(data)(ISG_STRUCT_NAME* this);
ist_usize        ISG_FN_NAME(size)(ISG_STRUCT_NAME* this);

#include "isg_undefs.h"