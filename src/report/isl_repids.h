
/*
    This file was a generating file.
*/

manifest(unknown, (CORE, INFO, NONE), "?")

manifest(custom_core_info, (CORE, INFO, CUSTOM), NULL)
manifest(custom_core_warn, (CORE, WARNING, CUSTOM), NULL)
manifest(custom_core_error, (CORE, ERROR, CUSTOM), NULL)
manifest(custom_core_panic, (CORE, PANIC, CUSTOM), NULL)
manifest(custom_core_fatal, (CORE, FATAL, CUSTOM), NULL)

manifest(unreachable_brench, (CORE, FATAL, NONE), \
    "program went into unreachable brench.")

manifest(utf8_negative_codepoint, (CORE, ERROR, CORELOC), \
    "can't encode negative codepoint.")
