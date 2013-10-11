#include <iostream>
#include <vector>

//#define PRINT_INFO

class FenvikTree
{
private:
	std::vector<std::vector<std::vector<int>>> v;
	int N;
public:
	int Sum(int, int, int, int, int, int);
	void Update(int, int, int, int);
	FenvikTree(int);
	int Sum(int, int, int);
	void Update(int, int, int, int, int, int, int);
};

FenvikTree::FenvikTree(int N)
{
	this->N = N;
	std::vector<int> t1;
	std::vector<std::vector<int>> t2;
	t1.assign(N, 0);
	for(size_t i=0; i < N; ++i)
		t2.push_back(t1);
	for(size_t i=0; i < N; ++i)
		v.push_back(t2);
}

int FenvikTree::Sum(int x, int y, int z)
{
	int result = 0;
	if (x < 0 || y < 0 || z < 0)
		return result;
	for(int i=x; i >= 0; i = (i & (i+1)) - 1)
		for(int j=y; j >= 0; j = (j & (j+1)) - 1)
			for(int k=z; k >= 0; k = (k & (k+1)) - 1)
				result += v[i][j][k];
#ifdef PRINT_INFO
	std::cout << "Sum of (" << x << " " << y << " " << z << ") is " << result << std::endl;
#endif
	return result;
}

int FenvikTree::Sum(int x1, int x2, int y1, int y2, int z1, int z2)
{
	int result = 0;
	result += Sum(x2, y2, z2);
	result -= Sum(x2, y2, z1-1) + Sum(x1-1, y2, z2) + Sum(x2, y1-1, z2);
	result += Sum(x2, y1-1, z1-1) + Sum(x1-1, y2, z1-1) + Sum(x1-1, y1-1, z2);
	result -= Sum(x1-1, y1-1, z1-1);
	return result;
}

void FenvikTree::Update(int x, int y, int z, int delta)
{
	for(int i=x; i < N; i = (i | (i+1)))
		for(int j=y; j < N; j = (j | (j+1)))
			for(int k=z; k < N; k = (k | (k+1)))
				v[i][j][k] += delta;
#ifdef PRINT_INFO
	std::cout << "Updating value: ( "  << x << " " << y << " " << z << ") delta= " << delta << std::endl;
#endif
}

void FenvikTree::Update(int x1, int x2, int y1, int y2, int z1, int z2, int delta)
{
	for(int i=x1; i <= x2; ++i)
		for(int j=y1; j <= y2; ++j)
			for(int k=z1; k <= z2; ++k)
				Update(i, j, k, delta);
}

int main()
{
	//freopen("C:\\temp\\in.txt", "r", stdin);
	int n, op, x1, x2, y1, y2, z1, z2, delta;
	std::cin >> n;
	std::cin >> op;

	FenvikTree *t = new FenvikTree(n);
	while(op < 3)
	{
		if (op == 1)
		{
			std::cin >> x1 >> y1 >> z1 >> delta;
			t->Update(x1, y1, z1, delta);
		}
		else
		{
			std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
			std::cout << t->Sum(x1, x2, y1, y2, z1, z2) << std::endl;
		}
		std::cin >> op;
	}
	return 0;
}

