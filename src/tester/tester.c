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

#include <stdlib.h>

#include "isl_test_astnode.h"
#include "isl_test_generic.h"
#include "isl_test_isg_list.h"
#include "isl_test_isl_list.h"
#include "isl_test_lexer.h"
#include "isl_test_memgr.h"
#include "isl_test_overload.h"
#include "isl_test_parser.h"
#include "isl_test_report.h"
#include "isl_test_string.h"
#include "isl_test_xssert.h"



int main(int argc, char* argv[]) {

    if (argc <= 1) system("chcp 65001");
    // isl_test_overload();
    // isl_test_xssert();
    // isl_test_memgr();
    // isl_test_isl_list();
    // isl_test_isg_list();
    isl_test_string();
    // isl_test_report();
    // isl_test_generic();
    // isl_test_lexer();
    // isl_test_astnode();
    isl_test_parser();



    return 0;
}
