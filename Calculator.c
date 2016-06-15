#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define bool  char
#define true  1
#define false 0

#define DEBUG

typedef enum {
	DIGIT,
	OPERATOR,
	FUNCTION,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	UNKNOWN,
} charType;

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

inline bool isInput(char c);
inline bool isOperator(char c);
inline bool isFunction(char c);
charType getType(char* c, bool sign);
unsigned int getPriority(char c);
void interfaceInit();
bool inputInit(char* s);
bool functionInit(char* s);
bool commandCheck(char* s);
void replaceNumber(char* a, double insert, const char* b);
bool replaceFuntion(char* s, function c, int functionLength);
bool calculate(char* s, double* result);
char* getInfixNotation();
char* shuntingYardAlgorithm(const char *input);

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
	return (c >= ' ' || c <= '~');
}

inline bool isOperator(char c) {
	return (c == '^' || c == '*' || c == '/' || c == '%' || c == '+' || c == '-');
}

inline bool isFunction(char c) {
	return (c >= 'A' && c < FUNCTION_END);
}

void interfaceInit() {
	system("color F0");
	system("title --------------------Welcome to use the calculator program--------------------");
	printf("Please enter the expression or command with English characters:\t\t");
	puts  ("Or you can enter \"help\" to know more.\n");
}

char* getInfixNotation() {
	unsigned int size = 100u, blockSize = 50u;
	char* buffer = (char*)malloc(size * sizeof(char));
	char c = EOF;
	int  n = 0;
	while(buffer != NULL && (c = getchar()) != EOF && c != '\n') {
		if(isspace(c))
			continue;
		if(!isInput(c)) {
			puts("you can only enter characters with English\n");
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
	char  ch;
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
				digitSign = true;
				stack[n2++] = inChar;
				inptr++;
				break;
			case RIGHT_PARENTHESIS:
				digitSign = false;
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
	return output;
}

bool calculate(char* s, double* result) {
	unsigned int size = 50u, blockSize = 25u;
	unsigned int n = 0;
	double* number = (double*)malloc(size * sizeof(double));
	if(number == NULL)
		assert(0);
	char* space = s;
	char buffer[20];
	while(space = strchr(space, ' ')) {
		if(n == size) {
			size += blockSize;
			number = (double*)realloc(number, size * sizeof(double));
			if(number == NULL)
				assert(0);
		}
		space++;
		sscanf(space, "%s", buffer);
		switch(getType(buffer, false)) {
			case DIGIT:
				number[n++] = atof(buffer);
				break;
			case OPERATOR:
				if(n < 2) {
					printf("[Error]: overmuch '%c'\n\n", *buffer);
					return false;
				}
				switch(*buffer) {
					case '^':
						number[n - 2] = pow(number[n - 2], number[n - 1]);
						break;
					case '*':
						number[n - 2] *= number[n - 1];
						break;
					case '/':
						if(number[n - 1] == 0) {
							puts("[Error]: division by zero\n");
							return false;
						}
						number[n - 2] /= number[n - 1];
						break;
					case '%':
						if(number[n - 1] != 0)
							number[n - 2] = fmod(number[n - 2], number[n - 1]);
						break;
					case '+':
						number[n - 2] += number[n - 1];
						break;
					case '-':
						number[n - 2] -= number[n - 1];
						break;
				}
				n--;
				break;
			case FUNCTION:
				switch(*buffer) {
					case ARCSIN:
						if(number[n - 1] >= -1 && number[n - 1] <= 1)
							number[n - 1] = asin(number[n - 1]);
						else {
							puts("[Error]: x out of (-1 <= x <= 1) in arcsin(x)\n");
							return false;
						}
						break;
					case ARCCOS:
						if(number[n - 1] >= -1 && number[n - 1] <= 1)
							number[n - 1] = acos(number[n - 1]);
						else {
							puts("[Error]: x out of (-1 <= x <= 1) in arccos(x)\n");
							return false;
						}
						break;
					case ARCTAN:
						number[n - 1] = atan(number[n - 1]);
						break;
					case FLOOR:
						number[n - 1] = floor(number[n - 1]);
						break;
					case CEIL:
						number[n - 1] = ceil(number[n - 1]);
						break;
					case ABS:
						number[n - 1] = fabs(number[n - 1]);
						break;
					case EXP:
						number[n - 1] = exp(number[n - 1]);
						break;
					case SIN:
						number[n - 1] = sin(number[n - 1]);
						break;
					case COS:
						number[n - 1] = cos(number[n - 1]);
						break;
					case TAN:
						number[n - 1] = tan(number[n - 1]);
						break;
					case LN:
						if(number[n - 1] > 0)
							number[n - 1] = log(number[n - 1]);
						else {
							puts("[Error]: x out of (x > 0) in ln(x)\n");
							return false;
						}
						break;
				}
				break;
		}
	}
	free(s);
	while(n > 1) {
		number[n - 2] *= number[n - 1];
		n--;
	}
	*result = *number;
	free(number);
	return true;
}
