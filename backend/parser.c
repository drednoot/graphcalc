#include "parser.h"

#include "stack.h"

int parse_string(const char *string, Lexem_stack *stack) {
  int rtn = 0;
  Lexem lexem = {NO_TYPE, -1, 0};
  Lexem previous_lexem = {NO_TYPE, -1, 0};
  const char *cur = string;
  Lexem_stack reverse_stack;
  rtn = create_lexem_stack(&reverse_stack);

  while (*cur && !rtn && !(rtn = parse_lexem(&cur, &lexem, previous_lexem))) {
    if (lexem.type == WHITESPACE) {
    } else if (lexem.type != NO_TYPE) {
      set_priority(&lexem);
      rtn = push_lexem(&reverse_stack, lexem);
      previous_lexem = lexem;
    }
  }

  if (!rtn) rtn = flip_lexem_stack(&reverse_stack, stack);
  remove_lexem_stack(&reverse_stack);

  return rtn;
}

int parse_lexem(const char **cur, Lexem *lexem, Lexem prev) {
  int rtn = 0;
  int found = 0;
  *lexem = (Lexem){NO_TYPE, -1, 0};

  found = try_whitespace(cur, lexem);
  if (!found) found = try_number(cur, lexem);
  if (!found) found = try_x(cur, lexem);
  if (!found) found = try_complex_operator(cur, lexem, prev);
  if (!found) found = try_simple_operator(cur, lexem);
  if (!found) rtn = PARSING_ERROR;

  return rtn;
}

void set_priority(Lexem *lexem) {
  if (lexem->priority == -1) {
    if (lexem->type == POW) {
      lexem->priority = 4;
    } else if (lexem->type == UMINUS || lexem->type == UPLUS) {
      lexem->priority = 3;
    } else if (lexem->type == MUL || lexem->type == DIV || lexem->type == MOD) {
      lexem->priority = 2;
    } else if (lexem->type == ADD || lexem->type == SUB) {
      lexem->priority = 1;
    } else {
      lexem->priority = 0;
    }
  }
}

// number

int try_number(const char **cur, Lexem *lexem) {
  int found = 0;
  const char *new_cur = *cur;
  Lexem new_lexem = {NUM, -1, 0};
  double integral = 0, quotient = 0;
  int exp = 0;

  found = try_integral(&new_cur, &integral);
  if (found) try_quotient(&new_cur, &quotient);
  if (found) try_exponent(&new_cur, &exp);

  if (found) {
    *cur = new_cur;
    new_lexem.num = (integral + quotient) * pow(10, exp);
    *lexem = new_lexem;
  }
  return found;
}

int try_integral(const char **cur, double *integral) {
  int found = is_digit(**cur);
  *integral = 0;

  while (is_digit(**cur)) {
    *integral *= 10;
    *integral += **cur - 0x30;
    (*cur)++;
  }

  return found;
}

int try_quotient(const char **cur, double *quotient) {
  int found = **cur == '.';
  const char *new_cur = *cur;
  *quotient = 0;
  int length = 0;

  if (found) new_cur++;
  while (is_digit(*new_cur)) {
    *quotient *= 10;
    *quotient += *new_cur - 0x30;
    new_cur++;
    length++;
  }

  if (found) {
    *cur = new_cur;
    if (length != 0) {
      *quotient = *quotient / pow(10, length);
    }
  }
  return found;
}

int try_exponent(const char **cur, int *exp) {
  int found_e = **cur == 'e' || **cur == 'E';
  int found_sign = 0;
  int sign = 1;
  const char *new_cur = *cur;
  *exp = 0;

  if (found_e) new_cur++;
  if (*new_cur == '+') {
    found_sign = 1;
  } else if (*new_cur == '-') {
    sign = -1;
    found_sign = 1;
  }
  if (found_e && found_sign && is_digit(new_cur[1])) {
    new_cur++;
  }
  while (is_digit(*new_cur)) {
    *exp *= 10;
    *exp += *new_cur - 0x30;
    new_cur++;
  }

  if (found_e) {
    *cur = new_cur;
    *exp *= sign;
  }
  return found_e;
}

int is_digit(char sym) { return (sym >= '0') && (sym <= '9'); }

// operators

int try_simple_operator(const char **cur, Lexem *lexem) {
  int found = 0;
  const char simple_operators[][MAX_SIMPLE_OP_LENGTH] = SIMPLE_OPERATORS;
  int arr_len = sizeof(simple_operators) / sizeof(simple_operators[0]);

  for (int i = 0; i < arr_len && !found; ++i) {
    int len = strlen(simple_operators[i]);
    if (strncmp(*cur, simple_operators[i], len) == 0) {
      found = 1;
      lexem->type = i + SIMPLE_OPERATOR_START;
      *cur += len;
    }
  }

  return found;
}

int try_complex_operator(const char **cur, Lexem *lexem, Lexem prev) {
  int found = 0;
  const char complex_operators[][MAX_COMPLEX_OP_LENGTH] = COMPLEX_OPERATORS;
  int arr_len = sizeof(complex_operators) / sizeof(complex_operators[0]);

  for (int i = 0; i < arr_len && !found; ++i) {
    int len = strlen(complex_operators[i]);
    if (strncmp(*cur, complex_operators[i], len) == 0) {
      found = 1;
      enum Lexem_type op = i + COMPLEX_OPERATOR_START;
      if (prev.type == RIGHTBR || prev.type == NUM || prev.type == XNUM) {
        op += UNARY_OFFSET;
      }
      lexem->type = op;
      *cur += len;
    }
  }

  return found;
}

// whitespace

int try_whitespace(const char **cur, Lexem *lexem) {
  int found = 0;

  if (**cur == ' ' || **cur == '\n' || **cur == '\t') {
    (*cur)++;
    found = 1;
    lexem->type = WHITESPACE;
  }

  return found;
}

int try_x(const char **cur, Lexem *lexem) {
  int found = 0;

  if (**cur == 'x') {
    (*cur)++;
    found = 1;
    lexem->type = XNUM;
  }

  return found;
}
