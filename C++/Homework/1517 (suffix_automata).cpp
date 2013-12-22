#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class suffix_automata
{
	static const int ALPHABET_SIZE = 26;
	static const char ALPHABET_START = 'A';
	static const int NO_STATE = -1;

	struct suffix_automata_state
	{
		int length;
		int link;
		int end_pos;
		int next[ALPHABET_SIZE];

		suffix_automata_state()
		{
			init_jumps();
			length = 0;
			link = NO_STATE;
			end_pos = NO_STATE;
		}

		suffix_automata_state(int length, int end_pos, int link = NO_STATE)
		{
			init_jumps();
			this->length = length;
			this->end_pos = end_pos;
			this->link = link;
		}

		void init_jumps()
		{
			memset(next, NO_STATE, sizeof(int) * ALPHABET_SIZE);
		}
	};

	int last;
	std::vector<suffix_automata_state> states;

	void change_transitions_for_char(int position, int char_index, int jump_index, int old_jump_index)
	{
		while(position != NO_STATE && states[position].next[char_index] == old_jump_index)
		{
			states[position].next[char_index] = jump_index;
			position = states[position].link;
		}
	}

	int set_transitions_for_char(int position, int char_index, int jump_index)
	{
		while(position != NO_STATE && states[position].next[char_index] == NO_STATE)
		{
			states[position].next[char_index] = jump_index;
			position = states[position].link;
		}
		return position;
	}

	int find_next_state_for_char(int current_position, int char_index)
	{
		while(current_position != NO_STATE && states[current_position].next[char_index] == NO_STATE)
		{
			current_position = states[current_position].link;
		}
		return current_position;
	}

public:
	void extend(char character)
	{
		int char_index = character - ALPHABET_START;
		int new_state_index = states.size();
		suffix_automata_state new_state;
		new_state.length = states[last].length + 1;
		new_state.end_pos = states[last].length;
		states.push_back(new_state);

		int position = set_transitions_for_char(last, char_index, new_state_index);

		if (position == NO_STATE)
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
				memcpy(clone.next, states[state_to_split].next, sizeof(int) * ALPHABET_SIZE);
				clone.link = states[state_to_split].link;

				change_transitions_for_char(position, char_index, clone_index, state_to_split);

				states[state_to_split].link = clone_index;
				states[new_state_index].link = clone_index;
				clone.end_pos = NO_STATE;
				states.push_back(clone);
			}
		}
		last = new_state_index;
	}

	void build(const std::string & string)
	{
		int n = string.length();
		suffix_automata_state root_state;
		root_state.link = NO_STATE;
		root_state.end_pos = NO_STATE;
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
		int lcs_end_index = NO_STATE;
		for(int i=0; i < string.length(); ++i)
		{
			int char_index = string[i] - ALPHABET_START;
			if (states[position].next[char_index] == NO_STATE)
			{
				position = find_next_state_for_char(position, char_index);
				if (position == NO_STATE) 
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

void input(std::istream & in, std::string & string1, std::string & string2)
{
	size_t length;
	in >> length;
	in >> string1;
	in >> string2;
}

std::string calc_lcs_for(const std::string & string1, const std::string & string2)
{
	suffix_automata automata;
	automata.build(string1);
	return automata.lcs(string2);
}

void output(std::ostream & out, const std::string & string)
{
	out << string << std::endl;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("C:\\temp\\input.txt", "r", stdin);
#endif
	std::string string1;
	std::string string2;
	input(std::cin, string1, string2);
	std::string answer = calc_lcs_for(string1, string2);
	output(std::cout, answer);
	return 0;
}