#include <functional>
#include <string>
#include <iostream>
#include <utility>

template<typename Key, typename Value, typename Hash = std::hash<Key> >
class CuckooHash
{
public:

	struct TableElement
	{
		TableElement()
		{
			isFree = true;
		}

		TableElement(const Key & key, const Value & value)
		{	
			this->key = key;
			this->value = value;
			isFree = false;
		}

		Key key;
		Value value;
		bool isFree;
	};

	CuckooHash() :
		m_Size(0),
		m_Capacity(10), 
		m_ShiftCounter(0)
	{	
		for (size_t i = 0; i < 2; ++i)
		{
			m_Table[i] = new TableElement[m_Capacity];
		}
	}

	void Add(const Key & key, const Value & value)
	{
		Value temp;
		if (Find(key, temp))
			return;
		Add(0, key, value);
	}

	bool Find(const Key & key, Value & value) const
	{
		size_t hash;
		for(size_t i = 0; i < 2; ++i)
		{
			hash = GetHash(i, key);
			if (m_Table[i][hash].key == key && m_Table[i][hash].isFree == false)
			{
				value = m_Table[i][hash].value;
				return true;
			}
		}
		return false;
	}

private:
	TableElement *m_Table[2];
	size_t m_Size;
	size_t m_Capacity;
	size_t m_ShiftCounter;

	size_t GetHash(size_t index, const Key & element) const
	{
		size_t hash = Hash()(element);
		if (index == 1)
		{
			return hash % m_Capacity;
		}
		else
		{
			return hash / m_Capacity % m_Capacity;
		}
	}

	void Add(size_t index, const Key & key, const Value & value)
	{
		++m_ShiftCounter;
		size_t hash = GetHash(index, key);
		if (m_Table[index][hash].isFree)
		{
			m_Table[index][hash] = TableElement(key, value);
			m_ShiftCounter = 0;
			++m_Size;
		}
		else
		{
			TableElement oldElem = m_Table[index][hash];
			m_Table[index][hash] = TableElement(key, value);
			if (m_ShiftCounter == m_Capacity)	// we got cycle
			{
				Grow();
				Add(0, oldElem.key, oldElem.value);
			}
			else
			{
				Add(1-index, oldElem.key, oldElem.value);
			}
		}
	}

	void Remove(const Key & key)
	{
		size_t hash;
		for(size_t i = 0; i < 2; ++i)
		{
			hash = GetHash(i, key);
			if (m_Table[i][hash].key == key)
			{
				m_Table[i][hash].isFree = true;
				--m_Size;
				return;
			}
		}
	}

	void Grow()
	{
		m_ShiftCounter = 0;
		TableElement *tempTable[2] = { m_Table[0], m_Table[1] };
		m_Capacity *= 2;
		for (size_t i = 0; i < 2; ++i)
		{
			m_Table[i] = new TableElement[m_Capacity];
		}
		for (size_t tableNum = 0; tableNum < 2; ++tableNum)
		{
			for(size_t index = 0; index < m_Capacity / 2; ++index)
			{
				if (tempTable[tableNum][index].isFree)
					continue;
				Add(tempTable[tableNum][index].key, tempTable[tableNum][index].value);
			}
		}
		for (size_t i = 0; i < 2; ++i)
		{
			delete [] tempTable[i];
		}
		std::cout << "Grow to " << m_Capacity << std::endl;
	}
};

int main()
{
	CuckooHash<int, int> hash;
	for(size_t i = 0; i < 1000; ++i)
	{
		hash.Add(i, i);
	}
	for(size_t i = 0; i < 1000; ++i)
	{
		int temp;
		if (hash.Find(i, temp))
		{
			std::cout << "True" << std::endl;
		}
		else
		{
			std::cout << "False" << std::endl;
			break;
		}
	}
	return 0;
}