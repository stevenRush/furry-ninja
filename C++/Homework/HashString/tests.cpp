#define BOOST_TEST_MODULE HashStringTest
#include <boost\test\included\unit_test.hpp>
#include <stdexcept>
#include <chrono>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>

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
{	\
	BOOST_TEST_MESSAGE(message); \
	auto start = std::chrono::high_resolution_clock::now(); \
	expression \
	auto end = std::chrono::high_resolution_clock::now();	\
	BOOST_TEST_MESSAGE("execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms");	\
}

	

/*
	C++ implementation of the algorithm in Figure 1 from a paper by Sawada and Ruskey
	Author: http://stackoverflow.com/users/181106/uckelman
*/

struct seq_printer {
  const std::vector<char>& _alpha;
  hash_string st;

  seq_printer(const std::vector<char>& alpha) : _alpha(alpha) {}

  void operator() (unsigned int* a, unsigned int* a_end) {
    for (unsigned int* i = a; i < a_end; ++i) {
		st.push_char(_alpha[*i]);
    }
  }
};

void debruijn(unsigned int t,
              unsigned int p,
              const unsigned int k,
              const unsigned int n,
              unsigned int* a,
              seq_printer & callback)
{
  if (t > n) {
    // we want only necklaces, not pre-necklaces or Lyndon words
    if (n % p == 0) {
      callback(a+1, a+p+1);
    }
  }
  else {
    a[t] = a[t-p];

    debruijn(t+1, p, k, n, a, callback);

    for (unsigned int j = a[t-p]+1; j < k; ++j) {
      a[t] = j;
      debruijn(t+1, t, k, n, a, callback);
    }
  }
}

BOOST_AUTO_TEST_CASE(simple_test)
{
	hash_string string1("abcabc");
	BOOST_CHECK_EQUAL(string1.get_hash(0, 3), string1.get_hash(3, 6));	// abc == abc
	BOOST_CHECK_NE(string1.get_hash(0, 3), string1.get_hash(0, 2));		// abc != ab
	hash_string string2("abcdef");
	BOOST_CHECK_NE(string2.get_hash(0, 3), string2.get_hash(3, 6));		// abc != def
}

BOOST_AUTO_TEST_CASE(empty_string_test)
{
	hash_string string("");
	BOOST_CHECK_NO_THROW(string.get_hash(0, 0));
}

BOOST_AUTO_TEST_CASE(empty_substrings_test)
{
	hash_string string("abacaba");
	BOOST_CHECK_EQUAL(string.get_hash(0, 0), string.get_hash(3, 3));
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
	BOOST_CHECK_EXCEPTION(string.get_hash(4, 3), std::invalid_argument, true_predicate);
}

BOOST_AUTO_TEST_CASE(power_of_2_test)
{
	BOOST_TEST_MESSAGE("Tests for power of 2 module: ");
	for(size_t index = 1; index < 18; ++index)
	{
		BOOST_TEST_MESSAGE("Test for Thue-Morse sequence of order " << index);
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

BOOST_AUTO_TEST_CASE(de_bruijn_test)
{
	BOOST_TEST_MESSAGE("starting testing substring collision: ");
	const int DE_BRUIJN_SEQUENCE_ORDER = 4;

	std::vector<char> alphabet;
	for(char index = 'A'; index <= 'Z'; ++index)
	{
		alphabet.push_back(index);
	}
	
	unsigned int* a = new unsigned int[DE_BRUIJN_SEQUENCE_ORDER+1];
	a[0] = 0;
	seq_printer printer(alphabet);
	CHECK_TIME("de bruijn generation...", 
		debruijn(1, 1, alphabet.size(), DE_BRUIJN_SEQUENCE_ORDER, a, printer);
	)
	hash_string & hash = printer.st;
	std::unordered_set<long long> hashes;

	CHECK_TIME("hash testing...",
	for(size_t index = 0; index < hash.size() - DE_BRUIJN_SEQUENCE_ORDER + 1; ++index)
	{
		long long ss_hash = hash.get_hash(index, index + DE_BRUIJN_SEQUENCE_ORDER);
		hashes.insert(ss_hash);
	}
	)
	BOOST_CHECK_EQUAL(hashes.size(), hash.size() - DE_BRUIJN_SEQUENCE_ORDER + 1);
	delete[] a;
}