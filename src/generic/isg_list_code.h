#define STRUCT_KIND list
#include "isg_defines.h"

#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_types.h"

inline STRUCT_NAME* ISG_FN_NAME(calloc)(ist_usize _capacity) {
    STRUCT_NAME* this = isl_malloc(STRUCT_NAME);
    this->size        = 0;
    this->data        = isl_list_calloc(ISG_VALUE_TYPE, _capacity);
    return this;
}
inline STRUCT_NAME* ISG_FN_NAME(malloc)(ist_usize _capacity) {
    STRUCT_NAME* this = isl_malloc(STRUCT_NAME);
    this->size        = 0;
    this->data        = isl_list_malloc(ISG_VALUE_TYPE, _capacity);
    return this;
}

inline STRUCT_NAME* ISG_FN_NAME(create)(ist_usize _capacity, ist_bool _doclean) {
    return _doclean ? ISG_FN_NAME(calloc)(_capacity) : ISG_FN_NAME(malloc)(_capacity);
}

inline void ISG_FN_NAME(resizm)(STRUCT_NAME* this, ist_usize _newcap) {
    isl_list_resizm(this->data, _newcap);
}
inline void ISG_FN_NAME(resizc)(STRUCT_NAME* this, ist_usize _newcap) {
    isl_list_resizc(this->data, _newcap);
}

inline void ISG_FN_NAME(addm)(STRUCT_NAME* this, ISG_VALUE_TYPE _value) {
    isl_list_addm(this->data, this->size, _value);
}
inline void ISG_FN_NAME(addc)(STRUCT_NAME* this, ISG_VALUE_TYPE _value) {
    isl_list_addc(this->data, this->size, _value);
}

inline void ISG_FN_NAME(delete)(STRUCT_NAME* this) {
#if !defined(ISG_VALUE_DONT_CLEAN)
    isl_list_foreach_to (value, this->data, this->size) {
        ISG_UCAT(ISG_VALUE_TYPE, clean)(value);
    }
#endif
    isl_list_freev(this->data);
    isl_free(this);
}

inline ISG_VALUE_TYPE* ISG_FN_NAME(data)(STRUCT_NAME* this) {
    return this->data;
}
inline ist_usize ISG_FN_NAME(size)(STRUCT_NAME* this) {
    return this->size;
}

#include "isg_undefs.h"