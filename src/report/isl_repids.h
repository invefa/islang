
/*
    This file is used to generate various information of the specified enumeration value,
    which can make maintenance more convenient.
    Written by invefa.
*/

manifest(unknown, (CORE, INFO, NONE), "?")


manifest(custom_core_info, (CORE, INFO, CUSTOM), NULL)
manifest(custom_core_warn, (CORE, WARNING, CUSTOM), NULL)
manifest(custom_core_error, (CORE, ERROR, CUSTOM), NULL)
manifest(custom_core_panic, (CORE, PANIC, CUSTOM), NULL)
manifest(custom_core_fatal, (CORE, FATAL, CUSTOM), NULL)


manifest(unreachable_brench, (CORE, FATAL, NONE),
"the program entered an unreachable branch...")
manifest(catch_nullptr, (CORE, FATAL, CORELOC),
"catched a nullptr, the address should not be null.")


manifest(utf8_negative_codepoint, (CORE, ERROR, CORELOC),
"cannot encode negative codepoint.")
manifest(illegal_utf8_codepoint, (CORE, ERROR, CORELOC),
"cannot encode illegal utf8 codepoint.")
manifest(illegal_utf8_sequence, (CORE, ERROR, CORELOC),
"cannot decode illegal utf8 sequence, did you get the index wrong?")


manifest(inform_allocated, (CORE, INFO, NONE),
"allocate <0x%p> {length=%llu}, then allocated-length=%llu.")
manifest(inform_released, (CORE, INFO, NONE),
"released <0x%p> {length=%llu}, then allocated-length=%llu.")
manifest(inform_buffer_reisze, (CORE, INFO, NONE),
"buffer<0x%p> {capacity=%llu, remaining=%llu} ->require(%llu) ->resize(%llu).")


manifest(catch_zero_string_length, (CORE, WARNING, CORELOC),
"there is a zero-length string: <0x%p>, please note its existence.")
manifest(catch_zero_capacity_list, (CORE, WARNING, CORELOC),
"there is a zero-capacity list: <0x%p>, please note its existence.")
manifest(catch_zero_capacity_buffer, (CORE, WARNING, CORELOC),
"there is a zero-capacity buffer<0x%p>, please note its existence.")

manifest(advance_codepoint_when_eof, (LEXER, ERROR, CORELOC),
"advance codepoint when EOF.")
manifest(unterminated_comment_block, (LEXER, WARNING, NONE),
"the comment block is not terminated with '*/' but catch the EOF.")
manifest(unrecongnized_codepoint, (LEXER, ERROR, NONE),
"unrecongnized codepoint: <0x%x>, the lexer will skip it.")