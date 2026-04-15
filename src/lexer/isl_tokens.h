
manifest(UNKNOWN, NULL)

/* end of file */
manifest(EOF, "\0")

/* end of statement */
manifest(EOS, ";")

/* end of parsent(parse-time entity) */
manifest(EOP, "`")

manifest(ID, NULL)

/* value literals */
manifest(VL_INT, NULL)
manifest(VL_REAL, NULL)
manifest(VL_STRING, NULL)

/* keywords */
manifest(START_KEYWORDS, NULL)

manifest(KW_IMPORT, "import")
manifest(KW_USE, "use")
manifest(KW_DO, "do")

manifest(KW_SPACE, "space")
manifest(KW_CONST, "const")
manifest(KW_type, "type")
manifest(KW_SPAN, "span")
manifest(KW_STRUCT, "struct")
manifest(KW_UNION, "union")
manifest(KW_ENUM, "enum")
manifest(KW_FN, "fn")
manifest(KW_FORM, "form")
manifest(KW_CONCEPT, "concept")

manifest(KW_IF, "if")
manifest(KW_FOR, "for")
manifest(KW_WHILE, "while")

manifest(KW_ELSE, "else")
manifest(KW_BREAK, "break")
manifest(KW_CONTINUE, "continue")

manifest(KW_RETURN, "return")
manifest(KW_DONE, "done")

manifest(KW_LET, "let")
manifest(KW_VAR, "var")

/* value types */
manifest(START_VALUE_TYPES, NULL)

manifest(VT_VOID, "void")
manifest(VT_I8, "i8")
manifest(VT_I16, "i16")
manifest(VT_I32, "i32")
manifest(VT_I64, "i64")
manifest(VT_U8, "u8")
manifest(VT_U16, "u16")
manifest(VT_U32, "u32")
manifest(VT_U64, "u64")
manifest(VT_F32, "f32")
manifest(VT_F64, "f64")
manifest(VT_INT, "int")
manifest(VT_UINT, "uint")
manifest(VT_REAL, "real")
manifest(VT_BYTE, "byte")
manifest(VT_BOOL, "bool")
manifest(VT_STR, "str")
// manifest(VTS_UNSIGNED, "unsigned")

manifest(END_VALUE_TYPES, NULL)

/* bool values */
manifest(BV_TRUE, "true")
manifest(BV_FALSE, "false")

manifest(END_KEYWORDS, NULL)

manifest(LPARE, "(")
manifest(RPARE, ")")
manifest(LBRACE, "{")
manifest(RBRACE, "}")
manifest(LBRACKET, "[")
manifest(RBRACKET, "]")

manifest(DOT, ".")
manifest(COMMA, ",")
manifest(COLON, ":")
manifest(QUESTION, "?")
manifest(WRAPPER, "->")

manifest(SELFADD, "++")
manifest(SELFSUB, "--")

manifest(ADD, "+")
manifest(SUB, "-")
manifest(MUL, "*")
manifest(DIV, "/")
manifest(MOD, "%")

manifest(ASSIGN, "=")
manifest(ADD_ASSIGN, "+=")
manifest(SUB_ASSIGN, "-=")
manifest(MUL_ASSIGN, "*=")
manifest(DIV_ASSIGN, "/=")
manifest(MOD_ASSIGN, "%=")


manifest(LNOT, "!")
manifest(LAND, "&&")
manifest(LOR, "||")

manifest(NOT, "~")
manifest(AND, "&")
manifest(OR, "|")
manifest(XOR, "^")

manifest(LSHIFT, "<<")
manifest(RSHIFT, ">>")
// manifest(BRSHIFT_ASSIGN, ">>=")
// manifest(BLSHIFT_ASSIGN, "<<=")


manifest(EQUAL, "==")
manifest(NOTEQUAL, "!=")
manifest(LESSTHAN, "<")
manifest(GREATERTHAN, ">")
manifest(LESSEQUAL, "<=")
manifest(GREATEQUAL, ">=")

manifest(LATEST, NULL)