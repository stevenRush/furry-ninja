#include <iostream>
#include <string>
#include <queue>

class Trie
{
private:
	static const int alphabet_size = 256;
	struct Node 
	{
		Node * children[alphabet_size];
		Node * jumps[alphabet_size];
		Node * parent;
		Node * suff_link;
		Node * term_link;
		unsigned char char_to_parent;
		size_t terminal_count;
		size_t level;

		Node(Node * parent, unsigned char char_to_parent)
		{
			memset(children, 0, sizeof(Node*) * alphabet_size);
			memset(jumps, 0, sizeof(Node*) * alphabet_size);
			suff_link = NULL;
			this->parent = parent;
			this->char_to_parent = char_to_parent;
			terminal_count = 0;
			term_link = NULL;
		}

		~Node()
		{
			for(size_t index = 0; index < alphabet_size; ++index)
			{
				if (children[index] != NULL)
					delete children[index];
			}
		}
	};

	static unsigned char get_char_index(unsigned char letter)
	{
		return letter;
	}

	Node * root;

	Node * get_suff_link(Node * node)
	{
		if (!node->suff_link)
		{
			if (node == root || node->parent == root)
			{
				node->suff_link = root;
			}
			else
			{
				node->suff_link = get_go(get_suff_link(node->parent), node->char_to_parent);
			}
		}
		return node->suff_link;
	}

	Node * get_go(Node * node, unsigned char next_char)
	{
		if (!node->jumps[next_char])
		{
			if (node->children[next_char])
			{
				node->jumps[next_char] = node->children[next_char];
			}
			else
			{
				node->jumps[next_char] = (node == root) ? root : get_go(get_suff_link(node), next_char);
			}
		}
		return node->jumps[next_char];
	}

	size_t count_terminals(Node * node)
	{
		if (node == NULL)
		{
			return 0;
		}

		return node->terminal_count + count_terminals(node->term_link);
	}

public:
	Trie()
	{
		root = new Node(NULL, 0);
	}

	void add_string(const std::string & st)
	{
		Node * current = root;
		for(auto it = st.begin(); it != st.end(); ++it)
		{
			unsigned char char_index = get_char_index(*it);
			if (current->children[char_index] == NULL)
			{
				current->children[char_index] = new Node(current, char_index);
			}
			current = current->children[char_index];
		}
		current->terminal_count += 1;
		current->level = st.size();
	}

	int process_text(const std::string & text) 
	{
		Node * current = root;
		size_t answer = 0;
		for(auto it = text.begin(); it != text.end(); ++it)
		{
			unsigned char char_index = get_char_index(*it);
			current = get_go(current, char_index);
			answer += count_terminals(current);
		}
		return answer;
	}

	void calculate_links()
	{
		std::queue<Node *> queue;
		queue.push(root);
		while(!queue.empty())
		{
			Node * current_node = queue.front();
			queue.pop();

			if (current_node == root || current_node->parent == root)
			{
				current_node->suff_link = root;
				current_node->term_link = NULL;
			}
			else
			{
				current_node->suff_link = get_go(current_node->parent->suff_link, current_node->char_to_parent);
				current_node->term_link = current_node->suff_link->terminal_count > 0 ? current_node->suff_link : current_node->suff_link->term_link;
			}

			for(size_t index = 0; index < alphabet_size; ++index)
			{
				Node * child = current_node->children[index];
				if (child != NULL)
				{
					queue.push(child);
				}
			}
		}
	}

	~Trie()
	{
		delete root;
	}
};

int main()
{
	Trie trie;
	trie.add_string("aba");
	trie.add_string("b");
	trie.calculate_links();
	std::cout << trie.process_text("abababa") << std::endl;
	return 0;
}