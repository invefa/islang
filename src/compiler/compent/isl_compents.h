
manifest(unk, (ist_usize))

manifest(entref, (struct { ist_u32 index; }))

manifest(name, (struct { ist_string name; }))
manifest(basetype, (struct { iscen_basetype type; }))

manifest(expr, (struct {
             ist_semtree    st;   /* semantical tree */
             ist_compentRef type; /* the type of this expr */
         }))
