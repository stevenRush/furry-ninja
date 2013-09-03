#pragma once

#include <vector>
#include <unordered_map>

class Heap
{
private:
	size_t base;
	std::unordered_map<size_t, size_t> m_Adr;
	std::vector<size_t> m_ReverAdr;
	std::vector<int> m_Items;

	void Heapify(size_t);
	void Swap(size_t, size_t);

public:
	Heap(void);
	~Heap(void);

	size_t Add(int);
	void Delete(size_t);
	int GetMax() const;
	void Print() const;
};

