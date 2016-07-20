#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandCheck.h"

bool commandCheck(char* s) {
	bool result = false;
	if(strcmp(s, "cls") == 0) {
		result = true;
		system("cls");
		printf("Please enter the expression or command with English characters:\t\t");
		puts  ("Or you can enter \"help\" to know more.\n");
	} else if(strcmp(s, "color") == 0) {
		result = true;
		static bool color = 0; /*Black->1 White->0*/
		color = !color;
		printf("\n");
		system(color ? "color 0F" : "color F0");
	} else if(strcmp(s, "help") == 0) {
		result = true;
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
	return result;
}