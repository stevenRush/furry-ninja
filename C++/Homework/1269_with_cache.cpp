#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <queue>
#include <stdio.h>
#include <cmath>

//std::string st;
char st[1000001];
size_t length;
size_t cache_size = 1;

inline void getline()
{
	char c;
	length = 0;
	while((c = getc(stdin)) != '\n' && !feof(stdin))
		st[length++] = c;
	st[length] = 0;
}

class Trie
{
private:
	static const int pool_size = 100 * 1024;
	struct Node
	{
		Node * parent;
		Node * suff_link;
		Node * term_link;
		unsigned char char_to_parent;
		bool is_leaf;
		short unsigned int level;
		size_t number;

		Node(Node * parent, unsigned char char_to_parent, size_t node_number)
		{
			this->parent = parent;
			this->char_to_parent = char_to_parent;
			number = node_number;
		}

		Node()
		{
		}
	};

	Node * root;
	std::map<std::pair<size_t, unsigned char>, Node*> children;
	double map_size_log;
	static Node pool[pool_size];
	static size_t counter;
	static std::unordered_map<size_t, std::pair<std::map<std::pair<size_t, unsigned char>, Node *>::iterator, size_t>> cache;

public:
	Trie()
	{
		counter = 0;
		root = new(pool + counter++) Node(NULL, 0, counter-1);
	}

	inline void calc_map_size_log()
	{
		map_size_log = 3 * std::log(children.size()) / std::log(2);
	}

	inline void add_string()
	{
		Node * current = root;
		for(size_t index = 0; index < length; ++index)
		{
			unsigned char char_index = st[index];
			auto child_map_key = std::make_pair(current->number, char_index);
			auto child_iter = children.find(child_map_key);
			if (child_iter == children.end())
			{
				current = new(pool + counter++) Node(current, char_index, counter-1);
				children[child_map_key] = current;
			}
			else
			{
				current = child_iter->second;
			}
		}
		current->is_leaf = true;
		current->level = length;
	}

	inline int process_text() 
	{
		Node * current = root;
		int level_found = -1;
		int first_entry = 100 * 10000;
		for(size_t index = 0; index < length; ++index)
		{
			unsigned char char_index = st[index];
			current = get_go(current, char_index);
			if (current->is_leaf)
			{
				int entry = index - current->level + 1;
				if (entry < first_entry)
					first_entry = entry;
			}
			if (current->term_link)
			{
				int entry = index - current->term_link->level + 1;
				if (entry < first_entry)
					first_entry = entry;
			}
		}
		if (root->is_leaf)
			return 0;
		if (first_entry == 100 * 10000)
			return -1;
		else
			return first_entry;
	}

	inline void add_words_from_stdin()
	{
		size_t samples_count;
		std::cin >> samples_count;
		//std::getline(std::cin, st);
		getline();
		for(size_t index = 0; index < samples_count; ++index)
		{
			//std::getline(std::cin, st);
			getline();
			add_string();
		}
	}

	inline Node * get_child_from_map(Node * node, unsigned char next_char)
	{
		auto pair = std::make_pair(node->number, next_char);
		auto it = children.find(pair);
		if (it != children.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}

	inline Node * get_child(Node * node, unsigned char next_char)
	{
		auto it = cache.find(node->number);
		if (it != cache.end())
		{
			if (it->second.second > map_size_log)
				return get_child_from_map(node, next_char);
			else
				return find_jump(it->second.first, next_char);
		}
		else
		{
			auto pair = std::make_pair(node->number, 0);
			auto child_iter = children.lower_bound(pair);
			if (child_iter == children.end())
			{
				return NULL; 
			}
			pair.first++;
			auto child_iter_next = children.lower_bound(pair);
			if (cache.size() >= cache_size)
			{
				cache.erase(cache.begin());
			}
			cache.insert(std::make_pair(node->number, std::make_pair(child_iter, std::distance(child_iter, child_iter_next))));
			return find_jump(child_iter, next_char);
		}
	}

	inline Node * find_jump(std::map<std::pair<size_t, unsigned char>, Node *>::iterator iter, unsigned char next_char)
	{
		size_t number = iter->first.first;
		while(iter != children.end() && iter->first.first == number && iter->first.second != next_char)
		{
			++iter;
		}
		if (iter == children.end() || iter->first.first != number)
		{
			return NULL;
		}
		else
		{
			return iter->second;
		}
	}

	inline Node * get_go(Node * node, unsigned char next_char)
	{
		while(true)
		{
			/*auto child_map_key = std::make_pair(node->number, next_char);
			auto it = children.find(child_map_key);
			if (it != children.end())
			{
				return it->second;
			}*/
			Node * needed_node = get_child(node, next_char);
			if (needed_node != NULL)
			{
				return needed_node;
			}
			if (node == root)
			{
				return root;
			}
			node = node->suff_link;
		}
	}

	inline Node * get_go_without_cache(Node * node, unsigned char next_char)
	{
		while(true)
		{
			auto child_map_key = std::make_pair(node->number, next_char);
			auto it = children.find(child_map_key);
			if (it != children.end())
			{
				return it->second;
			}
			if (node == root)
			{
				return root;
			}
			node = node->suff_link;
		}
	}

	void calculate_links()
	{
		std::queue<int> queue;
		queue.push(0);
		auto child_end = children.end();
		while(!queue.empty())
		{
			int current = queue.front();
			queue.pop();
			Node * current_node = pool + current;
			if (current_node == root || current_node->parent == root)
			{
				current_node->suff_link = root;
				current_node->term_link = NULL;
			}
			else
			{
				current_node->suff_link = get_go(current_node->parent->suff_link, current_node->char_to_parent);
				current_node->term_link = current_node->suff_link->is_leaf ? current_node->suff_link : current_node->suff_link->term_link;
			}
			auto child_map_key = std::make_pair(current, 0);
			auto it = children.lower_bound(child_map_key);
			while(it != child_end && it->first.first == current)
			{
				queue.push(it->second->number);
				++it;
			}
		}
	}

	~Trie()
	{

	}
};

Trie::Node Trie::pool[pool_size];
size_t Trie::counter;
std::unordered_map<size_t, std::pair<std::map<std::pair<size_t, unsigned char>, Trie::Node *>::iterator, size_t>> Trie::cache;

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);
	Trie trie;
	trie.add_words_from_stdin();	
	size_t strings_count;
	std::cin >> strings_count;
	bool found = false;
	//std::getline(std::cin, st);
	getline();
	trie.calculate_links();
	for(size_t index = 0; index < strings_count; ++index)
	{
		//std::getline(std::cin, st);
		getline();
		int entry = trie.process_text();
		if (entry != -1)
		{
			found = true;
			std::cout << (index + 1) << " " << (entry + 1) << std::endl;
			break;
		}
	}
	if (!found)
	{
		std::cout << "Passed" << std::endl;
	}
	return 0;
}