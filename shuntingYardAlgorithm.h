/*increase boolean*/
#ifndef _BOOL_
#define _BOOL_
#define bool  char
#define true  1
#define false 0
#endif

extern double ans;
extern double pi;

typedef struct {
	char* str;
	double* num;
} Constant;

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
int isConstant(const char* ptr);
charType getType(const char* c, char sign);
unsigned int getPriority(char c);
char* shuntingYardAlgorithm(const char* inptr);
bool calculate(char* s, double* result);
