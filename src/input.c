#include <stdio.h>
#include <ctype.h>
#include <math.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "input.h"

char* functionName[] = {
	"arcsin",
	"arccos",
	"arctan",
	"floor",
	"ceil",
	"abs",
	"exp",
	"sin",
	"cos",
	"tan",
	"ln",
};

inline bool isInput(char c) {
	return (c >= ' ' && c <= '~');
}

inline bool isFunction(char c) {
	return (c >= 'A' && c < FUNCTION_END);
}

bool functionInit(char* s) {
	char* ptr;
	char ch;
	int i;
	for(i = 0; i < FUNCTION_END - 'A'; i++) {
		int length = strlen(functionName[i]);
		while(ptr = strstr(s, functionName[i])) {
			if(*(ptr + length) != '(') {
				printf("[Error]: no '(' after function \"%s\"\n\n", functionName[i]);
				return false;
			}
			*ptr++ = 'A' + i;
			while(ch = *(ptr + length - 1))
				*ptr++ = ch;
			*ptr = ch;
		}
	}
	return true;
}

char* getInfixNotation(void) {
	unsigned int size = 100u, blockSize = 50u;
	char* buffer = (char*)malloc(size * sizeof(char));
	char c = EOF;
	int n = 0;
	while(buffer != NULL && (c = getchar()) != EOF && c != '\n') {
		if(isspace(c))
			continue;
		if(!isInput(c) || c == ')' && buffer[n - 1] == '(') {
			puts(isInput(c) ? "[Error]: empty between '(' and ')'\n" : "[Error]: you can only enter characters with English\n");
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
		buffer = (char*)realloc(buffer, (n + 1) * sizeof(char));
		if(!functionInit(buffer)) {
			free(buffer);
			buffer = getInfixNotation();
		}
	}
	return buffer;
}
