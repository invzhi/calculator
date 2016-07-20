/*increase boolean*/
#ifndef _BOOL_
#define _BOOL_
#define bool  char
#define true  1
#define false 0
#endif

extern double ans;

typedef struct {
	char str[];
	double num;
} Constant;

const Constant constant[] = {
	{"pi", 3.14159265359},
	{"ans", ans},
}

typedef enum {
	DIGIT,
	CONSTANT,
	OPERATOR,
	FUNCTION,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	UNKNOWN,
} charType;

inline bool isOperator(char c);
int isConstant(char* ptr);
charType getType(char* c, char sign);
unsigned int getPriority(char c);
char* shuntingYardAlgorithm(const char *input);
bool calculate(char* s, double* result);
