/*increase boolean*/
#ifndef _BOOL_
#define _BOOL_
#define bool  char
#define true  1
#define false 0
#endif

extern double Ans;

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
inline bool isFunction(char c);
char* getInfixNotation(void);
bool inputInit(char* s);
void replaceNumber(char* a, double insert, const char* b);
bool functionInit(char* s);
bool replaceFuntion(char* s, function c, int functionLength);
