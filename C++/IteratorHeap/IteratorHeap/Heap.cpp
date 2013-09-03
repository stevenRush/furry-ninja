#include "Heap.h"
#include <iostream>


Heap::Heap(void) :
	base(0)
{

}


Heap::~Heap(void)
{

}

size_t Heap::Add(int val)
{
	size_t res = base++;
	size_t ind = m_Items.size();
	m_Items.push_back(val);
	m_ReverAdr.push_back(res);
	std::pair<size_t, size_t> pair(res, ind);
	m_Adr.insert(pair);
	Heapify(ind);
	return res;
}

void Heap::Delete(size_t ind)
{
	size_t realIndex = m_Adr[ind];
	Swap(realIndex, m_Items.size()-1);
	m_Items.pop_back();
	m_ReverAdr.pop_back();
	m_Adr.erase(ind);
	Heapify(realIndex);
}

void Heap::Swap(size_t a, size_t b)
{
	std::swap(m_Items[a], m_Items[b]);
	std::swap(m_Adr[m_ReverAdr[a]], m_Adr[m_ReverAdr[b]]);
	std::swap(m_ReverAdr[a], m_ReverAdr[b]);
}

int Heap::GetMax() const
{
	return m_Items[0];
}

void Heap::Heapify(size_t ind)
{
	if (ind > m_Items.size()-1)
		return;

	if (ind > 0 && m_Items[ind] > m_Items[(ind-1) / 2])
	{
		Swap(ind, (ind-1) / 2);
		Heapify((ind-1) / 2);
		return;
	}

	if (m_Items.size() < 2*ind+2)
		return;	

	int swap_index = -1;

	if (m_Items.size() == 2*ind+2)
		swap_index = 2*ind+1;
	else
		swap_index = m_Items[2*ind+1] > m_Items[2*ind+2] ? 2*ind+1 : 2*ind+2;
	
	if (m_Items[ind] < m_Items[swap_index])
	{
		Swap(ind, swap_index);
		Heapify(swap_index);
	}
}

void Heap::Print() const
{
	for(size_t i=0; i < m_Items.size(); ++i)
		std::cout << m_Items[i] << " ";
	std::cout << std::endl;
}