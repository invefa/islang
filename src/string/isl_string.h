#ifndef ISC_STRING_H
#define ISC_STRING_H

#include "isl_types.h"

/* c-string, can be char* or ist_string, means a casual string */
typedef ist_char* ist_cstring;

/* ist_string was the byte isl_list actually, restrict type that must allocate at heap */
typedef ist_cstring ist_string;


ist_string  ist_string_consby_ref(ist_cstring _cstring, ist_usize _length);
ist_string* ist_string_initby_ref(ist_string* this, ist_cstring _cstring, ist_usize _length);
ist_string* ist_string_createby_ref(ist_cstring _cstring, ist_usize _length);

ist_string  ist_string_consby_raw(ist_cstring _cstring);
ist_string* ist_string_initby_raw(ist_string* this, ist_cstring _cstring);
ist_string* ist_string_createby_raw(ist_cstring _cstring);

void ist_string_clean(ist_string* this);
void ist_string_delete(ist_string* this);

/**
 * `strbuf` is the abbreviation of `string buffer`, it supply a span to construct a string,
 * The difference between it and string lies in that its main body is not the string itself, but the
 * storage space of the string. It is similar to `char*[1]`, which is a storage space for a string.
 * It does not store the size of the used space itself and is usually used in conjunction with
 * `idxptr` (`index pointer`) for dynamic string construction.
 */
typedef ist_string* ist_strbuf;

ist_strbuf  ist_strbuf_cons(ist_usize _capacity);
ist_strbuf* ist_strbuf_init(ist_strbuf* this, ist_usize _capacity);
// ist_strbuf* ist_strbuf_create(ist_usize _capacity);

void ist_strbuf_clean(ist_strbuf* this);
// void ist_strbuf_delete(ist_strbuf* this);


/* ensure string buffer remaining size enough to required */
void ist_strbuf_ensure(ist_strbuf this, ist_usize size, ist_usize _reqlen);

/* append c raw string to buffer */
ist_string ist_strbuf_append_raw(ist_strbuf this, ist_usize* idxptr, ist_cstring _string);

/* va_args must be end with NULL */
ist_string ist_strbuf_append_raws(ist_strbuf this, ist_usize* idxptr, ist_cstring _string, ...);

/* append reference string to buffer, must supply the length of it */
ist_string ist_strbuf_append_ref(
    ist_strbuf this,
    ist_usize*  idxptr,
    ist_cstring _string,
    ist_usize   _length
);

ist_string ist_strbuf_sprintf(ist_strbuf this, ist_usize* idxptr, ist_cstring _format, ...);

// ist_string* ist_string_sprintf(ist_string* this, ist_cstring _format, ...);

#endif // ISC_STRING_H