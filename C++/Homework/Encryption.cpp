#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

template<typename T>
struct ImplicityTreap
{
	int priority;
	size_t size;
	T key;
	ImplicityTreap* Left;
	ImplicityTreap* Right;

	ImplicityTreap(T key);
};

template<typename T>
ImplicityTreap<T>::ImplicityTreap(T Key)
{
	this->key = Key;
	priority = rand();
	Left = 0;
	Right = 0;
	size = 1;
}

template<typename T>
size_t SizeOf(const ImplicityTreap<T>* t)
{
	return t ? t->size : 0;
}

template<typename T>
void Recalc(ImplicityTreap<T>* t)
{
	if (!t)
		return;
	t->size = SizeOf(t->Left) + SizeOf(t->Right) + 1;
}

template<typename T>
ImplicityTreap<T>* Merge(ImplicityTreap<T>* l, ImplicityTreap<T>* r)
{
	if (!l)
		return r;
	if (!r)
		return l;
	ImplicityTreap<T>* result;

	if (l->priority >= r->priority)
	{
		result = l;
		result->Right = Merge(l->Right, r);
	}
	else
	{
		result = r;
		result->Left = Merge(l, r->Left);
	}
	Recalc(result);
	return result;
}

template<typename T>
void Split(ImplicityTreap<T>* t, size_t x, ImplicityTreap<T>*& l, ImplicityTreap<T>*& r)
{
	if (!t)
	{
		l = 0;
		r = 0;
		return;
	}

	size_t curIndex = SizeOf(t->Left) + 1;
	if (curIndex <= x)
	{
		l = t;
		Split(t->Right, x-curIndex, l->Right, r);
		Recalc(l);
	}
	else
	{
		r = t;
		Split(t->Left, x, l, r->Left);
		Recalc(r);
	}
}

template<typename T>
void PushBack(ImplicityTreap<T>*& t, T value)
{
	ImplicityTreap<T>* p = new ImplicityTreap<T>(value);
	t = Merge(t, p);
}

//#define PRINT_SIZES

template<typename T>
void PrintRecursive(ImplicityTreap<T>* t, std::string& s)
{
	if (!t)
		return;
	PrintRecursive(t->Left, s);
#ifdef PRINT_SIZES
	s += "(";
	s += t->key;
	s += ",";
	s += t->size;
	s += ") ";
#else
	s += t->key;
#endif
	PrintRecursive(t->Right, s);
}

template<typename T>
std::string Print(ImplicityTreap<T>* t)
{
	std::string s;
	PrintRecursive(t, s);
	return s;
}

template<typename T>
void DeleteTree(ImplicityTreap<T>* t)
{
	if (!t)
		return;
	DeleteTree(t->Left);
	DeleteTree(t->Right);
	delete t;
}

template<typename T>
void Encrypt(ImplicityTreap<T>*& t, size_t L, size_t R, size_t s)
{
	ImplicityTreap<T> *l1, *l2, *l3, *r;
	Split(t, L, l1, r);
	Split(r, R-L+1-s, l2, r);
	Split(r, s, l3, r);
	t = Merge(l1, l3);
	t = Merge(t, l2);
	t = Merge(t, r);
}

int main()
{
	srand(time(NULL));
	//freopen("C:\\temp\\in.txt", "r", stdin);
	ImplicityTreap<char>* t = 0;
	std::string st;
	std::cin >> st;
	t = new ImplicityTreap<char>(st[0]);
	for(size_t i=1; i < st.size(); ++i)
		PushBack(t, st[i]);
	int N;
	int l, r, s;
	std::cin >> N;
	for(size_t i=0; i < N; ++i)
	{
		std::cin >> l;
		std::cin >> r;
		std::cin >> s;
		Encrypt<char>(t, l-1, r-1, s);
	}
	std::cout << Print(t) << std::endl;
	DeleteTree(t);
	return 0;
}