#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define bool  char
#define true  1
#define false 0

enum errorType {
	OVERMUCH,
	NO_NUMBER,
	NOT_DECLARED,
	MALLOC_ERROR,
	ARC_OUT_OF_BOUNDS,
	LN_OUT_OF_BOUNDS,
};

void   interfaceInit();
void   expressionInit();
void   commandCheck();
char*  getExpression();
bool   isOperator(char c);
void   error(int n, char c);
void   replace(char* a, double num, const char* b);
double calculate(char* s);
double mathFunction(char* leftBracket, double num);

bool   again;
int    mathFunctionLength;
char*  str = NULL;
double Ans = 0;

int main()
{
	interfaceInit();
	while(true) {
		char  *leftBracket, *rightBracket;
		double result;
		do {
			again = false;
			free(str);
			str = getExpression();
			expressionInit();
			commandCheck();
		} while(again);
		while(rightBracket = strchr(str, ')')) {
			mathFunctionLength = 0;
			*rightBracket = '\0';
			leftBracket = strrchr(str,'(');
			result = calculate(leftBracket + 1);
			if(!again && leftBracket > str + 1 && islower(*(leftBracket - 1)))
				result = mathFunction(leftBracket, result);
			if(again)
				break;
			replace(leftBracket - mathFunctionLength, result, rightBracket + 1);
		}
		if(again)
			continue;
		result = calculate(str);
		if(!again)
			printf("= %g\n\n", Ans = result);
	}
}

void interfaceInit() {
	system("color F0");
	system("title --------------------Welcome to use the calculator program--------------------");
	printf("Please enter the expression or command with English characters:\t\t");
	puts  ("Or you can enter \"help\" to know more.\n");
}

void commandCheck()	{
	if(strcmp(str, "quit") == 0)
		exit(EXIT_SUCCESS);
	else if(strcmp(str, "cls") == 0) {
		again = true;
		system("cls");
		printf("Please enter the expression or command with English characters:\t\t");
		puts  ("Or you can enter \"help\" to know more.\n");
	} else if(strcmp(str, "color") == 0) {
		again = true;
		static bool color = 0; /*Black->1 White->0*/
		color = !color;
		printf("\n");
		system(color ? "color 0F" : "color F0");
	} else if(strcmp(str, "help") == 0) {
		again = true;
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
}

void expressionInit() {
	char* left;
	int n, bracket;
	n = bracket = 0;
	do {
		if(str[n] == '(')
			bracket++;
		else if(str[n] == ')') {
			if(bracket == 0) {
				error(OVERMUCH, ')');
				return;
			} else
				bracket--;
		}
	} while(str[n++]);
	if(bracket) {
		error(OVERMUCH, '(');
		return;
	}
	while(left = strstr(str, "()" ))
		error(NO_NUMBER, ')');
	while(left = strstr(str, "pi" ))
		replace(left, acos(-1.0), left + 2);
	while(left = strstr(str, "ans"))
		replace(left, Ans, left + 3);
}

char* getExpression() {
	unsigned int size = 100u, blockSize = 50u;
	char* buffer = (char*)malloc(size);
	char c = EOF;
	int  n = 0;
	while(buffer != NULL && (c = getchar()) != EOF && c != '\n') {
		if(isspace(c))
			continue;
		if(isupper(c))
			c = tolower(c);
		buffer[n++] = c;
		if(n == size) {
			size += blockSize;
			buffer = (char*)realloc(buffer, size);
		}
	}
	if(buffer == NULL)
		error(MALLOC_ERROR, 0);
	else if(n == 0) {
		free(buffer);
		buffer = getExpression();
	} else
		buffer[n] = '\0';
	return buffer;
}

bool isOperator(char c) {
	bool result = false;
	if(c == '^' || c == '*' || c == '/' || c == '%' || c == '+' || c == '-')
		result = true;
	return result;
}

void replace(char* a, double num, const char* b) {
	bool left = false, right = false;
	if(a > str && !isOperator(*(a-1)) && *(a-1) != '(')
		left  = true;
	if(*b != '\0' && !isOperator(*b) && *b != ')')
		right = true;
	char* b_ = (char*)malloc(strlen(b) + 1);
	if(b_ == NULL)
		error(MALLOC_ERROR, 0);
	strcpy(b_, b);
	sprintf(a, "%s%lf%s%s", left ? "*" : "", num, right ? "*" : "", b_);
	free(b_);
}

void error(int n, char c) {
	again = true;
	switch(n) {
		case OVERMUCH:
			printf("[Error]: too many '%c'\n\n", c);
			break;
		case NO_NUMBER:
			printf("[Error]: no number before '%c'\n\n", c);
			break;
		case NOT_DECLARED:
			printf("[Error]: something was not declared around '%c'\n\n", c);
			break;
		case MALLOC_ERROR:
			printf("[Error]: application memory failed\n\n");
			assert(0);
			break;
		case ARC_OUT_OF_BOUNDS:
			printf("[Error]: x<-1 or x>1  in arcsin(x) or arccos(x)\n\n");
			break;
		case LN_OUT_OF_BOUNDS:
			printf("[Error]: x<0  in ln(x)\n\n");
			break;
	}
}

double calculate(char* s) {
	char  *power, *endptr1, *endptr2;
	double cache, result = 0;
	char* s_ = (char*)malloc(strlen(s) + 100);
	if(s_ == NULL)
		error(MALLOC_ERROR, 0);
	strcpy(s_, s);
	while(power = strchr(s_, '^')) { /*Exponent ^ */
		if(power == s_) {
			error(NO_NUMBER, '^');
			return 0;
		}
		char* lastNumber = power;
		while(lastNumber > s_) {
			lastNumber--;
			if(isOperator(*lastNumber)) {
				lastNumber++;
				break;
			}
		}
		double num1 = strtod(lastNumber, &endptr1), 
			   num2 = strtod(power  + 1, &endptr2);
		if(*endptr1 != '^' || *endptr2 != '\0' && !isOperator(*endptr2)) {
			error(NOT_DECLARED, '^');
			return 0;
		}
		replace(lastNumber, pow(num1, num2), endptr2);
	}
	cache = strtod(s_, &endptr1);
	while(*endptr1) {
		if(s_ == endptr1) {
			error(isOperator(*s_) ? NO_NUMBER : NOT_DECLARED, *s_);
			return 0;
		}
		double next = strtod(endptr1 + 1, &endptr2);
		if(endptr1 + 1 == endptr2) {
			error(*endptr1 == *endptr2 ? OVERMUCH : NOT_DECLARED, *endptr1);
			return 0;
		}
		switch(*endptr1) {
			case '*':/*Multiplication * */
				cache *= next;
				break;
			case '/':/*Division       / */
				cache /= next;
				break;
			case '%':/*Modulus        % */
				if(next != 0)
					cache = fmod(cache, next);
				break;
			case '+':/*Addition       + */
				result += cache;
				cache = next;
				break;
			case '-':/*Subtraction    - */
				result += cache;
				cache = next * -1;
				break;
			default:
				error(*endptr1 == *(endptr1 - 1) ? OVERMUCH : NOT_DECLARED, *endptr1);
				return 0;
		}
		endptr1 = endptr2;
	}
	result += cache;
	free(s_);
	return result;
}

double mathFunction(char* leftBracket, double num) {
	if(leftBracket > str + 5) {
		if(strncmp(leftBracket - 6, "arcsin", 6) == 0) {
			mathFunctionLength = 6;
			if(num >= -1 && num <= 1)
				num = asin(num);
			else
				error(ARC_OUT_OF_BOUNDS, 0);
		} else if(strncmp(leftBracket - 6, "arccos", 6) == 0) {
			mathFunctionLength = 6;
			if(num >= -1 && num <= 1)
				num = acos(num);
			else
				error(ARC_OUT_OF_BOUNDS, 0);
		} else if(strncmp(leftBracket - 6, "arctan", 6) == 0) {
			mathFunctionLength = 6;
			num = atan(num);
		}
	} else if(leftBracket > str + 4 && strncmp(leftBracket - 5, "floor", 5) == 0) {
		mathFunctionLength = 5;
		num = floor(num);
	} else if(leftBracket > str + 3 && strncmp(leftBracket - 4, "ceil", 4) == 0) {
		mathFunctionLength = 4;
		num = ceil(num);
	} else if(leftBracket > str + 2) {	
		if(strncmp(leftBracket - 3, "abs", 3) == 0) {
			mathFunctionLength = 3;
			num = fabs(num);
		} else if(strncmp(leftBracket - 3, "exp", 3) == 0) {
			mathFunctionLength = 3;
			num = exp(num);
		} else if(strncmp(leftBracket - 3, "sin", 3) == 0) {
			mathFunctionLength = 3;
			num = sin(num);
		} else if(strncmp(leftBracket - 3, "cos", 3) == 0) {
			mathFunctionLength = 3;
			num = cos(num);
		} else if(strncmp(leftBracket - 3, "tan", 3) == 0) {
			mathFunctionLength = 3;
			num = tan(num);
		}
	} else if(strncmp(leftBracket - 2, "ln", 2) == 0) {
		mathFunctionLength = 2;
		if(num > 0)
			num = log(num);
		else
			error(LN_OUT_OF_BOUNDS, 0);
	} else
		error(NOT_DECLARED, *(leftBracket - 1));
	return num;
}
