#ifndef ISC_GENMACROS_H
#define ISC_GENMACROS_H

#include "isl_macros.h"
#include "isl_overload.h"

#define __isl_x_initby_createby_with_consby_packuwraper_0()          _ISL_MACRO_EMPTY
#define __isl_x_initby_createby_with_consby_packuwraper_1(_vargs...) , _vargs
#define __isl_x_initby_createby_with_consby_packuwraper(_vargs...) \
    _isl_binary_overload(__isl_x_initby_createby_with_consby_packuwraper, ##_vargs)
#define __isl_x_initby_createby_with_consby_params(_prefix, _paramspack) \
    _prefix __isl_x_initby_createby_with_consby_packuwraper _paramspack

#define _isl_declare_initby_createby_with_consby(_paramspack, _type, _bywhat) \
    _type* _type##_initby_##_bywhat(                                          \
        __isl_x_initby_createby_with_consby_params(_type* this, _paramspack)  \
    );                                                                        \
    _type* _type##_createby_##_bywhat(_ISL_MACRO_UNPACK _paramspack)

#define _isl_define_initby_createby_with_consby(_paramspack, _paramnamespack, _type, _bywhat) \
    _type* _type##_initby_##_bywhat(                                                          \
        __isl_x_initby_createby_with_consby_params(_type* this, _paramspack)                  \
    ) {                                                                                       \
        *this = _type##_consby_##_bywhat(_ISL_MACRO_UNPACK _paramnamespack);                  \
        return this;                                                                          \
    }                                                                                         \
    _type* _type##_createby_##_bywhat(_ISL_MACRO_UNPACK _paramspack) {                        \
        return _type##_initby_##_bywhat(                                                      \
            __isl_x_initby_createby_with_consby_params(isl_malloc(_type), _paramnamespack)    \
        );                                                                                    \
    }

#define _isl_declare_delete_with_clean(_type) void _type##_delete(_type* this)
#define _isl_define_delete_with_clean(_type) \
    void _type##_delete(_type* this) {       \
        if (!this) return;                   \
        _type##_clean(this);                 \
        isl_free(this);                      \
    }

#endif // ISC_GENMACROS_H
