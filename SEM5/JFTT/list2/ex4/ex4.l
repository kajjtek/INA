%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    #define STACK_SIZE 100
    int stack[STACK_SIZE];
    int sp = 0;
    int error_flag = 0;

    void yyerror(const char *s);

    void push(int val) {
        if(sp<STACK_SIZE){
            stack[sp++] = val;
        }else {
            yyerror("Stack Overflow");
        }
    }

    int pop(){
        if(sp>0){
            return stack[--sp];
        } else {
            yyerror("Stack empty - not enough operands");
            return 0;
        }
    }
%}
%option reentrant noyywrap
%x PANIC
%%
<PANIC>. {printf("dsadas");}
[ \t]+ ;
\-{0,1}[0-9]+ {
    if(!error_flag){
        int val = atoi(yytext);
        push(val);
    }
}
\+ {
    if(!error_flag){
        int a = pop();
        int b = pop();
        push(b+a);
    }   
}
\- {
    if(!error_flag){
        int a = pop();
        int b = pop();
        push(b-a);
    }
}
\* {
    if(!error_flag){
        int a = pop();
        int b = pop();
        push(a*b);   
    }
}
\/ {
    if(!error_flag){
        int a = pop();
        int b = pop();
        push(b/a);   
    }
}
\^ {
    if(!error_flag){
        int a = pop();
        int b = pop();
        int prod = 1;
        for(int i=0; i<a; i++){
            prod *= b;
        }
        push(prod);   
    }
}
\% {
    if(!error_flag){
        int a = pop();
        int b = pop();
        push(b%a);   
    }
}
\n {
    if(!error_flag){
        int res = pop();
        printf("= %d\n", res);
    }
    sp = 0;
    error_flag = 0;
}
. {
    if(!error_flag){
        yyerror("Wrong input");
    }
}
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    error_flag = 1;
    BEGIN(PANIC);
}

int main(int argc, char **argv) {
    yyscan_t scanner;
    yylex_init(&scanner);
    
    printf("Kalkulator RPN. Wpisz wyrażenie i Enter (Ctrl+D aby zakończyć).\n");
    
    yylex(scanner); 
    
    yylex_destroy(scanner);
    
    return 0;
}
