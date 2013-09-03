// sorter.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
template<typename T, typename C>
void SelectionSort(T *arr, size_t n, C c)
{
	for(size_t i = 0; i < n; i++)
	{
		size_t min = i;
		for(size_t j = i + 1; j < n; j++)
			if (c(arr[j], arr[min])) 
				min = j;
		std::swap(arr[i], arr[min]);
	}
}

template<typename T, typename C>
void BubbleSort(T *arr, size_t n, C c)
{
	for(size_t i = 0; i < n; i++)
		for(size_t j = 0; j < n-1; j++)
			if (c(arr[j], arr[j+1]))
				std::swap(arr[j], arr[j+1]);
}

template<typename T, typename C>
void raw_qSort(T *arr, size_t n, C c)
{
	T x = arr[n >> 1];
	int i = 0;
	int j = n;
	do
	{
		while(c(arr[i], x)) i++;
		while(c(x, arr[j])) j--;
		if (i <= j)
		{
			std::swap(arr[i], arr[j]);
			i++;
			j--;
		}
	} while (i <= j);
	if (j > 0)
		raw_qSort(arr, j, c);
	if (n > i)
		raw_qSort(arr+i, n-i, c);
}

template<typename T, typename C>
void QuickSort(T *arr, size_t n, C c)
{
	raw_qSort(arr, n-1, c);
}

struct CLess
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};

struct CGreat
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};


void TestGen(size_t len, int count)
{
	std::ofstream f;
	f.open("arrays.txt");
	srand(time(NULL));
	for(size_t i=0; i < count; i++)
	{
		for(size_t j=0; j < len; j++)
			f << rand() % 1000 << " ";
		f << std::endl;
	}
	f.close();
}

template<typename C>
void Checker(size_t len, int count, C c)
{
	std::ifstream f;
	f.open("sorted.txt");
	int temp, min;
	int testsFail = 0;

	for(size_t i=0; i < count; i++)
	{
		f >> min;
		bool flag = false;
		for(size_t j=1; j < len; j++)
		{
			f >> temp;
			if (!flag && c(temp, min)) 
			{
				flag = true;
				testsFail++;
			}
			min = temp;
		}
		std::cout << "Test " << i << ": " << (flag ? "FAIL" : "OK") << std::endl;
	}
	std::cout << "Successful: " << (count-testsFail) << std::endl;
	std::cout << "Failed: " << testsFail << std::endl;
	f.close();
}

void DoSort()
{
	int count = 1000;
	int len = 50;
	int *arr;
	CLess cmp;
	TestGen(len, count);
	std::ifstream in;
	std::ofstream out;
	in.open("arrays.txt");
	out.open("sorted.txt", std::ios_base::trunc);
	for(size_t i = 0; i < count; i++)
	{
		arr = new int[len];
		for(size_t j = 0; j < len; j++)
			in >> arr[j];
		QuickSort(arr, len, cmp);
		for(size_t j = 0; j < len; j++)
			out << arr[j] << " ";
		out << std::endl;
	}

	in.close();
	out.close();

	Checker(len, count, cmp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoSort();
	getch();
	return 0;
}

