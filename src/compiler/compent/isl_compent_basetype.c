#include "isl_compent_basetype.h"


isn_valueType iscen_basetype_to_isn_valueType(iscen_basetype v) {
    return v;
}

ist_u8 iscen_basetype_sizeof(iscen_basetype v) {
    return isn_valueType_sizeof(v);
}
