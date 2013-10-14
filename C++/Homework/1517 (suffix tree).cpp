#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <set>
#include <vector>

class suffix_tree
{
private:
	struct search_result
	{
		int depth;
		bool first;
		bool second;
		int end_index;

		search_result(int depth, bool first, bool second)
		{
			this->depth = depth;
			this->first = first;
			this->second = second;
			this->end_index = -1;
		}
	};
	
	struct suffix_tree_suffix;
	struct suffix_tree_edge;

	struct suffix_tree_node
	{
		suffix_tree_node * suffix_link;
		suffix_tree * tree;
		std::map<char, suffix_tree_edge *> edges;

		suffix_tree_node(suffix_tree_node * suffix_link, suffix_tree * tree)
		{
			this->suffix_link = suffix_link;
			this->tree = tree;
		}

		void add_edge(char ch, suffix_tree_edge * edge)
		{
			edges.insert(std::make_pair(ch, edge));
		}

		void remove_edge(char ch)
		{
			edges.erase(ch);
		}

		suffix_tree_edge * find_edge(char ch)
		{
			auto it = edges.find(ch);
			if (it == edges.end())
			{
				return NULL;
			}
			else
			{
				return it->second;
			}
		}

		search_result find_delimiters(int first_delimiter, int second_delimiter, int current_depth)
		{
			search_result result(-1, false, false);
			if (edges.empty())
			{
				return result;
			}

			result.depth = current_depth;
			for(auto it = edges.begin(); it != edges.end(); ++it)
			{
				search_result temp = it->second->find_delimiters(first_delimiter, second_delimiter, current_depth);
				int distance = std::distance(edges.begin(), it);
				result.first = result.first || temp.first;
				result.second = result.second || temp.second;
				if (result.depth < temp.depth)
				{
					result.depth = temp.depth;
					result.end_index = temp.end_index;
				}
			}
			if (!result.first || !result.second)
			{
				result.depth = -1;
			}
			return result;
		}
	};

	struct suffix_tree_edge
	{
		int begin_index;
		int end_index;
		suffix_tree_node * start_node;
		suffix_tree_node * end_node;

		suffix_tree_edge(int begin, int end, suffix_tree_node * start)
		{
			begin_index = begin;
			end_index = end;
			start_node = start;
			end_node = new suffix_tree_node(NULL, start->tree);
		}

		suffix_tree_node * split_edge(suffix_tree_suffix * suffix)
		{
			remove();
			suffix_tree_edge * new_edge = new suffix_tree_edge(begin_index, begin_index + suffix->get_span(), suffix->origin_node);
			new_edge->insert();
			new_edge->end_node->suffix_link = suffix->origin_node;
			begin_index += suffix->get_span() + 1;
			start_node = new_edge->end_node;
			insert();
			return new_edge->end_node;
		}

		void insert()
		{
			char begin_char = get_tree()->get_char(begin_index);
			start_node->add_edge(begin_char, this);
		}

		void remove()
		{
			char begin_char = get_tree()->get_char(begin_index);
			start_node->remove_edge(begin_char);
		}

		int get_span()
		{
			return end_index - begin_index;
		}

		bool contains(int char_index)
		{
			return char_index >= begin_index && char_index <= end_index;	
		}

		suffix_tree * get_tree()
		{
			return start_node->tree;
		}

		search_result find_delimiters(int first_delimiter, int second_delimiter, int current_depth)
		{
			int min_distance = std::min(first_delimiter - begin_index, second_delimiter - begin_index);
			search_result result(-1, false, false);
			result.first = contains(first_delimiter);
			result.second = contains(second_delimiter);
			if (result.first && result.second)
			{
				result.first = true;
				result.second = false;
				return result;
			}

			search_result temp = end_node->find_delimiters(first_delimiter, second_delimiter, current_depth + get_span()+1);
			if (temp.first && temp.second)
			{
				result.end_index = temp.end_index == -1 ? end_index : temp.end_index;
				temp.end_index = result.end_index;
			}

			if (!result.first && !result.second)
			{
				return temp;
			}

			result.first = result.first || temp.first;
			result.second = result.second || temp.second;

			if (!result.first || !result.second)
			{
				result.depth = -1;
			}
			return result;
		}
	};

	struct suffix_tree_suffix
	{
		suffix_tree_node * origin_node;
		int begin_index;
		int end_index;

		suffix_tree * get_tree()
		{
			return origin_node->tree;
		}

		suffix_tree_suffix(suffix_tree_node * origin, int begin, int end)
		{
			origin_node = origin;
			begin_index = begin;
			end_index = end;
		}

		bool is_explicit()
		{
			return begin_index > end_index;
		}

		bool is_implicit()
		{
			return end_index >= begin_index;
		}

		void canonize()
		{
			if (!is_explicit())
			{
				suffix_tree * tree = get_tree();
				suffix_tree_edge * edge = origin_node->find_edge(tree->get_char(begin_index));	
				int edge_span = edge->get_span();
				while(edge_span <= get_span())
				{
					begin_index += edge_span + 1;
					origin_node = edge->end_node;
					if (begin_index <= end_index)
					{
						edge = edge->end_node->find_edge(tree->get_char(begin_index));
						edge_span = edge->get_span();
					}
				}
			}
		}

		int get_span()
		{
			return end_index - begin_index;
		}

		void inc_begin_index()
		{
			begin_index += 1;
		}

		void change_origin_node()
		{
			origin_node = origin_node->suffix_link;
		}

		void inc_end_index()
		{
			end_index += 1;
		}
	};

	std::string text;
	suffix_tree_node * root;

	void add_prefix(suffix_tree_suffix * active, int end_index)
	{
		suffix_tree_node * last_parent_node = NULL;
		suffix_tree_node * parent_node;

		while(true)
		{
			suffix_tree_edge * edge;
			parent_node = active->origin_node;

			if (active->is_explicit())
			{
				char end_char = text[end_index];
				edge = active->origin_node->find_edge(end_char);
				if (edge != NULL)
				{
					break;
				}
			}
			else
			{
				char actve_begin_char = text[active->begin_index];
				edge = active->origin_node->find_edge(actve_begin_char);
				int span = active->get_span();
				if (text[edge->begin_index + span + 1] == text[end_index])
				{
					break;
				}
				parent_node = edge->split_edge(active);
			}

			suffix_tree_edge * new_edge = new suffix_tree_edge(end_index, text.length() -1, parent_node);
			new_edge->insert();
			update_suffix_link(last_parent_node, parent_node);
			last_parent_node = parent_node;

			if (active->origin_node == root)
			{
				active->inc_begin_index();
			}
			else
			{
				active->change_origin_node();
			}
			active->canonize();
		}
		update_suffix_link(last_parent_node, parent_node);
		active->inc_end_index();
		active->canonize();
	}

	void update_suffix_link(suffix_tree_node * node, suffix_tree_node * suffix_link)
	{
		if (node != NULL && node != root)
		{
			node->suffix_link = suffix_link;
		}
	}

public:
	suffix_tree()
	{
		root = new suffix_tree_node(NULL, this);
	}

	void add_string(const std::string & string)
	{
		text = string;
		suffix_tree_suffix * active = new suffix_tree_suffix(root, 0, -1);
		for(size_t index = 0; index < string.length(); ++index)
		{
			add_prefix(active, index);	
		}
	}

	char get_char(size_t index)
	{
		return text[index];
	}

	std::string get_lcs(size_t string1_length, size_t string2_length)
	{
		search_result result = root->find_delimiters(string1_length, string1_length + string2_length + 1, 0);
		if (result.depth == 0)
		{
			return std::string();
		}
		else
		{
			return text.substr(result.end_index - result.depth + 1, result.depth);
		}
	}
};

std::string make_big_string(const std::string & string1, const std::string & string2)
{
	std::string result = string1;
	result += (char)('Z' + 1);
	result += string2;
	result += (char)('Z' + 2);
	return result;
}

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);

	suffix_tree tree;
	size_t length;
	std::string string1; 
	std::string string2;
	
	std::cin >> length;
	std::cin >> string1;
	std::cin >> string2;
	std::string big_string = make_big_string(string1, string2);
	tree.add_string(big_string);
	std::string lcs = tree.get_lcs(string1.length(), string2.length());
	std::cout << lcs << std::endl;
	return 0;
}