
manifest(UNKNOWN, NULL)

manifest(EOF, "\0")
manifest(EOS, ";")

manifest(ID, NULL)

manifest(INTEGER_LITERAL, NULL)
manifest(REAL_LITERAL, NULL)
manifest(STRING_LITERAL, NULL)

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

manifest(ADD, "+")
manifest(SUB, "-")
manifest(MUL, "*")
manifest(DIV, "/")
manifest(MOD, "%%")

manifest(ASSIGN, "=")
manifest(ADD_ASSIGN, "+=")
manifest(SUB_ASSIGN, "-=")
manifest(MUL_ASSIGN, "*=")
manifest(DIV_ASSIGN, "/=")
manifest(MOD_ASSIGN, "%%=")


manifest(LNOT, "!")
manifest(LAND, "&&")
manifest(LOR, "||")

manifest(EQUAL, "==")
manifest(NOTEQUAL, "!=")
manifest(LESSTHAN, "<")
manifest(GREATERTHAN, ">")
manifest(LESSEQUAL, "<=")
manifest(GREATEQUAL, ">=")

manifest(KW_START, NULL)

manifest(BOL_TRUE, "true")
manifest(BOL_FALSE, "false")

manifest(INT, "int")
manifest(BOOL, "bool")
manifest(BYTE, "byte")
manifest(REAL, "real")
manifest(STRING, "string")

manifest(FN, "fn")
manifest(LET, "let")

manifest(IF, "if")
manifest(FOR, "for")
manifest(WHILE, "while")

manifest(ELSE, "else")
manifest(BREAK, "break")
manifest(CONTINUE, "continue")

manifest(RETURN, "return")

manifest(ENUM, "enum")
manifest(STRUCT, "struct")

manifest(KW_END, NULL)
