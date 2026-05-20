%{
    #include <math.h>
    #include <stdio.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    int yylex(void);
    void yyerror (char const *);
    #define CONSTANT 1234577L
    #define MAX_NUM_LEN 12

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

    long long power_exponential(long long base, long long exp) {
        long long res = 1;
        base %= (CONSTANT-1);
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % (CONSTANT-1);
            base = (base * base) % (CONSTANT-1);
            exp /= 2;
        }
        return res;
    }

    long long inverse_exponential(long long n) {return power_exponential(n, CONSTANT - 3);}

    long long division_exponential(long long a, long long b) {
        b = normalise_exponential(b);
        if (b == 0) { yyerror("Dzielenie przez zero!"); return 0; }
        return normalise_exponential(normalise_exponential(a) * normalise_exponential(inverse_exponential(b)));
    }
    

%}
%code requires {
    typedef struct {
        long long val;
        char *string;
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
    free($1.string);
    free($3.string);
}
| exp SUB exp {
    $$.val = normalise($1.val - $3.val);
    if(asprintf(&$$.string, "%s %s -", $1.string, $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| exp MUL exp {
    $$.val = normalise($1.val * $3.val);
    if(asprintf(&$$.string, "%s %s *", $1.string, $3.string)==-1) yyerror("MEMORY");
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
    free($1.string);
    free($3.string);
}
| SUB NUM %prec NEG {
    $$.val=-$2.val;
    if(asprintf(&$$.string, "%lld", normalise($$.val))==-1) yyerror("MEMORY");
}
| SUB '(' exp ')' %prec NEG {
    $$.val = normalise(-$3.val);
    if(asprintf(&$$.string, "%s -", $3.string)==-1) yyerror("MEMORY");
    free($3.string);
}
| exp POW exp_pow {
    $$.val = normalise(power($1.val, normalise_exponential($3.val)));

    if(asprintf(&$$.string, "%s %s ^", $1.string , $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| '(' exp ')' {
    $$.val = normalise($2.val);
    if(asprintf(&$$.string, "%s", $2.string)==-1) yyerror("MEMORY");
}

exp_pow:
    NUM {
        $$.val = $1.val;
        if(asprintf(&$$.string, "%lld", normalise_exponential($$.val))==-1) yyerror("MEMORY");
    };
| exp_pow SUM exp_pow {
    $$.val = normalise_exponential($1.val + $3.val);
    if(asprintf(&$$.string, "%s %s +", $1.string, $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| exp_pow SUB exp_pow {
    $$.val = normalise_exponential($1.val - $3.val);
    if(asprintf(&$$.string, "%s %s -", $1.string, $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| exp_pow MUL exp_pow {
    $$.val = normalise_exponential($1.val * $3.val);
    if(asprintf(&$$.string, "%s %s *", $1.string, $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| exp_pow DIV exp_pow {
    if($3.val == 0) {
        yyerror("Dzielenie przez zero");
        $$.val = 0;
    }
    $$.val = division_exponential($1.val, $3.val);
    if(asprintf(&$$.string, "%s %s /", $1.string, $3.string)==-1) yyerror("MEMORY");
    free($1.string);
    free($3.string);
}
| SUB NUM %prec NEG {
    $$.val=-$2.val;
    if(asprintf(&$$.string, "%lld", normalise_exponential($$.val))==-1) yyerror("MEMORY");
}
| SUB '(' exp_pow ')' %prec NEG {
    $$.val = normalise_exponential(-$3.val);
    if(asprintf(&$$.string, "%s -", $3.string)==-1) yyerror("MEMORY");
    free($3.string);
}
| '(' exp_pow ')' {
    $$.val = normalise_exponential($2.val);
    if(asprintf(&$$.string, "%s", $2.string)==-1) yyerror("MEMORY");
}

%%

int main() {
    return yyparse();
}