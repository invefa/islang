
manifest(unknown, (ist_compent))

manifest(entref, (struct {
             ist_compent base;
             ist_usize   index;
         }))

manifest(name, (struct {
             ist_compent base;
             ist_string  name;
         }))

manifest(expr, (struct {
             ist_compent  base;
             ist_astnode* node;
             ist_bool     is_const;
         }))
