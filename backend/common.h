#ifndef SMARTCALC_COMMON_H
#define SMARTCALC_COMMON_H

#define MEMORY_ERROR 1
#define PARSING_ERROR 2
#define STACK_EMPTY 3
#define UNOPENED_BRACKET_ERROR 4
#define UNCLOSED_BRACKET_ERROR 5
#define NOT_ENOUGH_ARGUMENTS 6
#define SYNTACTIC_ERROR 7
#define INCORRECT_X_ERROR 8

enum Lexem_type {
  NO_TYPE,
  UPLUS,
  UMINUS,
  ADD,
  SUB,
  XNUM,
  NUM,
  POW,
  MUL,
  DIV,
  MOD,
  LEFTBR,
  RIGHTBR,
  COS,
  SIN,
  TAN,
  ACOS,
  ASIN,
  ATAN,
  SQRT,
  LN,
  LOG,
  WHITESPACE,
  SIMPLE_OPERATOR_START = POW,
  COMPLEX_OPERATOR_START = UPLUS,
  UNARY_OFFSET = ADD - UPLUS,
  FUNCTION_START = COS,
  FUNCTION_END = LOG,
  OPERATOR_START = UPLUS,
  OPERATOR_END = MOD,
};

#define SIMPLE_OPERATORS                                                 \
  {                                                                      \
    "^", "*", "/", "mod", "(", ")", "cos", "sin", "tan", "acos", "asin", \
        "atan", "sqrt", "ln", "log"                                      \
  }
#define MAX_SIMPLE_OP_LENGTH 5

#define COMPLEX_OPERATORS \
  { "+", "-", }
#define MAX_COMPLEX_OP_LENGTH 5

#define MAX_ARGUMENTS 2

typedef struct {
  enum Lexem_type type;
  int priority;
  double num;
} Lexem;

#endif
