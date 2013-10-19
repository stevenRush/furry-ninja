#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class suffix_automata
{
	static const int alphabet_size = 26;

	struct suffix_automata_state
	{
		int length;
		int link;
		int end_pos;
		int next[alphabet_size];

		suffix_automata_state()
		{
			init_jumps();
			length = 0;
			link = -1;
			end_pos = -1;
		}

		suffix_automata_state(int length, int end_pos, int link = -1)
		{
			init_jumps();
			this->length = length;
			this->end_pos = end_pos;
			this->link = link;
		}

		void init_jumps()
		{
			memset(next, -1, sizeof(int) * alphabet_size);
		}
	};

	int last;
	std::vector<suffix_automata_state> states;

	void extend(char character)
	{
		int char_index = character - 'A';
		int new_state_index = states.size();
		suffix_automata_state new_state;
		new_state.length = states[last].length + 1;
		new_state.end_pos = states[last].length;
		states.push_back(new_state);

		int position = set_jumps(last, char_index, new_state_index);

		if (position == -1)
		{
			states[new_state_index].link = 0;
		}
		else
		{
			int state_to_split = states[position].next[char_index];
			if (states[position].length + 1 == states[state_to_split].length)
			{
				states[new_state_index].link = state_to_split;
			}
			else
			{
				suffix_automata_state clone;
				int clone_index = states.size();
				clone.length = states[position].length + 1;
				memcpy(clone.next, states[state_to_split].next, sizeof(int) * alphabet_size);
				clone.link = states[state_to_split].link;

				replace_jumps(position, char_index, clone_index, state_to_split);

				states[state_to_split].link = clone_index;
				states[new_state_index].link = clone_index;
				clone.end_pos = -1;
				states.push_back(clone);
			}
		}
		last = new_state_index;
	}

	void replace_jumps(int position, int char_index, int jump_index, int old_jump_index)
	{
		while(position != -1 && states[position].next[char_index] == old_jump_index)
		{
			states[position].next[char_index] = jump_index;
			position = states[position].link;
		}
	}

	int set_jumps(int position, int char_index, int jump_index)
	{
		while(position != -1 && states[position].next[char_index] == -1)
		{
			states[position].next[char_index] = jump_index;
			position = states[position].link;
		}
		return position;
	}

	int find_jump(int current_position, int char_index)
	{
		while(current_position != -1 && states[current_position].next[char_index] == -1)
		{
			current_position = states[current_position].link;
		}
		return current_position;
	}

public:
	void build(const std::string & string)
	{
		int n = string.length();
		suffix_automata_state root_state;
		root_state.link = -1;
		root_state.end_pos = -1;
		states.push_back(root_state);
		last = 0;
		for(auto it = string.begin(); it != string.end(); ++it)
		{
			extend(*it);
		}
	}

	std::string lcs(const std::string & string)
	{
		int position = 0;
		int current_length = 0;
		int lcs_length = 0;
		int lcs_end_index = -1;
		for(int i=0; i < string.length(); ++i)
		{
			int char_index = string[i] - 'A';
			if (states[position].next[char_index] == -1)
			{
				position = find_jump(position, char_index);
				if (position == -1) 
				{
					position = 0;
					current_length = 0;
					continue;
				}
				current_length = states[position].length;
			}
			++current_length;
			position = states[position].next[char_index];
			if (lcs_length < current_length) 
			{
				lcs_length = current_length;
				lcs_end_index = i;
			}
		}
		return string.substr(lcs_end_index - lcs_length + 1, lcs_length);
	}
};

void input(std::string & string1, std::string & string2)
{
	size_t length;
	std::cin >> length;
	std::cin >> string1;
	std::cin >> string2;
}

std::string solve(const std::string & string1, const std::string & string2)
{
	suffix_automata automata;
	automata.build(string1);
	return automata.lcs(string2);
}

void output(const std::string & string)
{
	std::cout << string << std::endl;
}

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);
	std::string string1;
	std::string string2;
	input(string1, string2);
	std::string answer = solve(string1, string2);
	output(answer);
	return 0;
}