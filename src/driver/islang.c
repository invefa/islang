/**
 * This c source file is the main source file for islang, and it is the entrance of islang.
 * This project is compiled with -std=gnu11, because it will use some extension functions
 * which are suplied by GNU standard extensions.
 *
 * There are a list of extensions are used:
 *  - Macros with a Variable Number of Arguments.
 *  - Statements and Declarations in Expressions.
 *  - Arithmetic on void- and Function-Pointers.
 *  - Conditionals with Omitted Operands.
 *  - Referring to a Type with typeof.
 *  - u8 prefix for string literal.
 *  - Function Names as Strings.
 *  - Designated Initializers.
 *  - Compound Literals.
 *
 * For more details about them, you can view:
 * https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html#C-Extensions
 *
 * Anyway, you had better compile this project with gcc -std=gnu11.
 * Written by invefa.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "isl_ansictrl.h"
#include "isl_astnode.h"
#include "isl_dbgutils.h"
#include "isl_lexer.h"
#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_overload.h"
#include "isl_parser.h"
#include "isl_report.h"
#include "isl_string.h"
#include "isl_utf8.h"
#include "isl_xssert.h"


int main(int argc, char* argv[]) {
    printf("hello from isalng.\n");
    return 0;
}
