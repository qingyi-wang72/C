#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include "calc.h"

/* Function */
int main(void){
    char input[BUFFER_SIZE];
    lexer_t lexer = (lexer_t)malloc(sizeof(lexer_s));
    poly_t output = NULL;
    int err = 0;

    printf("Cursed Calculator\n");
    printf(">>> ");

    // Reading input
    while(fgets(input, BUFFER_SIZE, stdin) != NULL) {
        if(strlen(input) == 2){
            if(input[0] == 'h'){
                help_function();
                printf(">>> ");
                continue;
            }
            else if(input[0] == 'q'){
                printf("Goodbye!\n");
                return 0;
            }
        }
        lexer_start(lexer, input);
        if ((err = setjmp(errjmp)) == 0) {
            scan(lexer);
            output = expr(lexer);
            if (lexer->token != EOI){
                error(ILLEGLE_INPUT);
                printf(">>> ");
                continue;
            } 
            poly_println(output);
            poly_free(output);
        }
        else {
            printf("Error: %s!\n", errmsg[err-1]);
        }
        printf(">>> ");
    }
    return 0;
}

// allocate a new single term polynomial coef*x^expo
poly_t poly_new(int expo, double coef) {
    poly_t poly = (poly_t)malloc(sizeof(poly_s));
    poly->expo = expo;
    poly->coef = coef;
    poly->next = NULL;
    return poly;
}

// free a whole polynomial
void poly_free(poly_t poly) {
    poly_t next = NULL;
    while (poly) {
        next = poly->next;
        free(poly);
        poly = next;
    }
}

// print a single term of polynomial
void poly_print(poly_t a) {
    if (a->expo) {
        printf("%.3fx", a->coef);
        if (a->expo > 1)
            printf("^%d.000", a->expo);
    } else
        printf("%.3f", a->coef);
}

// print a whole polynomial and a new line
void poly_println(poly_t a) {
    if (a) {
        poly_print(a);
        a = a->next;
        while (a) {
            if (a->coef >= 0)
                printf("+");
            poly_print(a);
            a = a->next;
        }
    }
    printf("\n");
}

// polynomials multiply with coef*x^expo
void poly_shift(poly_t a, int expo, double coef) {
    while (a) {
        a->expo += expo;
        a->coef *= coef;
        a = a->next;
    }
}

// add polynomial a and b, and append the result directly after polynomial c
void poly_concat(poly_t c, poly_t a, poly_t b) {
    while (a || b) {
        if (!b || (a && (a->expo > b->expo))) {
            c->next = poly_new(a->expo, a->coef);
            c = c->next; a = a->next;
        } else if (!a || (b && (b->expo > a->expo))) {
            c->next = poly_new(b->expo, b->coef);
            c = c->next; b = b->next;
        } else {
            c->next = poly_new(a->expo, a->coef + b->coef);
            c = c->next; a = a->next; b = b->next;
        }
    }
}

// polynomial addition
poly_t poly_add(poly_t a, poly_t b) {
    poly_t t = poly_new(0, 0.0), c = NULL;
    poly_concat(t, a, b);
    c = t->next;
    free(t);
    poly_free(a);
    poly_free(b);
    return c;
}

// polynomial negtive operation
poly_t poly_neg(poly_t a) {
    poly_shift(a, 0, -1.0);
    return a;
}

// polynomial subtraction
poly_t poly_sub(poly_t a, poly_t b) {
    return poly_add(a, poly_neg(b));
}

// polynomial multiplication
poly_t poly_mul(poly_t a, poly_t b) {
    poly_t c[2] = {NULL,NULL};
    while (a) {
        c[1] = poly_new(0, 0.0);
        poly_shift(b, a->expo, a->coef);
        poly_concat(c[1], c[0], b);
        poly_shift(b, -a->expo, 1/a->coef);
        poly_free(c[0]);
        c[0] = c[1]->next;
        free(c[1]);
        a = a->next;
    }
    poly_free(a);
    poly_free(b);
    return c[0];
}

// polynomial division
poly_t poly_div(poly_t a, poly_t b) {
    if (b->expo == 0) {
        if (b->coef == 0) {
            error(DIVIDE_BY_ZERO);
        }
        a->coef /= b->coef;
        poly_free(b);
        return a;
    }
    error(ILLEGLE_INPUT);
}

// polynomial exponentiation
poly_t poly_pow(poly_t a, poly_t b) {
    if (a->next == NULL && b->expo == 0) {
        a->expo *= b->coef;
        a->coef = pow(a->coef, b->coef);
        poly_free(b);
        return a;
    }
    error(ILLEGLE_INPUT);
}

// reset token buffer
void lexer_reset(lexer_t lexer) {
    *lexer->buf = '\0';
    lexer->len = 0;
}

// current character
char lexer_peek(lexer_t lexer) {
    return *lexer->stream;
}

// ignore current character and skip spaces
void lexer_ignore(lexer_t lexer) {
    ++lexer->stream;
    while (lexer_peek(lexer) == ' ')
        ++lexer->stream;
}

// set stream to input string and ignore heading spaces
void lexer_start(lexer_t lexer, const char* input) {
    lexer->stream = input - 1;
    lexer_reset(lexer);
    lexer_ignore(lexer);
}

// accept current character and append it to token buffer
void lexer_read(lexer_t lexer) {
    lexer->buf[lexer->len] = lexer_peek(lexer);
    lexer->buf[++lexer->len] = '\0';
    lexer_ignore(lexer);
}

// deal with digits 
void lexer_digits(lexer_t lexer) {
    while (isdigit(lexer_peek(lexer)))
        lexer_read(lexer);
    if (!lexer->len)
        error(ILLEGLE_INPUT);
}

// deal with decimal
void lexer_decimal(lexer_t lexer) {
    lexer_digits(lexer);
    if (lexer_peek(lexer) == '.') {
        lexer_read(lexer);
        lexer_digits(lexer);
    }
}

// deal with scientific notation 
double lexer_number(lexer_t lexer) {
    lexer_decimal(lexer);
    if (lexer_peek(lexer) == 'e') {
        lexer_read(lexer);
        if (lexer_peek(lexer) == '-')
            lexer_read(lexer);
        lexer_digits(lexer);
    }
    return atof(lexer->buf);
}

// deal with power 
int lexer_xpow(lexer_t lexer) {
    lexer_ignore(lexer);
    if (lexer_peek(lexer) == '^') {
        lexer_ignore(lexer);
        lexer_digits(lexer);
        return atoi(lexer->buf);
    }
    return 1;
}

// \n: EOI
// ()^+-*/: OPER
// x(^<digits>): POLY
// <number>(x(^<digits>)): POLY
void scan(lexer_t lexer) {
    lexer_reset(lexer);
    switch(lexer_peek(lexer)) {
        case '\n':
            lexer->token = EOI;
            break;
        case '(': case ')': case '^': case '+': case '-': case '*': case '/':
            lexer_read(lexer);
            lexer->token = OPER;
            break;
        case 'x':
            lexer->poly = poly_new(lexer_xpow(lexer), 1.0);
            lexer->token = POLY;
            break;
        default:
            lexer->poly = poly_new(0, lexer_number(lexer));
            if (lexer_peek(lexer) == 'x') {
                lexer_reset(lexer);
                lexer->poly->expo = lexer_xpow(lexer);
            }
            lexer->token = POLY;
            break;
    }
}

// test if current token is operater oper
int isoper(lexer_t lexer, char oper) {
    return lexer->token == OPER && *lexer->buf == oper;
}

// parentheses
poly_t atom(lexer_t lexer) {
    poly_t r = NULL;
    if (isoper(lexer, '(')) {
        scan(lexer);
        r = expr(lexer);
        if (!isoper(lexer, ')'))
            error(ILLEGLE_INPUT);
    } else if (lexer->token != POLY) {
        error(ILLEGLE_INPUT);
    } else {
        r = lexer->poly;
    }
    scan(lexer);
    return r;
}

// negative expression
poly_t neg_expr(lexer_t lexer) {
    if (isoper(lexer, '-')) {
        scan(lexer);
        return poly_neg(atom(lexer));
    }
    return atom(lexer);
}

// parse an expression (power expression)
poly_t pow_expr(lexer_t lexer) {
    poly_t r = neg_expr(lexer);
    if (isoper(lexer, '^')) {
        scan(lexer);
        r = poly_pow(r, pow_expr(lexer));
    }
    return r;
}

// parse an expression (multiplication or division expression)
poly_t mul_expr(lexer_t lexer) {
    poly_t r = pow_expr(lexer);
    while (lexer->token == OPER) {
        switch (*lexer->buf) {
            case '*':
                scan(lexer);
            case '(':
                r = poly_mul(r, pow_expr(lexer));
                break;
            case '/':
                scan(lexer);
                r = poly_div(r, pow_expr(lexer));
                break;
            default:
                return r;
        }
    }
    return r;
}

// parse an expression (addition or subtraction expression)
poly_t expr(lexer_t lexer) {
    poly_t r = mul_expr(lexer);
    while (lexer->token == OPER) {
        switch (*lexer->buf) {
            case '+':
                scan(lexer);
                r = poly_add(r, mul_expr(lexer));
                break;
            case '-':
                scan(lexer);
                r = poly_sub(r, mul_expr(lexer));
                break;
            default:
                return r;
        }
    }
    return r;
}

// Help message 
void help_function(){
    printf("The following arithmetic operations are valid:\n");
    printf("\n");
    printf("    '^' exponentiation\n");
    printf("    '+' addition\n");
    printf("    '-' subtraction\n");
    printf("    '*' multiplication\n");
    printf("    '/' division\n");
    printf("\n");
    printf("Single-variable algebraic expressions written in terms of 'x' are also valid.\n");
    printf("\n");
    printf("Enter 'h' to display this help message.\n");
    printf("Enter 'q' to quit the program.\n");
}

// Error message
void error(int err) {
    longjmp(errjmp, err); 
}
