#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "commandCheck.h"
#include "shuntingYardAlgorithm.h"

/*#define DEBUG*/

/* Calculator *
 * Commands   *
 * Constant   *
 * Operators  *
 * Funtions   *
 */

double Ans = 0;

int main(int argc, char *argv[]) {
	interfaceInit();
	double result;
	char *input = NULL, *reversePolishNotation = NULL;
	while(strcmp(input = getInfixNotation(), "quit") != 0) {
		#ifdef DEBUG
		printf("input:%s\n", input);
		#endif
		if(!commandCheck(input) && (reversePolishNotation = shuntingYardAlgorithm(input))) {
			#ifdef DEBUG
			printf("reversePolishNotation:%s\n", reversePolishNotation);
			#endif
			if(calculate(reversePolishNotation, &result))
				printf("= %g\n\n", Ans = result);
		}
		free(input);
	}
	free(input);
	return EXIT_SUCCESS;
}

void interfaceInit(void) {
	system("color F0");
	system("title --------------------Welcome to use the calculator program--------------------");
	printf("Please enter the expression or command with English characters:\t\t");
	puts  ("Or you can enter \"help\" to know more.\n");
}