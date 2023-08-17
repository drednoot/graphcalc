#ifndef SMARTCALC_PARSER_H
#define SMARTCALC_PARSER_H

#include <math.h>
#include <string.h>

#include "common.h"

int parse_lexem(const char **cur, Lexem *lexem, Lexem previous_lexem);
void set_priority(Lexem *lexem);

int try_number(const char **cur, Lexem *lexem);
int try_x(const char **cur, Lexem *lexem);
int try_integral(const char **cur, double *integral);
int try_quotient(const char **cur, double *quotient);
int try_exponent(const char **cur, int *exp);
int is_digit(char sym);

int try_simple_operator(const char **cur, Lexem *lexem);
int try_complex_operator(const char **cur, Lexem *lexem, Lexem prev);

int try_whitespace(const char **cur, Lexem *lexem);

#endif
