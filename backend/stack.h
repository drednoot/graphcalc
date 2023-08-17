#ifndef SMARTCALC_STACK_H
#define SMARTCALC_STACK_H

#include <stdlib.h>

#include "common.h"

typedef struct {
  Lexem *bottom;
  int size;
  int offset;
} Lexem_stack;

typedef struct {
  double *bottom;
  int size;
  int offset;
} Double_stack;

// lexem
int create_lexem_stack(Lexem_stack *stack);
int push_lexem(Lexem_stack *stack, Lexem lexem);
int pop_lexem(Lexem_stack *stack, Lexem *lexem);
void remove_lexem_stack(Lexem_stack *stack);
int flip_lexem_stack(Lexem_stack *origin, Lexem_stack *flipped);
int get_top_lexem(Lexem_stack stack, Lexem *lexem);
int lexem_move(Lexem_stack *origin, Lexem_stack *destination);

// double
int create_double_stack(Double_stack *stack);
int push_double(Double_stack *stack, double num);
int pop_double(Double_stack *stack, double *num);
void remove_double_stack(Double_stack *stack);
int get_top_double(Double_stack stack, double *num);

#endif
