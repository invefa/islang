#include "isl_compent_basetype.h"


isn_valueType iscn_basetype_to_isn_valueType(iscn_basetype v) {
    return v;
}

ist_u8 iscn_basetype_sizeof(iscn_basetype v) {
    return isn_valueType_sizeof(v);
}
