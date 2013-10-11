#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <queue>
#include <stdio.h>
#include <cstdlib>

char st[900001];
size_t length;

typedef std::pair<size_t, unsigned char> move_pair;

template<typename value_type>
class avl_tree
{
	struct Node
	{
		move_pair key;
		value_type value;
		unsigned char height;
		Node * left;
		Node * right;

		Node(move_pair key, value_type value)
		{
			this->key = key;
			this->value = value;
			height = 1;
			left = right = NULL;
		}
	};

	Node pool[100000];
	size_t pool_counter;

	typedef Node * pNode;

	pNode root;

	unsigned char height(pNode node)
	{
		return node ? node->height : 0;
	}

	int balance_factor(pNode node)
	{
		return height(node->right) - height(node->left);
	}

	void fix_height(pNode node)
	{
		unsigned char height_left = height(node->left);
		unsigned char height_right = height(node->right);
		node->height = (height_left > height_right ? height_left : height_right) + 1;
	}

	pNode rotate_right(pNode p)
	{
		pNode q = p->left;
		p->left = q->right;
		q->right = p;
		fix_height(p);
		fix_height(q);
		return q;
	}

	pNode rotateleft(pNode q) 
	{
		pNode p = q->right;
		q->right = p->left;
		p->left = q;
		fix_height(q);
		fix_height(p);
		return p;
	}

	pNode balance(pNode p) 
	{
		fix_height(p);
		if( balance_factor(p)==2 )
		{
			if( balance_factor(p->right) < 0 )
				p->right = rotate_right(p->right);
			return rotateleft(p);
		}
		if( balance_factor(p)==-2 )
		{
			if( balance_factor(p->left) > 0  )
				p->left = rotateleft(p->left);
			return rotate_right(p);
		}
		return p; 
	}

	pNode insert_node(pNode p, move_pair key, value_type value) // вставка ключа k в дерево с корнем p
	{
		if(!p) 
		{
			return new(pool + pool_counter++) Node(key, value);
		}
		if( key < p->key )
			p->left = insert_node(p->left, key, value);
		else
			p->right = insert_node(p->right, key, value);
		return balance(p);
	}

	value_type search_node(pNode tree, move_pair key)
	{
		if (!tree)
		{
			return value_type();
		}

		if (tree->key == key)
		{
			return tree->value;
		}

		if (key < tree->key)
		{
			return search_node(tree->left, key);
		}
		else
		{
			return search_node(tree->right, key);
		}
	}

	void find_jumps_node(pNode node, size_t number, std::queue<int> & queue)
	{
		if (!node)
		{
			return;
		}

		if (number <= node->key.first)
		{
			find_jumps_node(node->left, number, queue);
		}
		if (number == node->key.first)
		{
			queue.push(node->value->number);
		}
		if (number >= node->key.first)
		{
			find_jumps_node(node->right, number, queue);
		}
	}

public:
	avl_tree()
	{
		root = NULL;
		pool_counter = 0;
	}

	void insert(move_pair key, value_type value)
	{
		root = insert_node(root, key, value);
	}

	value_type search(move_pair key)
	{
		return search_node(root, key);
	}
	
	void find_jumps(size_t number, std::queue<int> & queue)
	{
		find_jumps_node(root, number, queue);
	}
};

inline void getline()
{
	length = 0;
	char c;
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
			suff_link = NULL;
			term_link = NULL;
			is_leaf = false;
			number = node_number;
		}

		Node()
		{
		}
	};

	Node * root;
	avl_tree<Trie::Node *> children;
	static Node pool[pool_size];
	static size_t counter;

	friend class avl_tree<Trie::Node *>;

public:
	Trie()
	{
		counter = 0;
		root = new(pool + counter++) Node(NULL, 0, counter-1);
	}

	void add_string()
	{
		Node * current = root;
		for(size_t index = 0; index < length; ++index)
		{
			unsigned char char_index = st[index];
			auto child_map_key = std::make_pair(current->number, char_index);
			Node * child = children.search(child_map_key);
			if (child == NULL)
			{
				current = new(pool + counter++) Node(current, char_index, counter-1);
				children.insert(child_map_key, current);
			}
			else
			{
				current = child;
			}
		}
		current->is_leaf = true;
		current->level = length;
	}

	int process_text() 
	{
		Node * current = root;
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

	void add_words_from_stdin()
	{
		size_t samples_count;
		std::cin >> samples_count;
		getline();
		for(size_t index = 0; index < samples_count; ++index)
		{
			getline();
			add_string();
		}
	}

	Node * get_go(Node * node, unsigned char next_char)
	{
		while(true)
		{
			auto child_map_key = std::make_pair(node->number, next_char);
			Node * child = children.search(child_map_key);
			if (child != NULL)
			{
				return child;
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
			children.find_jumps(current, queue);
		}
	}

	~Trie()
	{

	}
};

Trie::Node Trie::pool[pool_size];
size_t Trie::counter;

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);
	Trie trie;
	trie.add_words_from_stdin();	
	size_t strings_count;
	std::cin >> strings_count;
	bool found = false;
	getline();
	trie.calculate_links();
	for(size_t index = 0; index < strings_count; ++index)
	{
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