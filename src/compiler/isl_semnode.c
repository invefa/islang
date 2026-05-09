#include "isl_semnode.h"

#include "inttypes.h"
#include "isl_genmacros.h"


ist_string ist_semnodeKindNames[] = {
#define manifest(_name, _struct) [ist_semnodeKind_##_name] = #_name,
#include "isl_semnodes.h"
#undef manifest
};


#define ISG_STRUCT_NAME           ist_semtreeList
#define ISG_VALUE_TYPE            ist_semtree
#define ISG_VALUE_FN_CLEAN(_st_p) ist_semtree_delete(*(_st_p))
#include "isg_list_code.h"


ist_semnode ist_semnode_consby_full(ist_semnodeKind kind, ist_location loc, ist_semnodeAs as) {
    return ist_semnode_{.kind = kind, .location = loc, .as = as};
}
_isl_define_initby_createby_with_consby(
    (ist_semnodeKind kind, ist_location loc, ist_semnodeAs as),
    (kind, loc, as),
    ist_semnode,
    full
);

ist_semnode ist_semnode_consby_location(ist_location loc) {
    return ist_semnode_{.location = loc};
}
_isl_define_initby_createby_with_consby((ist_location loc), (loc), ist_semnode, location);


void ist_semnode_clean(ist_semnode* this) {}
void ist_semnode_delete(ist_semnode* this) {
    if (!this) return;
    ist_semnode_clean(this);
    isl_free(this);
}


void ist_semtree_delete(ist_semtree this) {
    if (!this) return;
    switch (this->kind) {
        case ist_semnodeKind_unk:
            break;
        default:
            isp_unreachable();
    }
    ist_semnode_delete(this);
}

ist_string ist_semtreeptr_dump(ist_semtree* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    if (!this) isp_unreachable();
    return ist_semtree_dump(*this, dctx);
}

ist_string ist_semtree_dump(ist_semtree this, ist_dumpctx dctx) {
    isl_dreport(rid_inform_dumping, "semnode", this);
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    if (!this) return ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "null");

    switch (this->kind) {
        case ist_semnodeKind_unk:
            break;

        default:
            isp_unreachable();
            break;
    }

    return *dctx.buffer;
}
