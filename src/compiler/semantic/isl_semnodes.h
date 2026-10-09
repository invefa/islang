/* unknown */
manifest(unk, (ist_usize))

manifest(fncall, (struct {
             ist_compentRef fn;
             ist_semtrees   args;
             ist_compentRef type;
         }))

manifest(formcall, (struct {
             ist_compentRef form;
             ist_semtrees   args;
             ist_compentRef type;
         }))

manifest(expr, (struct {
             ist_tokenType  opkind;
             ist_semtrees   args;
             ist_compentRef type;
         }))

manifest(use, (struct {
             ist_semtree lhs;
             ist_semtree rhs;
         }))

manifest(name, (ist_compentRef))
manifest(literal, (ist_tvalue))
