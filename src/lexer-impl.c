#ifndef _LEXER_IMPL_C_
#define _LEXER_IMPL_C_

#include "lexer-headers.h"




YYSTYPE
make_double (double value)
{
    YYSTYPE dst;
    dst = malloc (sizeof (_YYSTYPE));
    dst-> type = TYPE_DOUBLE;
    dst-> value. dvalue = value;
    return dst;
}
#endif
