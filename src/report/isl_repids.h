
/*
    This file is used to generate various information of the specified enumeration value,
    which can make maintenance more convenient.
    Written by invefa.
*/

manifest(unknown, (CORE, INFO, NONE), "?")


manifest(custom_core_info, (CORE, INFO, CUSTOM), NULL)
manifest(custom_core_warning, (CORE, WARNING, CUSTOM), NULL)
manifest(custom_core_error, (CORE, ERROR, CUSTOM), NULL)
manifest(custom_core_panic, (CORE, PANIC, CUSTOM), NULL)
manifest(custom_core_fatal, (CORE, FATAL, CUSTOM), NULL)

manifest(utf8_negative_codepoint, (CORE, ERROR, CORELOC), "cannot encode negative codepoint.")
manifest(open_file_failed, (CORE, FATAL, CORELOC), "failed to open file '%s', does it exist?")
manifest(unreachable_brench, (CORE, FATAL, CORELOC), "the program entered an unreachable branch...")
manifest(catch_size_overflow, (CORE, FATAL, CORELOC), "catched an overflow size.")
manifest(illegal_utf8_codepoint, (CORE, FATAL, CORELOC), "cannot encode illegal UTF-8 codepoint.")
manifest(
    catch_nullptr,
    (CORE, FATAL, CORELOC),
    "catched a nullptr, the address should not be null."
)
manifest(
    unknown_enum_value,
    (CORE, FATAL, CORELOC),
    "unknown enumeration value: <0x%zX>, please check the code."
)
manifest(
    illegal_utf8_sequence,
    (CORE, FATAL, CORELOC),
    "cannot decode illegal UTF-8 sequence, did you get the index wrong?"
)
manifest(
    incomplete_utf8_sequence,
    (CORE, FATAL, CORELOC),
    "cannot decode incomplete UTF-8 sequence, the buffer is not long enough."
)


manifest(
    inform_allocated,
    (CORE, INFO, NONE),
    "allocate <0x%zX> {length=%zu}, then allocated-length=%zu."
)
manifest(
    inform_released,
    (CORE, INFO, NONE),
    "released <0x%zX> {length=%zu}, then allocated-length=%zu."
)
manifest(
    inform_buffer_reisze,
    (CORE, INFO, NONE),
    "buffer<0x%zX> {capacity=%zu, remaining=%zu} ->require(%zu) ->resize(%zu)."
)
manifest(
    inform_list_reisze,
    (CORE, INFO, NONE),
    "list<0x%zX> {capacity=%zu, remaining=%zu} ->require(%zu) ->resize(%zu)."
)


manifest(
    catch_zero_string_length,
    (CORE, WARNING, CORELOC),
    "there is a zero-length string: <0x%zX>, please note its existence."
)
manifest(
    catch_zero_capacity_list,
    (CORE, WARNING, CORELOC),
    "there is a zero-capacity list: <0x%zX>, please note its existence."
)
manifest(
    catch_zero_capacity_buffer,
    (CORE, WARNING, CORELOC),
    "there is a zero-capacity buffer<0x%zX>, please note its existence."
)


manifest(
    advance_codepoint_when_eof,
    (LEXER, WARNING, CORELOC),
    "advance codepoint when we reach the end of source."
)
manifest(
    unterminated_comment_block,
    (LEXER, WARNING, USERLOC),
    "block comments do not end with '*/', but with the end of file."
)
manifest(
    unterminated_string,
    (LEXER, WARNING, USERLOC),
    "the string do not end with '\"', but with the end of file."
)
manifest(
    unrecongnized_codepoint,
    (LEXER, WARNING, USERLOC),
    "unrecongnized unicode codepoint: <0x%X>, it will be skipped."
)
manifest(
    is_it_the_version_code,
    (LEXER, WARNING, USERLOC),
    "are you sure? is it the version code? the second dot and its suffix numbers will be ignored."
)
