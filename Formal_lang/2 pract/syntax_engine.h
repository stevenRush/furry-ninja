#pragma once

#include <map>
#include <vector>
#include <string>

#include "types.h"

class method_t;
class method_signature_t;
class code_block_t;
class expression_t;
class statement_list_t;
class parameter_list_t;

extern std::vector<std::string> main_args;
extern std::string main_args_id;

class expression_t 
{
public:
	virtual variant_t eval(code_block_t * block) = 0;
};

class statement_t
{
public:
	virtual flow_interruption_type execute(code_block_t * block) = 0;
};

class code_block_t
{
private:
	std::map<std::string, variable_t> scope_variables;
	std::vector<code_block_t *> blocks;

	bool check_declared(const std::string & name);

public:
	code_block_t * parent;

	code_block_t(code_block_t * parent = NULL)
	{
		this->parent = parent;
	}

	void add_sub_block(code_block_t * sub_block)
	{
		blocks.push_back(sub_block);
	}

	void clear_scope()
	{
		scope_variables.clear();
	}

	variable_t get_variable(const std::string & name);
	void declare_variable(const std::string & name, variable_type type);
	void set_variable(const std::string & name, variant_t value);
	void declare_set_variable(const std::string & name, variant_t value);
	
};

class class_t
{
protected:
	code_block_t * class_block;
	statement_list_t * fields_declaration;
	method_t * main;
	std::map<std::string, method_t *> methods;
	std::string ID;
public:
	class_t();

	void set_name(const char * name);

	code_block_t * get_code_block() 
	{
		return class_block;
	}

	void set_main(method_t * main)
	{
		this->main = main;
	}

	method_t * get_method(const std::string & ID)
	{
		auto it = methods.find(ID);
		if (it == methods.end())
		{
			return NULL;
		}
		
		return it->second;
	}

	void add_field_declaration(statement_t * stmt);

	void run();
	void add_method(method_t * method);
};

class library_class_t : public class_t
{
public:
	library_class_t();
	code_block_t * get_code_block();
	void run();
};

class method_t 
{
protected:
	code_block_t * method_block;
	variable_type return_type;
	method_signature_t * arguments;
	statement_t * body;
	variant_t return_value;
	size_t arguments_passed;

public:
	std::string ID;

	method_t(const char * name, variable_type return_type, method_signature_t * args);
	void set_body(statement_t * body);
	void set_block(code_block_t * method_block);
	void set_return_value(variant_t value);
	variable_type get_return_type();
	virtual void add_argument(variant_t value);
	virtual variant_t run();
	const char * get_id();
};

class library_method_t : public method_t
{
protected:
	std::vector<variant_t> args;
public:
	library_method_t(const char * name, variable_type return_type, method_signature_t * args);
	void set_block();
	virtual variant_t run() = 0;
	void add_argument(variant_t value);
};

class println_method_t : public library_method_t
{
public:
	println_method_t();
	virtual variant_t run();
};

class get_int_arg_method_t : public library_method_t
{
public:
	get_int_arg_method_t();
	virtual variant_t run();
};

class get_bool_arg_method_t : public library_method_t
{
public:
	get_bool_arg_method_t();
	virtual variant_t run();
};

class assert_true_method_t : public library_method_t
{
public:
	assert_true_method_t();
	virtual variant_t run();
};

class argument_t 
{
private:
	std::string ID;
	variable_type type;

public:
	argument_t(const std::string & name, variable_type type)
	{
		this->ID = name;
		this->type = type;
	}

	friend class method_t;
};

class method_signature_t
{
private:
	std::vector<argument_t *> args;
public:
	size_t size()
	{
		return args.size();
	}

	argument_t * get_at(size_t index)
	{
		return args[index];
	}

	void add(argument_t * arg)
	{
		args.push_back(arg);
	}
};

class code_block_statement_t : public statement_t
{
private:
	statement_list_t * body;
public:
	code_block_statement_t(statement_list_t * body);
	flow_interruption_type execute(code_block_t * block);
};

class declaration_t : public statement_t
{
private:
	std::string ID;
	variable_type type;
	expression_t * value;
public:
	declaration_t(const char * name, variable_type type, expression_t * value = NULL)
	{
		this->ID = name;
		this->type = type;
		this->value = value;
	}

	flow_interruption_type execute(code_block_t * block)
	{
		block->declare_variable(ID, type); 
		if (value != NULL)
		{
			block->set_variable(ID, value->eval(block));
		}
		return fitNoIterruption;
	}
};

class assignment_t : public statement_t
{
private:
	std::string ID;
	expression_t * value;
public:
	assignment_t(const char * name, expression_t * value)
	{
		this->ID = name;
		this->value = value;
	}

	flow_interruption_type execute(code_block_t * block)
	{
		block->set_variable(ID, value->eval(block));
		return fitNoIterruption;
	}
};

class statement_list_t : public statement_t
{
private:
	std::vector<statement_t *> statements;
public:
	statement_list_t();

	void add(statement_t * stmt)
	{
		statements.push_back(stmt);
	}

	flow_interruption_type execute(code_block_t * block)
	{
		flow_interruption_type result;
		for(auto it = statements.begin(); it != statements.end(); ++it)
		{
			result = (*it)->execute(block);
			if (result != fitNoIterruption)
			{
				return result;
			}
		}
		return fitNoIterruption;
	}
};

class return_statement_t : public statement_t
{
private:
	expression_t * value;
	method_t * method;
public:
	return_statement_t(expression_t * value, method_t * method)
	{
		this->value = value;
		this->method = method;
	}

	flow_interruption_type execute(code_block_t * block);
};

class constant_t : public expression_t
{
private:
	variant_t value;
public:
	constant_t(variant_t value)
	{
		this->value = value;
	}

	virtual variant_t eval(code_block_t * block)
	{
		return value;
	}
};

class variable_expression_t : public expression_t 
{
private:
	std::string ID;
public:
	variable_expression_t(const char * name)
	{
		this->ID = name;
	}

	variant_t eval(code_block_t * block);
};

class binary_expression_t : public expression_t 
{
private:
	expression_t * arg1;
	expression_t * arg2;
	operation type;

	bool check_types(variant_t value1, variant_t value2);

public:
	binary_expression_t(operation type, expression_t * arg1, expression_t * arg2)
	{
		this->type = type;
		this->arg1 = arg1;
		this->arg2 = arg2;
	}

	variant_t eval(code_block_t * block);
};

class invocation_expression_t : public expression_t
{
private:
	parameter_list_t * params;
	std::string method_id;
	class_t * clazz;
public:
	invocation_expression_t(parameter_list_t * params, const char * method_name, class_t * clazz)
	{
		this->params = params;
		method_id = method_name;
		this->clazz = clazz;
	}

	variant_t eval(code_block_t * block);
};

class parameter_list_t 
{
private:
	std::vector<expression_t *> params;
public:
	parameter_list_t()
	{
	}

	size_t size()
	{
		return params.size();
	}

	expression_t * get_at(size_t index)
	{
		if (index >= params.size())
		{
			return NULL;
		}
		return params[index];
	}

	void add(expression_t * expr)
	{
		params.push_back(expr);
	}
};

class conditional_statement_t : public statement_t
{
private:
	statement_t * true_way;
	statement_t * false_way;
	expression_t * condition;
public:
	conditional_statement_t(expression_t * condition, statement_t * true_way, statement_t * false_way = NULL);
	flow_interruption_type execute(code_block_t * block);
};

class while_statement_t : public statement_t
{
private:
	expression_t * condition;
	statement_t * body;
public:
	while_statement_t(expression_t * condition, statement_t * body);
	flow_interruption_type execute(code_block_t * block);
};

class break_statement_t : public statement_t
{
	flow_interruption_type execute(code_block_t * block);
};

class invoke_statement_t : public statement_t
{
private:
	expression_t * invokee;
public:
	invoke_statement_t(expression_t * invokee);
	flow_interruption_type execute(code_block_t * block);
};

// Utilities

void raise_error(const char * format, ...);
variable_type parse_type(const char * string);
const char * to_string(variable_type type);
const char * to_string(variant_t value);
bool variant_equals(variant_t first, variant_t second);
method_signature_t * make_signature(int count, ...); 
int parse_int(const std::string & st);
bool parse_bool(const std::string & st);

#include "java.tab.hpp"