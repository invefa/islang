
manifest(unk, (ist_usize))

manifest(entref, (struct { ist_u32 index; }))

manifest(name, (struct { ist_string ident; }))
manifest(basetype, (struct { iscn_basetype type; }))
manifest(type, (struct {
             ist_semtree repr;
             ist_u32     uuid;
         }))

manifest(expr, (struct {
             ist_semtree    repr; /* semantical tree */
             ist_compentRef type; /* the type of this expr */
         }))
