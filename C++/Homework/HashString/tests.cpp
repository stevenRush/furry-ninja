#define BOOST_TEST_MODULE HashStringTest
#include <boost\test\included\unit_test.hpp>
#include <stdexcept>
#include <iostream>
#include <chrono>

#include "hash_string.h"

auto true_predicate = [](const std::exception & e) { return true; };
auto invert = [](char c) -> const char { return c == 'A' ? 'B' : 'A'; };

std::string inverse(const std::string & st)
{
	std::string result;
	for(auto it = st.begin(); it != st.end(); ++it)
	{
		result.push_back(invert(*it));
	}
	return result;
}

std::string get_thue_morse(int order)
{
	if (order == 0)
	{
		return "A";
	}

	std::string st = get_thue_morse(order - 1);
	return st + inverse(st);
}

#define CHECK_TIME(message, expression) \
	auto start = std::chrono::high_resolution_clock::now(); \
	expression \
	auto end = std::chrono::high_resolution_clock::now();	\
	std::cout << message << std::endl; \
	std::cout << "execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

BOOST_AUTO_TEST_CASE(simple_test)
{
	hash_string string1("abcabc");
	BOOST_CHECK_EQUAL(string1.get_hash(0, 3), string1.get_hash(3, 6));	// abc == abc
	BOOST_CHECK_NE(string1.get_hash(0, 3), string1.get_hash(0, 2));		// abc != ab
	hash_string string2("abcdef");
	BOOST_CHECK_NE(string2.get_hash(0, 3), string2.get_hash(3, 6));		// abc != def

}

BOOST_AUTO_TEST_CASE(push_back_test)
{
	hash_string string("abc");
	string.push_back("abc");
	BOOST_CHECK_EQUAL(string.get_hash(0, 3), string.get_hash(3, 6));	// abc == abc
}

BOOST_AUTO_TEST_CASE(exception_test)
{
	hash_string string("abcdef");
	BOOST_CHECK_EXCEPTION(string.get_hash(0, 10), std::out_of_range, true_predicate);
}

BOOST_AUTO_TEST_CASE(power_of_2_test)
{
	for(size_t index = 1; index < 10; ++index)
	{
		hash_string st(get_thue_morse(index));
		int middle_index = 1 << (index - 1);
		int string_length = 1 << index;
		BOOST_CHECK_NE(st.get_hash(0, middle_index), st.get_hash(middle_index, string_length));
	}
}

void measure_time(const std::string & message, size_t string_length)
{
	hash_string st("a");
	CHECK_TIME(message,
		for(size_t index = 0; index < string_length; ++index)
		{
			st.push_char('a');
		}
		st.get_hash(0, string_length);
	)
}

BOOST_AUTO_TEST_CASE(performance_test)
{
	measure_time("string length = 2^10", 1 << 10);
	measure_time("string length = 2^15", 1 << 15);	
	measure_time("string length = 2^20", 1 << 20); 	
}
