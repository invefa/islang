

/* concat with underline */
#define ISG_UCAT_IMPL(_a, _b) _a##_##_b
#define ISG_UCAT(_a, _b)      ISG_UCAT_IMPL(_a, _b)

/* define STRUCT_NAME for codes */
#if defined(ISG_STRUCT_NAME)
#define STRUCT_NAME ISG_STRUCT_NAME
#else // !defined(ISG_STRUCT_NAME)
#define STRUCT_NAME ISG_UCAT(ISG_VALUE_TYPE, STRUCT_KIND)
#endif

#define ISG_FN_NAME(_name) ISG_UCAT(STRUCT_NAME, _name)
