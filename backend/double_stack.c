#include "stack.h"

int create_double_stack(Double_stack *stack) {
  stack->bottom = malloc(sizeof(double));
  int rtn = 0;

  if (stack->bottom) {
    stack->size = 1;
    stack->offset = 0;
  } else {
    stack->size = 0;
    stack->offset = 0;
    rtn = MEMORY_ERROR;
  }

  return rtn;
}

int push_double(Double_stack *stack, double num) {
  int rtn = 0;
  double *temp = NULL;

  if (stack->offset == stack->size - 1) {
    temp = realloc(stack->bottom, sizeof(double) * (stack->size + 1));
    rtn = temp == NULL ? MEMORY_ERROR : 0;
    stack->size = temp == NULL ? stack->size : stack->size + 1;
    stack->bottom = temp == NULL ? stack->bottom : temp;
  }
  if (!rtn) {
    stack->bottom[stack->offset] = num;
    stack->offset++;
  }

  return rtn;
}

int pop_double(Double_stack *stack, double *num) {
  int rtn = 0;

  if (stack->offset == 0) {
    rtn = STACK_EMPTY;
  } else {
    *num = stack->bottom[stack->offset - 1];
    stack->offset--;
  }

  return rtn;
}

void remove_double_stack(Double_stack *stack) {
  if (stack->bottom) free(stack->bottom);
  stack->size = 0;
  stack->offset = 0;
  stack->bottom = NULL;
}

int flip_lexem_stack(Lexem_stack *origin, Lexem_stack *flipped) {
  int rtn = 0;
  Lexem lexem;

  while (!rtn && !pop_lexem(origin, &lexem)) {
    rtn = push_lexem(flipped, lexem);
  }

  return rtn;
}

int get_top_double(Double_stack stack, double *num) {
  int rtn = 0;

  if (stack.offset == 0) {
    rtn = STACK_EMPTY;
  } else {
    *num = stack.bottom[stack.offset - 1];
  }

  return rtn;
}
