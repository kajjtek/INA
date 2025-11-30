%{
    #include <math.h>
    #include <stdio.h>
    int yylex(void);
    void yyerror (char const *);
    #define CONSTANT 1234577L

    long long power(long long base, long long exp) {
        long long res = 1;
        base %= CONSTANT;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % CONSTANT;
            base = (base * base) % CONSTANT;
            exp /= 2;
        }
        return res;
    }

    long long inverse(long long n) {return power(n, CONSTANT - 2);}

    long long normalise(long long input){
        input %= CONSTANT;
        while(input<0) input+= CONSTANT;
        return input;
    }

    long long normalise_exponential(long long input){
        input %= (CONSTANT-1);
        while(input<0) input+= (CONSTANT-1);
        return input;
    }

    long long division(long long a, long long b) {
        b = normalise(b);
        if (b == 0) { yyerror("Dzielenie przez zero!"); return 0; }
        return normalise(normalise(a) * normalise(inverse(b)));
    }

%}
%code requires {
    typedef struct {
        long long val;
        char *string;
        int is_number;
    } Data;
}

%define api.value.type {Data}
%token NUM
%left SUB SUM
%left MUL DIV
%precedence NEG
%left POW
%%
input:
    %empty
| input line
;
line:
    '\n'
| exp '\n' {
    if ($1.string) {
            printf("%s\n", $1.string);
            free($1.string);
        }
        printf("Wynik: %lld\n", normalise($1.val));
    }
| error '\n' {yyerrok;}
;
exp:
    NUM {
        $$.val = $1.val;
        if(asprintf(&$$.string, "%lld", normalise($$.val))==-1) yyerror("MEMORY");
    };
| exp SUM exp {
    $$.val = normalise($1.val + $3.val);
    if(asprintf(&$$.string, "%s %s +", $1.string, $3.string)==-1) yyerror("MEMORY");
    $$.is_number = 0;
    free($1.string);
    free($3.string);
}
| exp SUB exp {
    $$.val = normalise($1.val - $3.val);
    if(asprintf(&$$.string, "%s %s -", $1.string, $3.string)==-1) yyerror("MEMORY");
    $$.is_number = 0;
    free($1.string);
    free($3.string);
}
| exp MUL exp {
    $$.val = normalise($1.val * $3.val);
    if(asprintf(&$$.string, "%s %s *", $1.string, $3.string)==-1) yyerror("MEMORY");
    $$.is_number = 0;
    free($1.string);
    free($3.string);
}
| exp DIV exp {
    if($3.val == 0) {
        yyerror("Dzielenie przez zero");
        $$.val = 0;
    }
    $$.val = division($1.val, $3.val);
    if(asprintf(&$$.string, "%s %s /", $1.string, $3.string)==-1) yyerror("MEMORY");
    $$.is_number = 0;
    free($1.string);
    free($3.string);
}
| SUB NUM %prec NEG {
    $$.val=-$2.val;
    $$.is_number = 1;
    if(asprintf(&$$.string, "%lld", normalise($$.val))==-1) yyerror("MEMORY");
}
| SUB '(' exp ')' %prec NEG {
    $$.val = normalise(-$3.val);
    $$.is_number = 0;
    if(asprintf(&$$.string, "%s -", $3.string)==-1) yyerror("MEMORY");
    free($3.string);
}
| exp POW exp {
    $$.val = normalise(power($1.val, normalise_exponential($3.val)));
    $$.is_number = 0;

    if($3.is_number==1) {
        free($3.string);
        if(asprintf(&$3.string, "%lld",normalise_exponential($3.val))==-1) yyerror("MEMORY");
    }

    if(asprintf(&$$.string, "%s %s ^", $1.string , $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| '(' exp ')' {
    $$.val = normalise($2.val);
    if(asprintf(&$$.string, "%s", $2.string)==-1) yyerror("MEMORY");
}
%%

int main() {
    return yyparse();
}