#ifndef SMARTCALC_BACKEND_H
#define SMARTCALC_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "backend/stack.h"

int parse_string(const char *string, Lexem_stack *stack);
int in_to_rpn(Lexem_stack *in, Lexem_stack *rpn);
int calculate(Lexem_stack rpn, double *result);
int form_answer(const char *string, double *result);

#ifdef __cplusplus
}
#endif

#endif
