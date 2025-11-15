%{
    #include <stdbool.h>
    bool keep_doc_comments = false;
%}
%option reentrant noyywrap
%x DOXYGEN
%x ONECOMMA
%x DOUBLECOMMA
%x COMMENT_MULTI
%%
\" {ECHO;BEGIN(DOUBLECOMMA);}
\' {ECHO;BEGIN(ONECOMMA);}
\/\*\*\/ {}
\/\* {BEGIN(COMMENT_MULTI);}
\/\*\* {
    if(keep_doc_comments){
        ECHO;
    }
    BEGIN(DOXYGEN);}
^#include[[:space:]]+\<[^\n]*\> {ECHO;}
^#include[[:space:]]+\"[^\n]*\" {ECHO;}
\/{2}(\!|\/)(.*\\\n)*.*\n {
    if(keep_doc_comments){
        ECHO;
    }
}
\/{2}(.*\\\n)*.*\n {}
. {ECHO;}

<ONECOMMA>\\\' {ECHO;}
<ONECOMMA>\' {ECHO;BEGIN(INITIAL);}
<ONECOMMA>[^'] {ECHO;}

<DOUBLECOMMA>[^"]|\\\" {ECHO;}
<DOUBLECOMMA>\" {ECHO;BEGIN(INITIAL);}


<COMMENT_MULTI>\*\/ {BEGIN(INITIAL);}
<COMMENT_MULTI>\n {}
<COMMENT_MULTI>. {}

<DOXYGEN>\*\/ {
    if(keep_doc_comments){
        ECHO;
    }
    BEGIN(INITIAL);}
<DOXYGEN>\n {
    if(keep_doc_comments){
        ECHO;
    }
}
<DOXYGEN>. {
    if(keep_doc_comments){
        ECHO;
    }
}
%%
int main(int argc, char **argv) {
    yyscan_t scanner;
    FILE *f;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }
    if (argc < 3) {
        keep_doc_comments = false;
    } else {
        char flag = *argv[2];
        if(flag=='f') keep_doc_comments = true;
    }

    f = fopen(argv[1], "r");

    if (!f) {
        perror(argv[1]);
        return 1;
    }

    yylex_init(&scanner);
    yyset_in(f, scanner);

    yylex(scanner);

    yylex_destroy(scanner);
    fclose(f);
    return 0;
}
