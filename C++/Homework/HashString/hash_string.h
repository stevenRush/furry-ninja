#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

class hash_string
{
private:
	static const long long power = 257;
	static const long long mod =  2971215073;

	std::vector<long long> powers;
	std::vector<long long> hash;
	std::string text;
	
	void init_powers();
	long long get_power(size_t index);
	void raw_push_char(char c);

public:
	hash_string(const std::string & text);
	hash_string();
	void print_substring(size_t begin, size_t end);
	void push_back(const std::string & string);
	void push_char(char c);
	size_t size();
	long long get_hash(size_t begin, size_t end);
};