
manifest(unk, (ist_usize))

manifest(entref, (struct { ist_u32 index; }))

manifest(name, (struct { ist_string name; }))
manifest(basetype, (struct { ist_basetype type; }))

manifest(expr, (struct {
             ist_vptr ceirs;
             ist_bool is_const;
         }))
