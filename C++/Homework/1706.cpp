#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

class suffix_array
{
private:
	size_t length;
	std::string string;
	std::vector<int> suffixes;
	std::vector<int> order;
	std::vector<long> lcp;

public:
	suffix_array(const std::string & word)
	{
		string = word;
		length = string.length();
	}

	void init()
	{
		std::vector<int> classes(length);
		suffixes.resize(length);
		order.resize(length);

		for(size_t index = 0; index < length; ++index)
		{
			suffixes[index] = index;
			order[index] = string[index];
		}

		for (size_t gramm_length = 1;; gramm_length *= 2)
		{
			auto gramm_comparator = [=] (size_t first, size_t second) -> bool
			{
				if (order[first] != order[second])
				return order[first] < order[second];
				first += gramm_length;
				second += gramm_length;
				return (first < length && second < length) ? order[first] < order[second] : first > second;
			};

			std::sort(suffixes.begin(), suffixes.end(), gramm_comparator);

			for(size_t index = 0; index < length-1; ++index)
			{
				classes[index + 1] = classes[index] + gramm_comparator(suffixes[index], suffixes[index + 1]);
			}

			for(size_t index = 0; index < length; ++index)
			{
				order[suffixes[index]] = classes[index];
			}

			if (classes[length - 1] == length - 1) 
			{
				break;
			}
		}
	}

	void calc_lcp()
	{
		lcp.resize(length-1);
		lcp.assign(length-1, 0);
		size_t prefix_length = 0;

		for (int index = 0; index < length; ++index) if (order[index] != length - 1)
		{
			for (int jndex = suffixes[order[index] + 1]; string[index + prefix_length] == string[jndex + prefix_length];)
			{
				++prefix_length;
			}

			lcp[order[index]] = prefix_length;

			if (prefix_length > 0)
			{
				prefix_length -= 1;
			}
		}
	}

	long long count_unique_substr()
	{
		calc_lcp();
		long long sum_lcp = std::accumulate(lcp.begin(), lcp.end(), 0);
		long long long_length = length;
		return long_length * (long_length + 1L) / 2L - sum_lcp;
	}
};


void input(std::istream & in, size_t & k, std::string & word)
{
#ifndef ONLINE_JUDGE
	freopen("C:\\temp\\input.txt", "r", stdin);
#endif
	in >> k;
	in >> word;
}

std::string shift_string(const std::string & word, size_t count, size_t index)
{
	size_t tail_length = word.length() - index;
	if (tail_length >= count)
	{
		return word.substr(index, count);
	}
	else
	{
		std::string result = word.substr(index, tail_length);
		result += word.substr(0, count - tail_length);
		return result;
	}
}

void calc_unique_substrs_for_every_shift(size_t k, const std::string & word, std::vector<int> & answer)
{
	answer.clear();
	for(size_t index = 0; index < word.length(); ++index)
	{
		std::string shifted = shift_string(word, k, index);
		suffix_array array(shifted);
		array.init();
		answer.push_back(array.count_unique_substr());
	}
}

void output(std::ostream & out, std::vector<int> & answer)
{
	for(auto it = answer.begin(); it != answer.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

int main()
{
	size_t k;
	std::string word;
	std::vector<int> answer;
	input(std::cin, k, word);
	calc_unique_substrs_for_every_shift(k, word, answer);
	output(std::cout, answer);
	return 0;
}