%{
#include "bisondef.h"

// forward declarations
int yylex();
void yyerror(const char *);

extern int yylineno;
extern FILE * yyin;

std::vector<std::string> main_args;
std::string main_args_id;
class_t main_class;
library_class_t library;
variable_type current_type;
method_t * current_method;

%}

%union {
	method_t * method;
	expression_t * expr;
	method_signature_t * args;
	argument_t * arg;
	statement_list_t * stmts;
	statement_t * stmt;
	parameter_list_t * params;
	variable_type type;
	operation op;
	variant_t value;
	char word[256];
};

// Types
%token <type> TYPE
%token INT
%token BOOL

// Comparison operations
%token LE
%token GE
%token NEQ
%token EQ

// Logical operations
%token AND
%token OR

%token <word> ID
%token MAIN;
%token MAIN_ARGS;

// reserved words
%token PUBLIC
%token VOID
%token STATIC
%token CLASS
%token RETURN
%token IF
%token ELSE
%token WHILE
%token BREAK;

// types
%type <expr> expression;
%type <expr> invoke_expression;
%type <value> constant;
%type <op> binary_operation;
%type <method> method_declaration;
%type <method> method_header;
%type <args> decl_param_list;
%type <arg> decl_param;
%type <stmts> statement_list;
%type <stmt> code_block;
%type <stmt> statement;
%type <stmt> declaration;
%type <stmt> assignment;
%type <expr> term;
%type <stmt> field_declaration;
%type <stmt> return;
%type <stmt> conditional_statement;
%type <params> param_list;
%type <stmt> cycle_statement;
%type <stmt> break_statement;
%type <stmt> invoke_statement;

// some conflict rules
%left '=' NEQ EQ LE GE
%left '+' '-' OR
%left '*' '/' AND
%nonassoc NOT

%% 

program : /* empty */
	| class

class : class_header '{' class_body '}'

class_header : PUBLIC CLASS ID

class_body : /* empty */
	| class_body member_declaration

member_declaration : field_declaration
		{
			main_class.add_field_declaration($1);
		}
	| method_declaration
		{
			main_class.add_method($1);
		}

field_declaration : member_modifier declaration
		{
			$$ = $2;
		}

method_header : PUBLIC STATIC VOID MAIN '(' MAIN_ARGS ID ')'
		{
			$$ = new method_t("main", vtNoType, new method_signature_t());
			main_class.set_main($$);
			main_args_id = $7;

			variant_t args_value;
			args_value.type = vtInt;
			args_value.int_value = -1;
			main_class.add_field_declaration(new declaration_t($7, vtInt, new constant_t(args_value)));
		}
	| member_modifier TYPE ID '(' decl_param_list ')' 
		{
			$$ = new method_t($3, $2, $5);
			$$->set_block(new code_block_t(main_class.get_code_block()));
			current_method = $$;
		}
	| member_modifier VOID ID '(' decl_param_list ')'
		{
			$$ = new method_t($3, vtNoType, $5);
			$$->set_block(new code_block_t(main_class.get_code_block()));
			current_method = $$;
		}

member_modifier : PUBLIC STATIC
	| STATIC

method_declaration: method_header code_block
		{
			$$ = $1;
			$$->set_body($2);
			current_method = NULL;
		}

decl_param_list : /* empty */
		{
			$$ = new method_signature_t();
		}
	| decl_param
		{
			$$ = new method_signature_t();
			$$->add($1);
		}
	| decl_param_list ',' decl_param
		{
			$$ = $1;
			$$->add($3);
		}

decl_param : TYPE ID
		{
			$$ = new argument_t($2, $1);
		}

code_block : '{' statement_list '}'
	{
		$$ = new code_block_statement_t($2);
	}

statement_list : /* empty */
		{
			$$ = new statement_list_t();
		}
	| statement_list statement
		{
			$$ = $1;
			$$->add($2);
		}

statement : declaration
		{
			$$ = $1;
		}
	| code_block
		{
			$$ = $1;
		}
	| assignment
		{
			$$ = $1;
		}
	| return
		{
			$$ = $1;
		}
	| conditional_statement
		{
			$$ = $1;
		}
	| cycle_statement
		{
			$$ = $1;
		}
	| break_statement
		{
			$$ = $1;
		}
	| invoke_statement
		{
			$$ = $1;
		}

invoke_statement : invoke_expression ';'
		{
			$$ = new invoke_statement_t($1);
		}

break_statement: BREAK ';'
		{
			$$ = new break_statement_t();
		}

cycle_statement : WHILE '(' expression ')' statement
		{
			$$ = new while_statement_t($3, $5);
		}

conditional_statement :	IF '(' expression ')' statement
		{
			$$ = new conditional_statement_t($3, $5);
		}
	| IF '(' expression ')' statement ELSE statement
		{
			$$ = new conditional_statement_t($3, $5, $7);
		}

return : RETURN expression ';'
		{
			$$ = new return_statement_t($2, current_method);
		}
	| RETURN ';'
		{
			$$ = new return_statement_t(NULL, current_method);
		}

declaration : TYPE ID ';'
		{
			$$ = new declaration_t($2, $1);
		}
	| TYPE ID '=' expression ';'
		{
			$$ = new declaration_t($2, $1, $4);
		}

assignment : ID '=' expression ';'
		{
			$$ = new assignment_t($1, $3);
		}

expression : expression binary_operation expression
		{
			$$ = new binary_expression_t($2, $1, $3);
		}
	| term
		{
			$$ = $1;
		}
	| invoke_expression
		{
			$$ = $1;
		}

invoke_expression: ID '.' ID '(' param_list ')'
		{
			$$ = new invocation_expression_t($5, $3, &library);
		}
	| ID '(' param_list ')'
		{
			$$ = new invocation_expression_t($3, $1, &main_class);
		}

param_list : /* empty */
		{
			$$ = new parameter_list_t();
		}
	| expression
		{
			$$ = new parameter_list_t();
			$$->add($1);
		}
	| param_list ',' expression
		{
			$$ = $1;
			$$->add($3);
		}

term : ID
		{
			$$ = new variable_expression_t($1);
		}
	| '(' expression ')'
		{
			$$ = $2;
		}
	| constant
		{
			$$ = new constant_t($1);
		}

binary_operation : 
	  '+'
		{
			$$ = operation::opAdd;
		}
	| '-'
		{
			$$ = operation::opSub;
		}
	| '*'
		{
			$$ = operation::opMul;
		}
	| '/'
		{
			$$ = operation::opDiv;
		}
	| '%'
		{
			$$ = operation::opMod;
		}
	| AND
		{
			$$ = operation::opAnd;
		}
	| OR
		{
			$$ = operation::opOr;
		}
	| EQ
		{
			$$ = operation::opEquals;
		}
	| NEQ
		{
			$$ = operation::opNotEquals;
		}
	| '<'
		{
			$$ = operation::opLesser;
		}
	| '>'
		{
			$$ = operation::opGreater;
		}
	| LE
		{
			$$ = operation::opLesserEquals;
		}
	| GE
		{
			$$ = operation::opGreaterEquals;
		}

constant : INT
	| BOOL
%%

void yyerror(const char *s) 
{
	fprintf(stderr, "line number %d: ", yylineno);
	fprintf(stderr, "%s\n", s);
}

int main(int argc, const char* argv[]) 
{
	if (argc < 0) 
	{
		printf("Usage: %s <input_file> [<args>]\n", argv[0]);
		exit(0);
	}
	else 
	{
		yyin = fopen(argv[1], "r");
	} 
	for(size_t index = 0; index < argc - 2; ++index)
	{
		main_args.push_back(argv[index + 2]);
	}
	yyparse();
	main_class.run();
	return 0;
}
