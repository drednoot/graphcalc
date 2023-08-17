#include "stack.h"

int create_lexem_stack(Lexem_stack *stack) {
	stack->bottom = malloc(sizeof(Lexem));
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

int push_lexem(Lexem_stack *stack, Lexem lexem) {
	int rtn = 0;
	Lexem *temp = NULL;

	if (stack->offset == stack->size - 1) {
		temp = realloc(stack->bottom, sizeof(Lexem) * (stack->size + 1));
		rtn = temp == NULL ? MEMORY_ERROR : 0;
		stack->size = temp == NULL ? stack->size : stack->size + 1;
		stack->bottom = temp == NULL ? stack->bottom : temp;
	}
	if (!rtn) {
		stack->bottom[stack->offset] = lexem;
		stack->offset++;
	}

	return rtn;
}

int pop_lexem(Lexem_stack *stack, Lexem *lexem) {
	int rtn = 0;

	if (stack->offset == 0) {
		rtn = STACK_EMPTY;
	} else {
		*lexem = stack->bottom[stack->offset - 1];
		stack->offset--;
	}

	return rtn;
}

void remove_lexem_stack(Lexem_stack *stack) {
	if (stack->bottom) free(stack->bottom);
	stack->size = 0;
	stack->offset = 0;
	stack->bottom = NULL;
}

int get_top_lexem(Lexem_stack stack, Lexem *lexem) {
	int rtn = 0;

	if (stack.offset == 0) {
		rtn = STACK_EMPTY;
	} else {
		*lexem = stack.bottom[stack.offset - 1];
	}

	return rtn;
}

int lexem_move(Lexem_stack *origin, Lexem_stack *destination) {
	int rtn = 0;
	Lexem temp;

	rtn = pop_lexem(origin, &temp);
	if (!rtn) rtn = push_lexem(destination, temp);

	return rtn;
}
