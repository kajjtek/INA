%{
    #include <stdio.h>

    struct stats {
        int num_lines;
        int num_words;
    };
%}
%option reentrant noyywrap
%option extra-type="struct stats*"
%%
^[ \t\r\v\f]*\n    {}
^[ \t\r\v\f]+      {}
[ \t\r\v\f]+\n {
    struct stats *ns = yyget_extra(yyscanner);
    ns->num_lines++;
    printf("\n");
}
[ \t]+ {
    printf(" ");
}
[^[:space:]]+ {
    struct stats *ns = yyget_extra(yyscanner);
    ns->num_words++;
    ECHO;
}
\n {
    struct stats *ns = yyget_extra(yyscanner);
    ns->num_lines++;
    ECHO;
}
%%
int main(int argc, char **argv) {
    yyscan_t scanner;
    struct stats ns = {0, 0};
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
    yyset_extra(&ns, scanner);
    yyset_in(f, scanner);   // <--- important

    yylex(scanner);

    printf("# of lines = %d, # of words = %d\n",
           ns.num_lines, ns.num_words);

    yylex_destroy(scanner);
    fclose(f);
    return 0;
}
