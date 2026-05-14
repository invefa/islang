/* unknown */
manifest(unk, (ist_usize))


manifest(apply, (struct {
             ist_compentRef operator;
             ist_semtrees   operands;
             ist_compentRef type;
         }))

manifest(use, (struct {
             ist_semtree lhs;
             ist_semtree rhs;
         }))

manifest(name, (ist_compentRef))
manifest(literal, (ist_tvalue))
