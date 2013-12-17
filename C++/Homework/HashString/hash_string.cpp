#include "hash_string.h"


void hash_string::init_powers()
{
	powers.push_back(1);
	for(size_t index = 0; index < text.size() - 1; ++index)
	{
		powers.push_back(powers.back() * power % mod);
	}
}

long long hash_string::get_power(size_t index)
{
	if (powers.size() <= index)
	{
		while(powers.size() <= index)
		{
			powers.push_back(powers.back() * power % mod);
		}
	}
	return powers[index];
}

hash_string::hash_string(const std::string & text)
{
	this->text = text;
	init_powers();

	if (text.empty())
	{
		return;
	}

	hash.push_back(text[0] % mod);

	for(auto it = text.begin() + 1; it != text.end(); ++it)
	{
		raw_push_char(*it);
	}
}

void hash_string::raw_push_char(char c)
{
	size_t next_power = hash.size();
	long long hash_value = (c * get_power(next_power) % mod + hash.back()) % mod;
	hash.push_back(hash_value);
}

void hash_string::push_back(const std::string & string)
{
	text += string;
	for(auto it = string.begin(); it != string.end(); ++it)
	{
		raw_push_char(*it);
	}
}

void hash_string::push_char(char c)
{
	text.push_back(c);
	raw_push_char(c);
}

long long hash_string::get_hash(size_t begin, size_t end)
{
	if (begin > hash.size() || end > hash.size())
	{
		throw std::out_of_range("index out of bounds!");
	}
	long long hash_start = (begin == 0 ? 0 : hash[begin - 1]) * get_power(end - begin) % mod;
	return (hash[end - 1] - hash_start + mod) % mod;	// hack for C++ mod operation
}