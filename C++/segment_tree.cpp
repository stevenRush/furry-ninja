#include <vector>
#include <iostream>
#include <limits>
#include <utility>

class segment_tree
{
	struct segment_tree_node
	{
		int minimum;
		int minimum_count;
		int maximum;
		int sum;

		void set_value(int value)
		{
			minimum = value;
			maximum = value;
			minimum_count = 1;
			sum = value;
		}

		void aggregate(const segment_tree_node & left, const segment_tree_node & right)
		{
			minimum = std::min(left.minimum, right.minimum);
			maximum = std::max(left.maximum, right.maximum);
			
			minimum_count = 0;
			if (minimum == left.minimum)
			{
				minimum_count += left.minimum_count;
			}
			if (minimum == right.minimum)
			{
				minimum_count += right.minimum_count;
			}

			sum = left.sum + right.sum;
		}
	};

	std::vector<segment_tree_node> nodes;
	size_t array_size;

	void build(const std::vector<int> array, size_t current_node, size_t left_bound, size_t right_bound)
	{
		if (left_bound == right_bound)
		{
			nodes[current_node].set_value(array[left_bound]);	
		}
		else
		{
			int middle = (left_bound + right_bound) >> 1;
			build(array, current_node * 2, left_bound, middle);
			build(array, current_node * 2 + 1, middle + 1, right_bound);
			nodes[current_node].aggregate(nodes[2 * current_node], nodes[2 * current_node + 1]);
		}
	}

	int raw_sum(int current_node, int left, int right, int left_bound, int right_bound) 
	{
		if (left_bound > right_bound)
		{
			return 0;
		}

		if (left == left_bound && right == right_bound)
		{
			return nodes[current_node].sum;
		}

		int middle = (left + right) / 2;
		return	raw_sum (2 * current_node, left, middle, left_bound, std::min(right_bound, middle)) + 
				raw_sum (2 * current_node + 1, middle + 1, right,  std::max(left_bound, middle + 1), right_bound);
	}

	void raw_update(int current_node, int left, int right, size_t position, int new_value)
	{
		if (left == right)
		{
			nodes[current_node].sum = new_value;
		}
		else
		{
			int middle = (left + right) / 2;
			if (position <= middle)
			{
				raw_update(2 * current_node, left, middle, position, new_value);
			}
			else
			{
				raw_update(2 * current_node + 1, middle + 1, right, position, new_value);
			}
			nodes[current_node].aggregate(nodes[2 * current_node], nodes[2 * current_node + 1]);
		}
	}

	int raw_maximum(int current_node, int left, int right, int left_bound, int right_bound) 
	{
		if (left_bound > right_bound)
		{
			return 0;
		}

		if (left == left_bound && right == right_bound)
		{
			return nodes[current_node].maximum;
		}

		int middle = (left + right) / 2;
		return std::max(	raw_maximum (2 * current_node, left, middle, left_bound, std::min(right_bound, middle)), 
							raw_maximum (2 * current_node + 1, middle + 1, right,  std::max(left_bound, middle + 1), right_bound));
	}

	std::pair<int, int> raw_minimum(int current_node, int left, int right, int left_bound, int right_bound) 
	{
		if (left_bound > right_bound)
		{
			return std::make_pair(std::numeric_limits<int>::max(), 0);
		}

		if (left == left_bound && right == right_bound)
		{
			return std::make_pair(nodes[current_node].minimum, nodes[current_node].minimum_count);
		}

		int middle = (left + right) / 2;
		auto left_result = raw_minimum(2 * current_node, left, middle, left_bound, std::min(right_bound, middle));
		auto right_result = raw_minimum(2 * current_node + 1, middle + 1, right,  std::max(left_bound, middle + 1), right_bound);
		int minimum = std::min(left_result.first, right_result.first);
		int minimum_count = 0;
		if (left_result.first == minimum)
		{
			minimum_count += left_result.second;
		}
		if (right_result.first == minimum)
		{
			minimum_count += right_result.second;
		}
		return std::make_pair(minimum, minimum_count);
	}

public:
	segment_tree(const std::vector<int> & array)
	{
		nodes.resize(array.size() * 4);
		array_size = array.size();
		build(array, 1, 0, array.size() - 1);
	}

	int sum(size_t left_bound, size_t right_bound)
	{
		return raw_sum(1, 0, array_size - 1, left_bound, right_bound);	
	}

	void update(size_t position, int new_value)
	{
		return raw_update(1, 0, array_size - 1, position, new_value);
	}

	int maximum(size_t left_bound, size_t right_bound)
	{
		return raw_maximum(1, 0, array_size - 1, left_bound, right_bound);
	}

	std::pair<int, int> minimum(size_t left_bound, size_t right_bound)
	{
		return raw_minimum(1, 0, array_size - 1, left_bound, right_bound);
	}
};