#include "Heap.h"
#include <deque>
#include <iostream>

int main()
{
	freopen("C:\\in.txt", "r", stdin);
	std::deque<size_t> d;
    Heap h;
	int M;
	int s;
	int max;
	std::cin >> M;
	for(size_t i = 0; i < M-1; ++i)
	{
		std::cin >> s;
		d.push_back(h.Add(s));
	}
	std::cin >> s;
	do
	{
		d.push_back(h.Add(s));
		std::cout << h.GetMax() << std::endl;
		h.Delete(d.front());
		d.pop_front();
		std::cin >> s;
	} while(s != -1);
	fclose(stdin);


	return 0;
}