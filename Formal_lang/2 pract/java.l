%option noyywrap

%{
	#include "bisondef.h"
	
	void yyerror(const char*);
%}

%option yylineno

LETTER ([a-zA-Z])
DIGIT ([0-9])
WORD ({LETTER}|_)({LETTER}|{DIGIT}|_)*

%%

{DIGIT}+ {
		yylval.value.type = vtInt;
		yylval.value.int_value = atoi(yytext);
		return INT;
	}

"int"	{ 
		yylval.type = vtInt;
		return TYPE; 	
	}
"boolean"	{ 
		yylval.type = vtBool;
		return TYPE; 	
	}
"true"	{		
		yylval.value.type = vtBool;
		yylval.value.bool_value = true;
		return BOOL;
	}
"false" {
		yylval.value.type = vtBool;
		yylval.value.bool_value = false;
		return BOOL;
	}

"public" {
		return PUBLIC;
	}

"void" {
		return VOID;
	}

"class" {
		return CLASS;
	}

"static" {
		return STATIC;
	}

"return" {
		return RETURN;
	}

"if"	{
		return IF;
	}

"else"	{
		return ELSE;
	}

"while"	{
		return WHILE;
	}

"break"	{
		return BREAK;
	}

"main" {
		return MAIN;
	}

"String[]" {
		return MAIN_ARGS;
	}

and		{ return AND; }
or		{ return OR; }
not		{ return NOT; }
"<="	{ return LE; }
">="	{ return GE; }
"=="	{ return EQ; }
"!="	{ return NEQ; }

{WORD}	{
		strcpy(yylval.word, yytext);
		return ID;
	}


[+-/*\()_{}=;,<>.] { return *yytext;}

[ \t\n] ;

.	{
		
		yyerror("Unknown symbol\n");
	};
%%
