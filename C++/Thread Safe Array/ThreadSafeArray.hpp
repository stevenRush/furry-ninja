#pragma once
#include "ThreadSafeArray.h"

template<typename T, size_t SIZE>
size_t threadsafe_array<T, SIZE>::size() const
{
	return SIZE;
}

template<typename T, size_t SIZE>
bool threadsafe_array<T, SIZE>::empty() const
{
	return SIZE == 0;
}

template<typename T, size_t SIZE>
T threadsafe_array<T, SIZE>::get(size_t index) const
{
	if (index >= SIZE)
		throw std::out_of_range("Index out of range!");
	size_t block_num = get_block_num(index);
	boost::shared_lock<boost::shared_mutex> lock(mutexes[block_num]);
	return data[index];
}

template<typename T, size_t SIZE>
void threadsafe_array<T, SIZE>::get(size_t index, T & value) const
{
	if (index >= SIZE)
		throw std::out_of_range();
	size_t block_num = get_block_num(index);
	boost::shared_lock<boost::shared_mutex> lock(mutexes[block_num]);
	value = data[index];
}

template<typename T, size_t SIZE>
T threadsafe_array<T, SIZE>::operator[](size_t index) const
{
	size_t block_num = get_block_num(index);
	boost::shared_lock<boost::shared_mutex> lock(mutexes[block_num]);
	value = data[index];
}

template<typename T, size_t SIZE>
void threadsafe_array<T, SIZE>::set(size_t index, const T & value)
{
	if (index >= SIZE)
		throw std::out_of_range("Index out of range!");
	size_t block_num = get_block_num(index);
	std::unique_lock<boost::shared_mutex> lock(mutexes[block_num]);
	data[index] = value;
}

template<typename T, size_t SIZE>
void threadsafe_array<T, SIZE>::fill(const T & value)
{
	for(size_t block = 0; block < BLOCK_COUNT; ++block)
	{
		std::unique_lock<boost::shared_mutex> lock(mutexes[block]);
		for(size_t index = block * BLOCK_SIZE; index < std::min(SIZE, (block+1) * BLOCK_SIZE); ++index)
		{
			data[index] = value;
		}
	}

}

template<typename T, size_t SIZE>
void threadsafe_array<T, SIZE>::swap(threadsafe_array<T, SIZE> & other)
{
	if (&other == this)		// avoiding deadlock 
		return;
	auto locks = lock_array();
	auto other_locks = other.lock_array();
	std::swap(data, other.data);
}

template<typename T, size_t SIZE>
std::vector<std::unique_lock<boost::shared_mutex>> threadsafe_array<T, SIZE>::lock_array() const
{
	std::vector<std::unique_lock<boost::shared_mutex>> locks;
	for(size_t block = 0; block < BLOCK_COUNT; ++block)
	{
		locks.push_back(std::unique_lock<boost::shared_mutex>(mutexes[block]));
	}
	return std::move(locks);
}

namespace std
{
	template<typename T, size_t SIZE>
	void swap(threadsafe_array<T, SIZE> & a, threadsafe_array<T, SIZE> & b)
	{
		a.swap(b);
	}
}