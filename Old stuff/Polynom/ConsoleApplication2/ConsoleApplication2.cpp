// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Polynom.h"

int _tmain(int argc, _TCHAR* argv[])
{
	double arr1[] = {1, 4, 4, 0, 1};
	double arr2[] = {1, 0};
	CPolynom a(arr1, sizeof(arr1)/sizeof(arr1[0])-1);
	CPolynom b(arr2, sizeof(arr2)/sizeof(arr2[0])-1);
	CPolynom c = CPolynom::NOD(a, a.Derivative());
	//a.Derivative().Print();
	a.Decomposition();
	//a.Derivative().Print();
	//c.Print();
	//std::cout << b.ValueAt(1) << std::endl;
	//std::cout << b.ValueAt(-1) << std::endl;
	return 0;
}

