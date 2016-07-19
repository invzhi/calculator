#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.h"
#include "input.h"
#include "shuntingYardAlgorithm.h"

inline bool isOperator(char c) {
	return (c == '^' || c == '*' || c == '/' || c == '%' || c == '+' || c == '-');
}

int isConstant(char* ptr) {
	int i, number = sizeof(constant) / sizeof(Constant);
	int ret = 0;
	for(i = 0; i < number; i++) {
		int length = strlen(constant[i].str);
		if(strncmp(ptr, constant[i].str, length) == 0) {
			ret = i + 1;
			break;
		}
	}
	return ret;
}

unsigned int getPriority(char c) {
	unsigned int priority;
	switch(c) {
		case '^':
			priority = 3u;
			break;
		case '*':
		case '/':
		case '%':
			priority = 2u;
			break;
		case '+':
		case '-':
			priority = 1u;
			break;
		default:
			assert(0);
			break;
	}
	return priority;
}

charType getType(char* s, bool sign) {
	charType result;
	char *ptr, ch = *s;
	strtod(s, &ptr);
	if(ptr > s && !sign)
		result = DIGIT;
	else if(isConstant(ptr))
		result = CONSTANT;
	else if(isOperator(ch))
		result = OPERATOR;
	else if(isFunction(ch))
		result = FUNCTION;
	else if(ch == '(')
		result = LEFT_PARENTHESIS;
	else if(ch == ')')
		result = RIGHT_PARENTHESIS;
	else
		result = UNKNOWN;
	return result;
}

char* shuntingYardAlgorithm(const char *input) {
	unsigned int outputSize = 100u, stackSize = 50u, blockSize = 50u;
	unsigned int n1 = 0, n2 = 0, overNumber = 0;
	char *stack  = (char*)malloc(stackSize * sizeof(char)),
		*output = (char*)malloc(outputSize * sizeof(char));
	if(stack == NULL || output == NULL)
		assert(0);
	const char *inptr = input;
	bool digitSign = false;
	char inChar, ch;
	output[n1++] = ' ';
	while(inChar = *inptr) {
		if(n2 == stackSize) {
			stackSize += blockSize;
			stack = (char*)realloc(stack, stackSize * sizeof(char));
			if(stack == NULL)
				assert(0);
		}
		if(n1 > outputSize - 20) {
			outputSize += blockSize;
			output = (char*)realloc(output, outputSize * sizeof(char));
			if(output == NULL)
				assert(0);
		}
		char* ptr;
		bool match = false;
		int number;
		switch(getType(inptr, digitSign)) {
			case DIGIT:
				digitSign = true;
				strtod(inptr, &ptr);
				if(ptr - inptr > 7)
					overNumber++;
				while(inptr < ptr)
					output[n1++] = *inptr++;
				output[n1++] = ' ';
				break;
			case CONSTANT:
				digitSign = true;
				number = strlen(constant[isConstant(inptr) - 1].str);
				while(number--)
					output[n1++] = *inptr++;
				break;
			case OPERATOR:
				digitSign = false;
				while(n2 > 0) {
					ch = stack[n2 - 1];
					if(isOperator(ch) && getPriority(ch) >= getPriority(inChar)) {
						output[n1++] = ch;
						output[n1++] = ' ';
						n2--;
					} else
						break;
				}
				stack[n2++] = *inptr++;
				break;
			case FUNCTION:
			case LEFT_PARENTHESIS:
				digitSign = false;
				stack[n2++] = inChar;
				inptr++;
				break;
			case RIGHT_PARENTHESIS:
				digitSign = true;
				while(n2 > 0) {
					ch = stack[--n2];
					if(ch == '(') {
						match = true;
						break;
					} else {
						output[n1++] = ch;
						output[n1++] = ' ';
					}
				}
				if(!match) {
					puts("[Error]: parentheses mismatched\n");
					free(stack);
					free(output);
					return NULL;
				}
				if(n2 > 0) {
					ch = stack[n2 - 1];
					if(isFunction(ch)) {
						output[n1++] = ch;
						output[n1++] = ' ';
						n2--;
					}
				}
				inptr++;
				break;
			case UNKNOWN:
				printf("[Error]: Unknown token '%c'\n\n", inChar);
				free(stack);
				free(output);
				return NULL;
		}
	}
	while(n2 > 0) {
		ch = stack[--n2];
		if(ch == '(' || ch == ')') {
			puts("[Error]: parentheses mismatched\n");
			return NULL;
		}
		output[n1++] = ch;
		output[n1++] = ' ';
	}
	free(stack);
	output[n1 - 1] = '\0';
	output = realloc(output, (strlen(output) + 1) * sizeof(char));
	if(overNumber)
		printf("[Warning]: you may have %d figure more than 8 significant digits\n", overNumber);
	return output;
}

bool calculate(char* s, double* result) {
	Stack stack;
	stackInit(&stack);
	double num;
	char* space = s;
	char buffer[20];
	while(space = strchr(space, ' ')) {
		space++;
		sscanf(space, "%s", buffer);
		int number;
		switch(getType(buffer, false)) {
			case DIGIT:
				stackPush(&stack, atof(buffer));
				break;
			case CONSTANT:
				stackPush(&stack, constant[isConstant(buffer) - 1].num);
				break;
			case OPERATOR:
				if(stack.n < 2) {
					printf("[Error]: overmuch '%c'\n\n", *buffer);
					return false;
				}
				switch(*buffer) {
					case '^':
						num = stackPop(&stack);
						stackPush(&stack, pow(stackPop(&stack), num));
						break;
					case '*':
						stackPush(&stack, stackPop(&stack) * stackPop(&stack));
						break;
					case '/':
						if((num = stackPop(&stack)) == 0) {
							puts("[Error]: division by zero\n");
							return false;
						}
						stackPush(&stack, stackPop(&stack) / num);
						break;
					case '%':
						if((num = stackPop(&stack)) != 0)
							stackPush(&stack, fmod(stackPop(&stack), num));
						break;
					case '+':
						stackPush(&stack, stackPop(&stack) + stackPop(&stack));
						break;
					case '-':
						num = stackPop(&stack);
						stackPush(&stack, stackPop(&stack) - num);
						break;
				}
				break;
			case FUNCTION:
				num = stackPop(&stack);
				switch(*buffer) {
					case ARCSIN:
						if(num < -1 || num > 1) {
							puts("[Error]: x out of (-1 <= x <= 1) in arcsin(x)\n");
							return false;
						}
						stackPush(&stack, asin(num));
						break;
					case ARCCOS:
						if(num < -1 || num > 1) {
							puts("[Error]: x out of (-1 <= x <= 1) in arccos(x)\n");
							return false;
						}
						stackPush(&stack, acos(num));
						break;
					case ARCTAN:
						stackPush(&stack, atan(num));
						break;
					case FLOOR:
						stackPush(&stack, floor(num));
						break;
					case CEIL:
						stackPush(&stack, ceil(num));
						break;
					case ABS:
						stackPush(&stack, fabs(num));
						break;
					case EXP:
						stackPush(&stack, exp(num));
						break;
					case SIN:
						stackPush(&stack, sin(num));
						break;
					case COS:
						stackPush(&stack, cos(num));
						break;
					case TAN:
						stackPush(&stack, tan(num));
						break;
					case LN:
						if(num <= 0) {
							puts("[Error]: x out of (x > 0) in ln(x)\n");
							return false;
						}
						stackPush(&stack, log(num));
						break;
				}
				break;
		}
	}
	free(s);
	while(stack.n > 1)
		stackPush(&stack, stackPop(&stack) * stackPop(&stack));
	*result = stackPop(&stack);
	return true;
}