#pragma once
#include <mutex>
#include <boost\thread\shared_mutex.hpp>
#include <boost\thread\locks.hpp>
#include <vector>

template<typename T, size_t SIZE>
class threadsafe_array
{
public:
	threadsafe_array(void)
	{
	}

	~threadsafe_array(void)
	{
	}

	size_t size() const;
	bool empty() const;

	T get(size_t index) const;
	void get(size_t index, T & value) const;
	T operator[](size_t index) const;
	void set(size_t index, const T & value);

	void fill(const T & value);
	void swap(threadsafe_array & other);

private:
	static const size_t BLOCK_SIZE = SIZE > 32 ? 16 : 4;
	static const size_t BLOCK_COUNT = (SIZE / BLOCK_SIZE) + 1;
	T data[SIZE];
	mutable boost::shared_mutex mutexes[BLOCK_COUNT];

	size_t get_block_num(size_t index) const
	{
		return index / BLOCK_SIZE;
	}

	std::vector<std::unique_lock<boost::shared_mutex>> lock_array() const;
};
