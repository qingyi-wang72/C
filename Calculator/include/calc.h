#ifndef _CALC_H_
#define _CALC_H_

// Error message
#define BUFFER_SIZE 1024
#define DIVIDE_BY_ZERO 1
#define ILLEGLE_INPUT 2

// Construct linked list
// each node represent a single term - coef*x^expo
// without variable x - coef*x^0
typedef struct poly_s* poly_t;
typedef struct poly_s {
    int expo;
    double coef;
    poly_t next;
} poly_s, *poly_t;

// Recursive descent 
// OPER: operator
// POLY: polynomial
// EOI: end of input
typedef struct lexer_s {
    const char* stream;
    char buf[BUFFER_SIZE];
    char len;
    enum { OPER, POLY, EOI } token; 
    poly_t poly;
} lexer_s, *lexer_t;

jmp_buf errjmp;
const char* errmsg[2] = {"Divide by zero", "Illegal input"};

/* Function Prototypes */
poly_t poly_new(int expo, double coef);
void poly_free(poly_t poly);
void poly_print(poly_t a);
void poly_println(poly_t a);
void poly_shift(poly_t a, int expo, double coef);
void poly_concat(poly_t c, poly_t a, poly_t b);
poly_t poly_add(poly_t a, poly_t b);
poly_t poly_neg(poly_t a);
poly_t poly_sub(poly_t a, poly_t b);
poly_t poly_mul(poly_t a, poly_t b);
poly_t poly_div(poly_t a, poly_t b);
poly_t poly_pow(poly_t a, poly_t b);

void lexer_reset(lexer_t lexer);
char lexer_peek(lexer_t lexer);
void lexer_ignore(lexer_t lexer);
void lexer_start(lexer_t lexer, const char* input);
void lexer_read(lexer_t lexer);
void lexer_digits(lexer_t lexer);
void lexer_decimal(lexer_t lexer);
double lexer_number(lexer_t lexer);
int lexer_xpow(lexer_t lexer);

void scan(lexer_t lexer);
int isoper(lexer_t lexer, char oper);
poly_t atom(lexer_t lexer);
poly_t neg_expr(lexer_t lexer);
poly_t pow_expr(lexer_t lexer);
poly_t mul_expr(lexer_t lexer);
poly_t expr(lexer_t lexer);
void help_function();
void error(int err);

#endif 
