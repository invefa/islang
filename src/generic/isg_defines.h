
/**
 * This file was supplied for all files that with the prefix of isg(that means islang generating).
 * The generating (also call it as generic for islang core codes) make it easy to generate the
 * various collections for any type. The generating codes are used in the lexer, parser, and vm.
 * Now we supplied the isg_list from the original isl_list, it can make codes more readable.
 */


/* concat with underline */
#define ISG_UCAT_IMPL(_a, _b) _a##_##_b
#define ISG_UCAT(_a, _b)      ISG_UCAT_IMPL(_a, _b)

/* define STRUCT_NAME for codes */
#if !defined(ISG_STRUCT_NAME)
#define ISG_STRUCT_NAME ISG_UCAT(ISG_VALUE_TYPE, STRUCT_KIND)
#endif

#define ISG_FN_NAME(_name) ISG_UCAT(ISG_STRUCT_NAME, _name)
