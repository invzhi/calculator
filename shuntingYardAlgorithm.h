/*increase boolean*/
#ifndef _BOOL_
#define _BOOL_
#define bool  char
#define true  1
#define false 0
#endif

typedef enum {
	DIGIT,
	OPERATOR,
	FUNCTION,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	UNKNOWN,
} charType;

inline bool isOperator(char c);
charType getType(char* c, char sign);
unsigned int getPriority(char c);
char* shuntingYardAlgorithm(const char *input);
bool calculate(char* s, double* result);
