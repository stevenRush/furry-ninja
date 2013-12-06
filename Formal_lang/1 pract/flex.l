%option noyywrap
%option nounistd
%option never-interactive

%{
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <iostream>

#include "my_flex_consts.h"

std::stack<double> stack;

double get_operand()
{
	if (stack.empty())
	{
		std::cout << "missing operand" << std::endl;
		exit(-1);
	}
	double result = stack.top();
	stack.pop();
	return result;
}

void process_operation(token_type type)
{
	double arg2 = get_operand();
	double arg1 = get_operand();
	double result;
	switch(type)
	{
	case PLUS:
		result = arg1 + arg2;
		break;
	case MINUS:
		result = arg1 - arg2;
		break;
	case MULTIPLY:
		result = arg1 * arg2;
		break;
	case DIVIDE:
		if (fabs(arg2) < DBL_EPSILON)
		{
			std::cout << "dividing by zero!" << std::endl;
			exit(-1);
		}
		result = arg1 / arg2;
		break;	
	}
	stack.push(result);
}
%}

FLOAT [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
SPACES [ \t]

%%
{FLOAT}		{
				stack.push(atof(yytext));
			}
"+"			{
				process_operation(PLUS);
			}
"-"			{
				process_operation(MINUS);
			}
"*"			{
				process_operation(MULTIPLY);
			}
"/"			{
				process_operation(DIVIDE);
			}
{SPACES}	// do nothing
.			{
				std::cout << "unrecognized character" << std::endl;
			}
%%
int main(int argc, char * argv[])
{
	if (argc < 2) 
	{
        std::cout << "Error: too few arguments. \nUSAGE: program input.txt" << std::endl;
        exit(-1);
    } 
    yyin = fopen(argv[1], "r");
    if (!yyin) 
	{
        std::cout << "Error: input file not found" << std::endl;
        exit(-1);
    }
	yylex();
	if (stack.size() != 1)
	{
		std::cout << "incorrect expression" << std::endl;
		exit(-1);
	}
	std::cout << stack.top() << std::endl;
	return 0;
}