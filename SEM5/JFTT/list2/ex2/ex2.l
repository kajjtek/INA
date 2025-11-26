%{
%}
%option reentrant noyywrap
%x ONECOMMA
%x DOUBLECOMMA
%x COMMENT_O
%x COMMENT_D
%%
\"\"\" {BEGIN(COMMENT_D);}
\'\'\' {BEGIN(COMMENT_O);}
\" {ECHO;BEGIN(DOUBLECOMMA);}
\' {ECHO;BEGIN(ONECOMMA);}
#.*\n {printf("\n");}

<ONECOMMA>\' {ECHO;BEGIN(INITIAL);}
<ONECOMMA>[^'] {ECHO;}

<DOUBLECOMMA>\" {ECHO;BEGIN(INITIAL);}
<DOUBLECOMMA>[^"] {ECHO;}

<COMMENT_O>\'\'\' {BEGIN(INITIAL);}
<COMMENT_O>\'\'|\' {}
<COMMENT_O>[^'] {}

<COMMENT_D>\"\"\" {BEGIN(INITIAL);}
<COMMENT_D>\"\"|\" {}
<COMMENT_D>[^"] {}

%%
// mozna sie zastanowic jeszcze ale dziala
int main(int argc, char **argv) {
    yyscan_t scanner;
    FILE *f;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
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
