%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "./calculator.tab.h"

    void yyerror(const char *s);
%}
%x COMMA
%%

<COMMA>\\\n ;
<COMMA>\n {BEGIN(INITIAL);}
<COMMA>. ;

\\\n ;
\# {BEGIN(COMMA);}
[ \t]+ ;
[0-9]+ {
    yylval.val = atoll(yytext);
    yylval.is_number = 1;
    return NUM;
}
\+ {
    return SUM;
}
\- {
    return SUB;
}
\* {
    return MUL;
}
\/ {
    return DIV;
}
\^ {
    return POW;
}
\( {
    return '(';
}
\) {
    return ')';
}
\n {
    return '\n';
}
. {
    yyerror("Wrong input");
}

%%
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
