#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

void stackInit(Stack* s) {
	s->size = 50u;
	s->n = 0u;
	s->stack = (double*)malloc(sizeof(double) * s->size);
	if(s->stack == NULL)
		assert(0);
}

void stackPush(Stack* s, double value) {
	if(s->n == s->size) {
		s->size += 25u;
		s->stack = (double*)realloc(s->stack, sizeof(double) * s->size);
	}
	s->stack[s->n++] = value;
}

double stackPop(Stack* s) {
	double value = 0;
	if(s->n > 0)
		value = s->stack[--s->n];
	else
		puts("[Error]: stack is empty and can't pop\n");
	return value;
}
