#include <stdio.h>
#include <ctype.h>
#include <math.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "input.h"

inline bool isInput(char c) {
	return (c >= ' ' && c <= '~');
}

inline bool isFunction(char c) {
	return (c >= 'A' && c < FUNCTION_END);
}

void replaceNumber(char* a, double insert, const char* b) {
	char* b_ = (char*)malloc((strlen(b) + 1) * sizeof(char));
	if(b_ == NULL)
		assert(0);
	strcpy(b_, b);
	sprintf(a, "(%lf)%s", insert, b_);
	free(b_);
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
	return functionInit(s);
}

char* getInfixNotation(void) {
	unsigned int size = 100u, blockSize = 50u;
	char* buffer = (char*)malloc(size * sizeof(char));
	char c = EOF;
	int n = 0;
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
