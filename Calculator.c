#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define bool  char
#define true  1
#define false 0
#define WHITE 0
#define ERROR 0
#define MAXC  500
#define PI    3.14159265358

/*-version v1.1*/

enum errorType {
	OVERMUCH,
	NO_NUMBER,
	NOT_DECLARED,
	ARC_OUT_OF_BOUNDS,
	LN_OUT_OF_BOUNDS,
};

void   interfaceInit ();
void   expressionInit();
void   commandCheck  ();
bool   isOperator(char  c);
void   error(int n, char c);
void   replace(char* a, double num, const char* b);
double calculate(char* s);
double mathFunction(char *leftBracket, double num);

char   str[MAXC + 100];
char  *expression;
bool   wrong;
double Ans = 0;
int    mathFunctionLength = 0; 

int main()
{
	interfaceInit();

	while(true) {
		do {
			wrong = false;
			fgets(str, MAXC, stdin);
			expressionInit();
			commandCheck  ();
		} while(*str == '\0' || wrong);

		double number;
		char *leftBracket, *rightBracket;

		while(rightBracket = strchr(str, ')')) {
			mathFunctionLength = 0;
			*rightBracket = '\0';
			leftBracket = strrchr(str,'(');
			number = calculate(leftBracket + 1);
			if(islower(*(leftBracket - 1)) && !wrong)
				number = mathFunction(leftBracket, number);
			if(wrong)
				break;
			replace(leftBracket - mathFunctionLength, number, rightBracket + 1);
		}
		if(wrong)
			continue;
		Ans = calculate(str);
		if(!wrong)
			printf("= %g\n\n", Ans);
	}
	return 0;
}

void interfaceInit() {
	system("color F0");
	system("title --------------------Welcome to use the calculator program--------------------");
	printf("Please enter the expression or command with English characters:\t\t");
	puts  ("Or you can enter \"help\" to know more.\n");
}

void commandCheck()	{
	if(strcmp(str, "quit") == 0)
		exit(0);
	else if(strcmp(str, "cls") == 0) {
		*str = '\0';
		system("cls");
		printf("Please enter the expression or command with English characters:\t\t");
		puts  ("Or you can enter \"help\" to know more.\n");
	} else if(strcmp(str, "color") == 0) {
		*str = '\0';
		static bool color = WHITE;
		color = !color;
		printf("\n");
		system(color ? "color 0F" : "color F0");
	} else if(strcmp(str, "help") == 0) {
		*str = '\0';
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
	char* c;
	int i, space, bracket;
	i = space = bracket = 0;
	while(str[i]) {
		if(isspace(str[i]))
			space++;
		else {
			if(str[i] == '(')
				bracket++;
			else if(str[i] == ')') {
				if(bracket == 0) {
					error(OVERMUCH, ')');
					return;
				}
				bracket--;
			} else if(isupper(str[i]))
				str[i] = tolower(str[i]);
			i++;
		}
		str[i] = str[i + space];
	}
	if(bracket) {
		error(OVERMUCH, '(');
		return;
	}
	while(c = strstr(str, "pi" ))
		replace(c, PI , c + 2);
	while(c = strstr(str, "ans"))
		replace(c, Ans, c + 3);
}

bool isOperator(char c) {
	bool result = false;
	if(c == '^' || c == '*' || c == '/' || c == '%' || c == '+' || c == '-')
		result = true;
	return result;
}

void replace(char* a, double num, const char* b) {
	bool left = false, right = false;
	if(a > str && a > expression && !isOperator(*(a-1)) && *(a-1) != '(')
		left  = true;
	if(*b != '\0' && !isOperator(*b) && *b != ')')
		right = true;
	char* b_ = (char*) malloc(strlen(b) + 1);
	strcpy(b_, b);
	sprintf(a, "%s%lf%s%s", left?"*":"", num, right?"*":"", b_);
	free(b_);
}

void error(int n, char c) {
	wrong = true;
	switch(n) {
		case OVERMUCH:
			printf("[Error]: too many '%c'\n\n", c);
			break;
		case NO_NUMBER:
			printf("[Error]: no number before '%c'\n\n", c);
			break;
		case NOT_DECLARED:
			printf("[Error]: something was not declared around first '%c'\n\n", c);
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
	double result, cache;
	expression = (char*) malloc(strlen(s) + 50);
	strcpy(expression, s);
	/*Exponent ^ */
	while(power = strchr(expression, '^')) {
		if(power == expression) {
			error(NO_NUMBER, '^');
			return ERROR;
		}
		char* lastNumber = power;
		while(lastNumber > expression) {
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
			return ERROR;
		}
		replace(lastNumber, pow(num1, num2), endptr2);
	}
	result = 0;
	cache = strtod(expression, &endptr1);
	while(*endptr1) {
		if(expression == endptr1){
			error(isOperator(*expression) ? NO_NUMBER : NOT_DECLARED, *expression);
			return ERROR;
		}
		double next = strtod(endptr1 + 1, &endptr2);
		if(endptr1 + 1 == endptr2) {
			error(*endptr1 == *endptr2 ? OVERMUCH : NOT_DECLARED, *endptr1);
			return ERROR;
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
				return ERROR;
		}
		endptr1 = endptr2;
	}
	result += cache;
	free(expression);
	return result;
}

double mathFunction(char *leftBracket, double num) {
	if       (strncmp(leftBracket - 2, "ln"    , 2) == 0){
		mathFunctionLength = 2;
		if(num > 0)
			num = log(num);
		else
			error(LN_OUT_OF_BOUNDS, 0);
	} else if(strncmp(leftBracket - 3, "abs"   , 3) == 0){
		mathFunctionLength = 3;
		num = fabs(num);
	} else if(strncmp(leftBracket - 3, "exp"   , 3) == 0){
		mathFunctionLength = 3;
		num = exp(num);
	} else if(strncmp(leftBracket - 5, "floor" , 5) == 0){
		mathFunctionLength = 5;
		num = floor(num);
	} else if(strncmp(leftBracket - 4, "ceil"  , 4) == 0){
		mathFunctionLength = 4;
		num = ceil(num);
	} else if(strncmp(leftBracket - 6, "arcsin", 6) == 0){
		mathFunctionLength = 6;
		if(num >= -1 && num <= 1)
			num = asin(num);
		else
			error(ARC_OUT_OF_BOUNDS, 0);
	} else if(strncmp(leftBracket - 6, "arccos", 6) == 0){
		mathFunctionLength = 6;
		if(num >= -1 && num <= 1)
			num = acos(num);
		else
			error(ARC_OUT_OF_BOUNDS, 0);
	} else if(strncmp(leftBracket - 6, "arctan", 6) == 0){
		mathFunctionLength = 6;
		num = atan(num);
	} else if(strncmp(leftBracket - 3, "sin"   , 3) == 0){
		mathFunctionLength = 3;
		num = sin(num);
	} else if(strncmp(leftBracket - 3, "cos"   , 3) == 0){
		mathFunctionLength = 3;
		num = cos(num);
	} else if(strncmp(leftBracket - 3, "tan"   , 3) == 0){
		mathFunctionLength = 3;
		num = tan(num);
	} else
		error(NOT_DECLARED, *(leftBracket - 1));
	return num;
}
