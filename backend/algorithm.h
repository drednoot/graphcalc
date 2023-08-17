#ifndef SMARTCALC_ALG_H
#define SMARTCALC_ALG_H

#include "common.h"
#include "stack.h"
#include <math.h>

int dijkstra_iteration(Lexem token, Lexem_stack *stack, Lexem_stack *queue);
int priority_push(Lexem token, Lexem_stack *stack, Lexem_stack *queue);
int close_bracket_push(Lexem_stack *stack, Lexem_stack *queue);
int clean_stack(Lexem_stack *stack, Lexem_stack *queue);

int is_function(Lexem lexem);
int is_operator(Lexem lexem);

int calc_operator(Lexem op, Double_stack *stack);
int get_argument_count(Lexem op);
int get_arguments(Double_stack *stack, int amount, double args[MAX_ARGUMENTS]);

#endif
