#include "isl_lexer.h"

#define ISL_TEST_ITEM lexer
#include "isg_test_defines.h"
ISL_TEST_HEAD {

#define lexer_switch_codepage(_pagename, _pagesrc)                                                \
    ist_lexer_switch_codepage(                                                                    \
        &lexer, ist_codepage_createby_source(&module, ist_string_consby_raw(_pagename), _pagesrc) \
    )


    /* init some basic information */
    ist_string filepath            = ist_string_consby_raw("./scripts/test.is");
    ist_string macro_after_wrapper = ist_string_consby_raw(u8"use main = fn(){\n"
                                                           u8"std::println(\"hello World!\");\n"
                                                           u8"//this is a line comment.\n"
                                                           u8"/* this is a block comment! */\n"
                                                           u8"};");
    ist_string macro_after_lbrace  = ist_string_consby_raw(u8"cast<String>(1)");

    /* print file content */
    ist_string file_contents = isl_read_file(filepath);
    printf("file context:\n%s\n", file_contents);
    ist_string_clean(&file_contents);

    /* construct module and lexer, and create dump buffer */
    ist_module module  = ist_module_consby_filepath(filepath);
    ist_lexer  lexer   = ist_lexer_consby_module(&module);
    ist_strbuf dumpbuf = ist_strbuf_cons(128);

    ist_module_register_string(&module, macro_after_wrapper, ISL_MOSKIND_SOURCE);
    ist_module_register_string(&module, macro_after_lbrace, ISL_MOSKIND_SOURCE);

    /* make sure token dumping synchronized with fn:advance analysis */
    printf("%s\n", ist_token_dump_old(&lexer.cur_token, dumpbuf, NULL));
    printf("%s\n", ist_token_dump_old(&lexer.nex_token, dumpbuf, NULL));


    while (lexer.sec_token.type != ISL_TOKENT_EOF) {
        printf("%s\n", ist_token_dump_old(&lexer.sec_token, dumpbuf, NULL));
        isl_wssert(lexer.sec_token.type != ISL_TOKENT_WRAPPER);
        /* switch codepage if the current token is a wrapper */
        if (lexer.sec_token.type == ISL_TOKENT_WRAPPER)
            lexer_switch_codepage("afterWrapper", macro_after_wrapper);

        /* switch codepage if the current token is a lbrace */
        else if (lexer.sec_token.type == ISL_TOKENT_LBRACE)
            lexer_switch_codepage("afterLBrace", macro_after_lbrace);


        /* lookahead if the current token is add */
        else if (lexer.sec_token.type == ISL_TOKENT_ADD) {

            ist_lexer_lookahead_start(&lexer);
            isl_report(rid_custom_core_warn, "start lookahead.");

            while (lexer.sec_token.type != ISL_TOKENT_EOS && lexer.sec_token.type != ISL_TOKENT_EOF)
            {
                ist_lexer_advance(&lexer);
                printf("ahead: %s\n", ist_token_dump_old(&lexer.sec_token, dumpbuf, NULL));
            }

            ist_lexer_lookahead_end(&lexer);
            isl_report(rid_custom_core_warn, "end lookahead.");
        }

        ist_lexer_advance(&lexer);
    }
    /* print the last token:EOF */
    printf("%s\n", ist_token_dump_old(&lexer.sec_token, dumpbuf, NULL));

    /* clean up */
    ist_lexer_clean(&lexer);
    ist_module_clean(&module);
    ist_string_delete(dumpbuf);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
