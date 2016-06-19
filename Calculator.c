#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/*#define DEBUG*/

/*increase boolean*/
#define bool  char
#define true  1
#define false 0

typedef enum {
	DIGIT,
	OPERATOR,
	FUNCTION,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	UNKNOWN,
} charType;

/*replace mathematical function in uppercase*/
typedef enum {
	ARCSIN = 'A',
	ARCCOS,
	ARCTAN,
	FLOOR,
	CEIL,
	ABS,
	EXP,
	SIN,
	COS,
	TAN,
	LN,
	FUNCTION_END,
} function;

typedef struct {
	double* stack;
	unsigned int n;
	unsigned int size;
} Stack;

/*stack*/
void stackInit(Stack* s);
void stackPush(Stack* s, double value);
double stackPop(Stack* s);
/*inline*/
inline bool isInput(char c);
inline bool isOperator(char c);
inline bool isFunction(char c);
/*user interface*/
void interfaceInit(void);
/*input infix notation*/
char* getInfixNotation(void);
bool inputInit(char* s);
void replaceNumber(char* a, double insert, const char* b);
/*Command line operation*/
bool commandCheck(char* s);
/*shunting yard algorithm*/
bool functionInit(char* s);
bool replaceFuntion(char* s, function c, int functionLength);
charType getType(char* c, bool sign);
unsigned int getPriority(char c);
char* shuntingYardAlgorithm(const char *input);
/*calculate reverse polish notation*/
bool calculate(char* s, double* result);

double Ans = 0;

int main() {
	interfaceInit();
	double result;
	char *input = NULL, *reversePolishNotation = NULL;
	while(strcmp(input = getInfixNotation(), "quit") != 0) {
		#ifdef DEBUG
		printf("input:%s\n", input);
		#endif
		if(!commandCheck(input) && (reversePolishNotation = shuntingYardAlgorithm(input))) {
			#ifdef DEBUG
			printf("reversePolishNotation:%s\n", reversePolishNotation + 1);
			#endif
			if(calculate(reversePolishNotation, &result))
				printf("= %g\n\n", Ans = result);
		}
		free(input);
	}
	free(input);
	return EXIT_SUCCESS;
}

inline bool isInput(char c) {
	return (c >= ' ' && c <= '~');
}

inline bool isOperator(char c) {
	return (c == '^' || c == '*' || c == '/' || c == '%' || c == '+' || c == '-');
}

inline bool isFunction(char c) {
	return (c >= 'A' && c < FUNCTION_END);
}

void interfaceInit(void) {
	system("color F0");
	system("title --------------------Welcome to use the calculator program--------------------");
	printf("Please enter the expression or command with English characters:\t\t");
	puts  ("Or you can enter \"help\" to know more.\n");
}

char* getInfixNotation(void) {
	unsigned int size = 100u, blockSize = 50u;
	char* buffer = (char*)malloc(size * sizeof(char));
	char c = EOF;
	int  n = 0;
	while(buffer != NULL && (c = getchar()) != EOF && c != '\n') {
		if(isspace(c))
			continue;
		if(!isInput(c)) {
			puts("[Error]: you can only enter characters with English\n");
			free(buffer);
			fflush(stdin);
			return getInfixNotation();
		}
		if(isupper(c))
			c = tolower(c);
		buffer[n++] = c;
		if(n == size) {
			size += blockSize;
			buffer = (char*)realloc(buffer, size * sizeof(char));
		}
	}
	if(buffer == NULL || n == 0) {
		free(buffer);
		buffer = getInfixNotation();
	} else {
		buffer[n] = '\0';
		buffer = (char*)realloc(buffer, (strlen(buffer) + 100) * sizeof(char));
		if(!inputInit(buffer)) {
			free(buffer);
			buffer = getInfixNotation();
		}
	}
	return buffer;
}

bool inputInit(char* s) {
	char* ptr = s;
	if(ptr = strstr(s, "()" )) {
		puts("[Error]: empty between '(' and ')'\n");
		return false;
	}
	while(ptr = strstr(s, "pi"))
		replaceNumber(ptr, acos(-1.0), ptr + 2);
	while(ptr = strstr(s, "ans"))
		replaceNumber(ptr, Ans, ptr + 3);
	if(functionInit(s))
		return true;
	else
		return false;
}

void replaceNumber(char* a, double insert, const char* b) {
	char* b_ = (char*)malloc((strlen(b) + 1) * sizeof(char));
	if(b_ == NULL)
		assert(0);
	strcpy(b_, b);
	sprintf(a, "%lf%s", insert, b_);
	free(b_);
}

bool commandCheck(char* s)	{
	bool result = false;
	if(strcmp(s, "cls") == 0) {
		result = true;
		system("cls");
		printf("Please enter the expression or command with English characters:\t\t");
		puts  ("Or you can enter \"help\" to know more.\n");
	} else if(strcmp(s, "color") == 0) {
		result = true;
		static bool color = 0; /*Black->1 White->0*/
		color = !color;
		printf("\n");
		system(color ? "color 0F" : "color F0");
	} else if(strcmp(s, "help") == 0) {
		result = true;
		puts("\n============================= HELP =============================");
		puts("|                                                              |");
		puts("|-------------------------- Commands --------------------------|");
		puts("|                                                              |");
		puts("|         \"cls\"   ----------clear screen                       |");
		puts("|         \"quit\"  ----------quit the program                   |");
		puts("|         \"color\" ----------change the color of the console    |");
		puts("|                                                              |");
		puts("|-------------------------- Constant --------------------------|");
		puts("|                                                              |");
		puts("|                           PI   Ans                           |");
		puts("|                                                              |");
		puts("|-------------------------- Operators -------------------------|");
		puts("|                                                              |");
		puts("|     Exponent       ^                                         |");
		puts("|     Multiplication *   Division    /      Modulus     %      |");
		puts("|     Addition       +   Subtraction -                         |");
		puts("|                                                              |");
		puts("|-------------------------- Functions -------------------------|");
		puts("|                                                              |");
		puts("|  abs(x), ln (x), exp(x), floor (x), ceil  (x)                |");
		puts("|  sin(x), cos(x), tan(x), arcsin(x), arccos(x), arctan(x).    |");
		puts("|                                                              |");
		puts("================================================================");
		puts("\nPlease enter the expression or command with English characters:\n");
	}
	return result;
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
	}
	return priority;
}

charType getType(char* s, bool sign) {
	charType result;
	char *ptr, ch = *s;
	strtod(s, &ptr);
	if(ptr > s && !sign)
		result = DIGIT;
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

bool functionInit(char* s) {
	char* ptr;
	while(ptr = strstr(s, "arcsin"))
		if(!replaceFuntion(ptr, ARCSIN, 6))
			return false;
	while(ptr = strstr(s, "arccos"))
		if(!replaceFuntion(ptr, ARCCOS, 6))
			return false;
	while(ptr = strstr(s, "arctan"))
		if(!replaceFuntion(ptr, ARCTAN, 6))
			return false;
	while(ptr = strstr(s, "floor"))
		if(!replaceFuntion(ptr, FLOOR, 5))
			return false;
	while(ptr = strstr(s, "ceil"))
		if(!replaceFuntion(ptr, CEIL, 4))
			return false;
	while(ptr = strstr(s, "abs"))
		if(!replaceFuntion(ptr, ABS, 3))
			return false;
	while(ptr = strstr(s, "exp"))
		if(!replaceFuntion(ptr, EXP, 3))
			return false;
	while(ptr = strstr(s, "sin"))
		if(!replaceFuntion(ptr, SIN, 3))
			return false;
	while(ptr = strstr(s, "cos"))
		if(!replaceFuntion(ptr, COS, 3))
			return false;
	while(ptr = strstr(s, "tan"))
		if(!replaceFuntion(ptr, TAN, 3))
			return false;
	while(ptr = strstr(s, "ln"))
		if(!replaceFuntion(ptr, LN, 2))
			return false;
	return true;
}

bool replaceFuntion(char* s, function c, int functionLength) {
	bool ret;
	char ch;
	if(*(s + functionLength) == '(') {
		*s++ = c;
		while(ch = *(s + functionLength - 1))
			*s++ = ch;
		*s = ch;
		ret = true;
	} else {
		printf("[Error]: no '(' after function \"%.*s\"\n\n", functionLength, s);
		ret = false;
	}
	return ret;
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
		switch(getType(inptr, digitSign)) {
			case DIGIT:
				digitSign = true;
				strtod(inptr, &ptr);
				if(ptr - inptr > 8)
					overNumber++;
				while(inptr < ptr)
					output[n1++] = *inptr++;
				output[n1++] = ' ';
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
		switch(getType(buffer, false)) {
			case DIGIT:
				stackPush(&stack, atof(buffer));
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
