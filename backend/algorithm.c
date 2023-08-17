#include "algorithm.h"

#include "../backend.h"

int in_to_rpn(Lexem_stack *in, Lexem_stack *rpn) {
  int rtn = 0;
  Lexem_stack stack;
  Lexem token = {NO_TYPE, -1, 0};
  rtn = create_lexem_stack(&stack);

  while (!rtn && !pop_lexem(in, &token)) {
    rtn = dijkstra_iteration(token, &stack, rpn);
  }
  if (!rtn) rtn = clean_stack(&stack, rpn);

  remove_lexem_stack(&stack);
  return rtn;
}

int dijkstra_iteration(Lexem token, Lexem_stack *stack, Lexem_stack *queue) {
  int rtn = 0;
  int is_empty;
  Lexem stack_top = {NO_TYPE, -1, 0};

  if (token.type == NUM || token.type == XNUM) {
    rtn = push_lexem(queue, token);
  } else if (is_function(token) || token.type == LEFTBR) {
    rtn = push_lexem(stack, token);
  } else if (is_operator(token)) {
    rtn = priority_push(token, stack, queue);
  } else if (token.type == RIGHTBR) {
    rtn = close_bracket_push(stack, queue);
    is_empty = get_top_lexem(*stack, &stack_top);
    if (!rtn && !is_empty && is_function(stack_top))
      rtn = lexem_move(stack, queue);
  }

  return rtn;
}

int priority_push(Lexem token, Lexem_stack *stack, Lexem_stack *queue) {
  int rtn = 0;
  Lexem stack_top = {NO_TYPE, -1, 0};
  int is_empty = 0;

  is_empty = get_top_lexem(*stack, &stack_top);
  while (!is_empty && !rtn && (stack_top.priority >= token.priority)) {
    rtn = lexem_move(stack, queue);
    is_empty = get_top_lexem(*stack, &stack_top);
  }
  if (!rtn) rtn = push_lexem(stack, token);

  return rtn;
}

int close_bracket_push(Lexem_stack *stack, Lexem_stack *queue) {
  int rtn = 0;
  Lexem stack_top = {NO_TYPE, -1, 0};
  int is_empty = 0;

  is_empty = get_top_lexem(*stack, &stack_top);
  while (!is_empty && !rtn && (stack_top.type != LEFTBR)) {
    rtn = lexem_move(stack, queue);
    is_empty = get_top_lexem(*stack, &stack_top);
  }
  if (!rtn && is_empty) rtn = UNOPENED_BRACKET_ERROR;
  if (!is_empty) pop_lexem(stack, &stack_top);

  return rtn;
}

int clean_stack(Lexem_stack *stack, Lexem_stack *queue) {
  int rtn = 0;
  Lexem stack_top = {NO_TYPE, -1, 0};
  int is_empty = 0;

  is_empty = get_top_lexem(*stack, &stack_top);
  while (!is_empty && !rtn && (stack_top.type != LEFTBR)) {
    rtn = lexem_move(stack, queue);
    is_empty = get_top_lexem(*stack, &stack_top);
  }
  if (!is_empty && stack_top.type == LEFTBR) rtn = UNCLOSED_BRACKET_ERROR;

  return rtn;
}

int is_function(Lexem lexem) {
  return (lexem.type >= FUNCTION_START) && (lexem.type <= FUNCTION_END);
}

int is_operator(Lexem lexem) {
  return (lexem.type >= OPERATOR_START) && (lexem.type <= OPERATOR_END);
}

// calculate

int calculate(Lexem_stack rpn, double *result) {
  int rtn = 0;
  Double_stack stack;
  rtn = create_double_stack(&stack);
  double number;
  double x = *result;

  for (int i = 0; i < rpn.offset && !rtn; ++i) {
    Lexem lexem = rpn.bottom[i];
    if (lexem.type == NUM) {
      rtn = push_double(&stack, lexem.num);
    } else if (lexem.type == XNUM) {
      rtn = push_double(&stack, x);
    } else {
      rtn = calc_operator(lexem, &stack);
    }
  }

  if (!rtn) rtn = pop_double(&stack, result);
  if (!rtn) {
    rtn = get_top_double(stack, &number);
    if (!rtn) {
      rtn = SYNTACTIC_ERROR;
    } else {
      rtn = 0;
    }
  }
  remove_double_stack(&stack);
  return rtn;
}

int calc_operator(Lexem op, Double_stack *stack) {
  int rtn = 0;
  double args[MAX_ARGUMENTS] = {0};
  rtn = get_arguments(stack, get_argument_count(op), args);
  enum Lexem_type t = op.type;
  double result = 0;

  if (rtn)
    ;
  else if (t == UPLUS) {
    result = args[0];
  } else if (t == UMINUS) {
    result = -args[0];
  } else if (t == ADD) {
    result = args[0] + args[1];
  } else if (t == SUB) {
    result = args[0] - args[1];
  } else if (t == POW) {
    result = pow(args[0], args[1]);
  } else if (t == MUL) {
    result = args[0] * args[1];
  } else if (t == DIV) {
    result = args[0] / args[1];
  } else if (t == MOD) {
    result = fmod(args[0], args[1]);
  } else if (t == COS) {
    result = cos(args[0]);
  } else if (t == SIN) {
    result = sin(args[0]);
  } else if (t == TAN) {
    result = tan(args[0]);
  } else if (t == ACOS) {
    result = acos(args[0]);
  } else if (t == ASIN) {
    result = asin(args[0]);
  } else if (t == ATAN) {
    result = atan(args[0]);
  } else if (t == SQRT) {
    result = sqrt(args[0]);
  } else if (t == LN) {
    result = log(args[0]);
  } else if (t == LOG) {
    result = log(args[0]) / log(10);
  }
  if (!rtn) rtn = push_double(stack, result);

  return rtn;
}

int get_argument_count(Lexem op) {
  int count = 0;
  enum Lexem_type t = op.type;
  if (t == UPLUS || t == UMINUS || t == COS || t == SIN || t == TAN ||
      t == ACOS || t == ASIN || t == ATAN || t == SQRT || t == LN || t == LOG) {
    count = 1;
  } else if (t == ADD || t == SUB || t == POW || t == MUL || t == DIV ||
             t == MOD) {
    count = 2;
  }

  return count;
}

int get_arguments(Double_stack *stack, int amount, double args[MAX_ARGUMENTS]) {
  int rtn = 0;

  for (int i = amount - 1; i >= 0 && !rtn; --i) {
    rtn = pop_double(stack, &args[i]);
  }
  if (rtn) rtn = NOT_ENOUGH_ARGUMENTS;

  return rtn;
}

int form_answer(const char *string, double *result) {
  Lexem_stack in, rpn;
  int incrtd = 0, rpncrtd = 0;
  int rtn = 0;

  rtn = create_lexem_stack(&in);
  if (!rtn) {
    incrtd = 1;
    rtn = create_lexem_stack(&rpn);
  }
  if (!rtn) rpncrtd = 1;

  if (!rtn) rtn = parse_string(string, &in);

  if (!rtn) rtn = in_to_rpn(&in, &rpn);

  if (!rtn) rtn = calculate(rpn, result);

  if (incrtd) remove_lexem_stack(&in);
  if (rpncrtd) remove_lexem_stack(&rpn);

  return rtn;
}
