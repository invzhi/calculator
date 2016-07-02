/*increase boolean*/
#ifndef _BOOL_
#define _BOOL_
#define bool  char
#define true  1
#define false 0
#endif

typedef struct {
	double* stack;
	unsigned int n;
	unsigned int size;
} Stack;

void stackInit(Stack* s);
void stackPush(Stack* s, double value);
double stackPop(Stack* s);