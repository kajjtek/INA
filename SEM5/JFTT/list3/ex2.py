import ply.lex as lex
import ply.yacc as yacc
import sys

# --- KONFIGURACJA I MATEMATYKA ---

CONSTANT = 1234577

def normalise(n):
    """Normalizuje liczbę do zakresu [0, CONSTANT-1]"""
    n %= CONSTANT
    while n < 0:
        n += CONSTANT
    return n

def normalise_exponential(n):
    """Normalizuje wykładnik (Małe Twierdzenie Fermata)"""
    n %= (CONSTANT - 1)
    while n < 0:
        n += (CONSTANT - 1)
    return n

def power(base, exp):
    """Szybkie potęgowanie modularne"""
    res = 1
    base %= CONSTANT
    while exp > 0:
        if exp % 2 == 1:
            res = (res * base) % CONSTANT
        base = (base * base) % CONSTANT
        exp //= 2
    return res

def inverse(n):
    """Odwrotność modularna"""
    return power(n, CONSTANT - 2)

def division(a, b):
    """Dzielenie modularne"""
    b = normalise(b)
    if b == 0:
        print("Error: Dzielenie przez zero!")
        return 0
    return normalise(normalise(a) * normalise(inverse(b)))

# --- LEXER (Analizator leksykalny) ---

tokens = (
    'NUM',
    'SUM', 'SUB', 'MUL', 'DIV', 'POW',
    'LPAREN', 'RPAREN',
    'NEWLINE'
)

# Definicje stanów (do obsługi komentarzy)
states = (
    ('COMMA', 'exclusive'),
)

# Proste tokeny
t_SUM    = r'\+'
t_SUB    = r'-'
t_MUL    = r'\*'
t_DIV    = r'/'
t_POW    = r'\^'
t_LPAREN = r'\('
t_RPAREN = r'\)'

# Ignorowanie znaków
t_ignore = ' \t'
t_COMMA_ignore = ''

# Obsługa liczb
def t_NUM(t):
    r'[0-9]+'
    t.value = int(t.value)
    return t

# --- Obsługa komentarzy (zgodna z calc.l) ---

# Wejście w tryb komentarza po znaku '#'
def t_begin_COMMA(t):
    r'\#'
    t.lexer.begin('COMMA')

# Ignorowanie "escaped newline" w trybie normalnym
def t_escaped_newline(t):
    r'\\\n'
    pass  # Ignoruj

# W trybie COMMA: ignoruj "escaped newline" (kontynuacja komentarza)
def t_COMMA_escaped_newline(t):
    r'\\\n'
    pass

# W trybie COMMA: zwykły newline kończy komentarz
def t_COMMA_newline(t):
    r'\n'
    t.lexer.begin('INITIAL')
    # Nie zwracamy tokenu NEWLINE tutaj, bo w C: <COMMA>\n {BEGIN(INITIAL);}

# W trybie COMMA: ignoruj wszystko inne
def t_COMMA_content(t):
    r'[^\n\\]+'
    pass

def t_COMMA_backslash(t):
    r'\\'
    pass

# Obsługa nowej linii w trybie normalnym (ważne dla gramatyki)
def t_NEWLINE(t):
    r'\n'
    t.lexer.lineno += 1
    return t

def t_error(t):
    print(f"Error: Wrong input at '{t.value[0]}'")
    t.lexer.skip(1)

def t_COMMA_error(t):
    t.lexer.skip(1)

# Budowa lexera
lexer = lex.lex()

# --- PARSER (Analizator składniowy) ---

# Precedencje operatorów (zgodne z calculator.y)
precedence = (
    ('left', 'SUM', 'SUB'),
    ('left', 'MUL', 'DIV'),
    ('left', 'NEG'),  # Priorytet dla unarnego minusa
    ('left', 'POW'),
)

# Struktura danych przekazywana w p[]: słownik {'val': int, 'string': str}

def p_input(p):
    '''input : 
             | input line'''
    pass

def p_line_newline(p):
    '''line : NEWLINE'''
    pass

def p_line_exp(p):
    '''line : exp NEWLINE'''
    # Wypisz ciąg RPN (jeśli istnieje) i wynik
    if p[1]['string']:
        print(p[1]['string'])
    print(f"Wynik: {normalise(p[1]['val'])}")

def p_line_error(p):
    '''line : error NEWLINE'''
    # yyerrok jest obsługiwane automatycznie przez PLY wznawiając parsowanie
    pass

def p_exp_num(p):
    '''exp : NUM'''
    val = normalise(p[1])
    p[0] = {
        'val': val,
        'string': str(val),
        'is_number': True
    }

def p_exp_binary(p):
    '''exp : exp SUM exp
           | exp SUB exp
           | exp MUL exp
           | exp DIV exp
           | exp POW exp'''
    
    val1 = p[1]['val']
    val3 = p[3]['val']
    op = p[2]
    
    res_val = 0
    op_char = ''
    
    if op == '+':
        res_val = normalise(val1 + val3)
        op_char = '+'
    elif op == '-':
        res_val = normalise(val1 - val3)
        op_char = '-'
    elif op == '*':
        res_val = normalise(val1 * val3)
        op_char = '*'
    elif op == '/':
        if val3 == 0:
            print("Error: Dzielenie przez zero") # Logika z pliku .y
            res_val = 0
        else:
            res_val = division(val1, val3)
        op_char = '/'
    elif op == '^':
        # Specjalna logika dla POW z calculator.y
        exp_val = normalise_exponential(val3)
        res_val = normalise(power(val1, exp_val))
        op_char = '^'
        
        # Jeśli wykładnik był liczbą, aktualizujemy jego string (zgodnie z logiką C)
        str3 = p[3]['string']
        if p[3].get('is_number', False):
             str3 = str(exp_val)
        
        # Nadpisz string dla prawej strony, by użyć znormalizowanego wykładnika
        p[3]['string'] = str3

    # Konstrukcja stringa RPN: "A B op"
    # Uwaga: Dla POW używamy zaktualizowanego p[3]['string']
    str1 = p[1]['string']
    str3 = p[3]['string']
    
    p[0] = {
        'val': res_val,
        'string': f"{str1} {str3} {op_char}",
        'is_number': False
    }

def p_exp_unary_minus_num(p):
    '''exp : SUB NUM %prec NEG'''
    # Specyficzna logika z C: | SUB NUM ...
    val = -p[2]
    norm_val = normalise(val)
    p[0] = {
        'val': val, # C kod ustawia $$.val = -$2.val (nieznormalizowane tutaj, ale normalizowane przy wypisywaniu stringa)
        'string': str(norm_val),
        'is_number': True
    }

def p_exp_unary_minus_group(p):
    '''exp : SUB LPAREN exp RPAREN %prec NEG'''
    # Specyficzna logika z C: | SUB '(' exp ')' ...
    val = normalise(-p[3]['val'])
    p[0] = {
        'val': val,
        'string': f"{p[3]['string']} -",
        'is_number': False
    }

def p_exp_group(p):
    '''exp : LPAREN exp RPAREN'''
    val = normalise(p[2]['val'])
    p[0] = {
        'val': val,
        'string': p[2]['string'],
        'is_number': p[2].get('is_number', False)
    }

def p_error(p):
    if p:
        print(f"Error: Syntax error at '{p.value}'")
    else:
        print("Error: Syntax error at EOF")

# Budowa parsera
parser = yacc.yacc()

# --- GŁÓWNA PĘTLA ---

def main():
    print(f"Kalkulator Modularny (Z_p, p={CONSTANT})")
    print("Wpisz wyrażenie i naciśnij Enter. Ctrl+C lub Ctrl+D aby wyjść.")
    
    while True:
        try:
            s = input('> ')
            if not s: continue 
            # Dodajemy nową linię, bo gramatyka oczekuje tokenu NEWLINE na końcu 'line'
            s += '\n'
            parser.parse(s, lexer=lexer)
        except EOFError:
            break
        except KeyboardInterrupt:
            break
        except Exception as e:
            print(f"Error: {e}")

if __name__ == '__main__':
    main()