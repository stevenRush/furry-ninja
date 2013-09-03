#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

struct Segment
{
	int begin;
	int end;

	Segment(int begin, int end)
	{
		this->begin = begin;
		this->end = end;
	}

	int Length()
	{
		return end-begin;
	}
};

struct Tree 
{
	Tree *left;
	Tree *right;
	int priority;
	int begin;
	int end;
	bool white;

	Tree(int begin, int end, int white)
	{
		this->begin = begin;
		this->end = end;
		this->white = white;
		left = right = 0;
		priority = rand();
	}
};

int contain(int l, int r, int x)
{
	if (l <= x && r >= x)
		return 0;
	if (x < l)
		return -1;
	if (x > r)
		return 1;
}

typedef Tree* pTree;

void DeleteTree(pTree);

void Split(pTree t, int k, pTree &l, pTree &r)
{
	if (!t)
	{
		l = 0;
		r = 0;
		return;
	}

	switch(contain(t->begin, t->end, k))
	{
	case -1:
		r = t;
		Split(t->left, k, l, r->left);
		break;
	case 0:
		r = new Tree(*t);
		l = t;
		l->right = 0;
		r->left = 0;
		r->begin = k;
		l->end = k;
		break;
	case 1:
		l = t;
		Split(t->right, k, l->right, r);
		break;
	}
}

pTree Merge(pTree l, pTree r)
{
	if (!l)
		return r;
	if (!r)
		return l;

	pTree result;

	if (l->priority >= r->priority)
	{
		result = l;
		result->right = Merge(l->right, r);
	}
	else
	{
		result = r;
		result->left = Merge(l, r->left);
	}
	return result;
}

void CutTree(pTree &t, int begin, int end, int white)
{
	pTree a, b, c;
	Split(t, begin, a, b);
	Split(b, end, b, c);
	DeleteTree(b);
	b = new Tree(begin, end, white);
	t = Merge(a, b);
	t = Merge(t, c);
}

void DeleteTree(pTree t)
{
	if (!t)
		return;
	DeleteTree(t->left);
	DeleteTree(t->right);
	delete t;
}

void FindWhiteSegments(pTree t, std::vector<Segment> &segs)
{
	if (!t)
		return;

	FindWhiteSegments(t->left, segs);
	if (t->white)
	{
		Segment s(t->begin, t->end);
		segs.push_back(s);
	}
	FindWhiteSegments(t->right, segs);		
}

void FindBiggestWhite(std::vector<Segment> &segs, int &begin, int &end)
{
	int maxLen = segs[0].Length();
	begin = segs[0].begin;
	end = segs[0].end;

	for(size_t i = 1; i < segs.size(); ++i)
	{
		if (segs[i].begin == segs[i-1].end)
			segs[i].begin = segs[i-1].begin;
		if (segs[i].Length() > maxLen)
		{
			begin = segs[i].begin;
			end = segs[i].end;
			maxLen = segs[i].Length();
		}
	}
}

int main()
{
	//freopen("C:\\temp\\in.txt", "r", stdin);
	srand(time(0));
	int n;
	int begin, end;
	char color;
	pTree t = new Tree(0, 1000000000, 1);
	std::cin >> n;
	for(size_t i = 0; i < n; ++i)
	{
		std::cin >> begin;
		std::cin >> end;
		std::cin >> color;
		CutTree(t, begin, end, color == 'w' ? 1 : 0);
	}

	std::vector<Segment> segs;
	FindWhiteSegments(t, segs);
	FindBiggestWhite(segs, begin, end);
	std::cout << begin << " " << end << std::endl;
	DeleteTree(t);
	//fclose(stdin);
}