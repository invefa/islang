#define STRUCT_KIND list
#include "isg_defines.h"

#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_types.h"

inline ISG_STRUCT_NAME* ISG_FN_NAME(calloc)(ist_usize _capacity) {
    ISG_STRUCT_NAME* this = isl_malloc(ISG_STRUCT_NAME);
    *this = (ISG_STRUCT_NAME){.size = 0, .data = isl_list_calloc(ISG_VALUE_TYPE, _capacity)};
    return this;
}
inline ISG_STRUCT_NAME* ISG_FN_NAME(malloc)(ist_usize _capacity) {
    ISG_STRUCT_NAME* this = isl_malloc(ISG_STRUCT_NAME);
    *this = (ISG_STRUCT_NAME){.size = 0, .data = isl_list_malloc(ISG_VALUE_TYPE, _capacity)};
    return this;
}

inline ISG_STRUCT_NAME* ISG_FN_NAME(create)(ist_usize _capacity, ist_bool _doclean) {
    return _doclean ? ISG_FN_NAME(calloc)(_capacity) : ISG_FN_NAME(malloc)(_capacity);
}

inline void ISG_FN_NAME(resizm)(ISG_STRUCT_NAME* this, ist_usize _newcap) {
    isl_list_resizmv(this->data, _newcap);
}
inline void ISG_FN_NAME(resizc)(ISG_STRUCT_NAME* this, ist_usize _newcap) {
    isl_list_resizcv(this->data, _newcap);
}

inline void ISG_FN_NAME(addm)(ISG_STRUCT_NAME* this, ISG_VALUE_TYPE _value) {
    isl_list_addmv(this->data, this->size, _value);
}
inline void ISG_FN_NAME(addc)(ISG_STRUCT_NAME* this, ISG_VALUE_TYPE _value) {
    isl_list_addcv(this->data, this->size, _value);
}

inline void ISG_FN_NAME(delete)(ISG_STRUCT_NAME* this) {
#if !defined(ISG_VALUE_DONT_CLEAN)
    isl_list_foreach_to (value, this->data, this->size) {
        ISG_UCAT(ISG_VALUE_TYPE, clean)(value);
    }
#endif
    isl_list_freev(this->data);
    isl_free(this);
}

inline ISG_VALUE_TYPE* ISG_FN_NAME(data)(ISG_STRUCT_NAME* this) {
    return this->data;
}
inline ist_usize ISG_FN_NAME(size)(ISG_STRUCT_NAME* this) {
    return this->size;
}

#include "isg_undefs.h"