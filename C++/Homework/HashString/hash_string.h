#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

class hash_string
{
private:
	static const long long power = 257;
	static const long long mod = 1000 * 1000 * 1000 + 7;

	std::vector<long long> powers;
	std::vector<long long> hash;
	std::string text;
	
	void init_powers();
	long long get_power(size_t index);
	void raw_push_char(char c);

public:
	hash_string(const std::string & text);
	void push_back(const std::string & string);
	void push_char(char c);
	long long get_hash(size_t begin, size_t end);
};