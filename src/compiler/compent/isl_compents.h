
manifest(unk, (ist_usize))

manifest(entref, (struct { ist_u32 index; }))

manifest(name, (struct { ist_string name; }))
manifest(basetype, (struct { iscet_basetype type; }))

manifest(expr, (struct {
             ist_semtree    st;              /* semantical tree */
             ist_compentRef type;            /* the type of this expr */
             ist_bool       has_span    : 1; /* has span/span-ref */
             ist_bool       has_fncall  : 1; /* has fn-call */
             ist_bool       has_formcall: 1; /* has form-call */
             ist_bool       is_const    : 1; /* is const value */
         }))
