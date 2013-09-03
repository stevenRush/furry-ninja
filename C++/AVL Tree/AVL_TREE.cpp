#include <algorithm>
#include <iostream>
#include <vector>

struct Node
{
	int Key;
	size_t Height;
	size_t Size;
	Node* Left;
	Node* Right;

	Node(int k)
	{
		Key = k;
		Left = Right = 0;
		Height = 1;
		Size = 1;
	}
};

typedef Node* pNode;
typedef Node* Handle;

class AVLTree
{
private:
	pNode mainNode;

	static size_t Height(pNode p)
	{
		return p ? p->Height : 0;
	}

	static size_t BalanceFactor(pNode p)
	{
		if (!p)
			return 0;
		return Height(p->Right) - Height(p->Left);
	}

	static void FixHeight(pNode p)
	{
		size_t l = Height(p->Left);
		size_t r = Height(p->Right);

		p->Height = std::max(l, r) + 1;
	}

	static size_t Size(pNode p)
	{
		return p ? p->Size : 0;
	}

	static void Recalc(pNode p)
	{
		if (!p)
			return;
		p->Size = Size(p->Left) + Size(p->Right) + 1;
	}

	static void RotateLeft(pNode& p)
	{
		pNode a = p;
		pNode b = p->Right;
		pNode L = a->Left;
		pNode C = b->Left;
		pNode R = b->Right;
		a->Left = L;
		a->Right = C;
		b->Left = a;
		b->Right = R;
		FixHeight(a);
		Recalc(a);
		FixHeight(b);
		Recalc(b);
		p = b;
	}

	static void RotateRight(pNode& p)
	{
		pNode a = p;
		pNode b = a->Left;
		pNode R = a->Right;
		pNode L = b->Left;
		pNode C = b->Right;
		a->Left = C;
		a->Right = R;
		b->Left = L;
		b->Right = a;
		FixHeight(a);
		Recalc(a);
		FixHeight(b);
		Recalc(b);
		p = b;
	}

	static void Balance(pNode& p)
	{
		if (!p)
			return;
		FixHeight(p);
		if (BalanceFactor(p) == 2)
		{
			if (BalanceFactor(p->Right) < 0)
				RotateRight(p->Right);
			RotateLeft(p);
		}

		if (BalanceFactor(p) == -2)
		{
			if (BalanceFactor(p->Left) > 0)
				RotateLeft(p->Left);
			RotateRight(p);
		}
	}

	static pNode raw_Insert(pNode p, int K, Handle& h)
	{
		if (!p) return h = new Node(K);

		if (K < p->Key)
			p->Left = raw_Insert(p->Left, K, h);
		else
			p->Right = raw_Insert(p->Right, K, h);
		Balance(p);
		return p;
	}	

	static void raw_Print(pNode p)
	{
		if (!p)
			return;
		raw_Print(p->Left);
		std::cout << p->Key << " ";
		raw_Print(p->Right);
	}

	static void DeleteTree(pNode p)
	{
		if (!p)
			return;
		DeleteTree(p->Left);
		DeleteTree(p->Right);
		delete p;
	}

	static void FindMax(pNode& max, pNode& max_parent)
	{
		max_parent = NULL;
		while(max->Right)
		{
			max_parent = max;
			max = max->Right;
		}
	}

	static void FindMin(pNode& min, pNode& min_parent)
	{
		min_parent = NULL;
		while(min->Left)
		{
			min_parent = min;
			min = min->Left;
		}
	}

	static pNode raw_GetElementByIndex(pNode t, size_t K)
	{
		if (!t)
			return NULL;

		size_t curIndex = Size(t->Left) + 1;
		if (curIndex < K)
			return raw_GetElementByIndex(t->Right, K-curIndex);
		
		if (curIndex > K)
			return raw_GetElementByIndex(t->Left, K);

		return t;
	}

	static pNode Find(pNode p, int K)
	{
		if (!p)
			return NULL;

		if (K < p->Key)
			return Find(p->Left, K);
		if (K > p->Key)
			return Find(p->Right, K);
		return p;
	}

	static void raw_Remove(pNode& t, pNode p, pNode del_parent)
	{
		if (p->Key < t->Key)
		{
			raw_Remove(t->Left, p, t);
			Balance(t);
		}
		else if (p->Key > t->Key)
		{
			raw_Remove(t->Right, p, t);
			Balance(t);
		}

		if (p->Key == t->Key)
		{
			if (!del_parent && !t->Left && !t->Right)	// Если удаляем корень 
			{
				delete t;
				t = NULL;
				return;
			}

			// Если левое поддерево длиннее, то берем из него элемент на замену
			if (Height(p->Left) > Height(p->Right))
			{
				pNode	rem = p->Left,			// Элемент на замену
						rem_parent;				// Родитель элемента на замену
				FindMax(rem, rem_parent);	
				// Если у удаляемого элемента всего один сын - левый
				if (!rem_parent)
				{
					t = rem;
				}
				else
				{
					rem_parent->Right = NULL;
					rem->Right = p->Right;
					rem->Left = p->Left;
					t = rem;
				}
			}
			else
			{
				// Если удаляемый узел - лист
				if (p->Left == p->Right && !p->Left)
				{
					t = NULL;
					return;
				}

				pNode	rem = p->Right,			// Элемент на замену 
						rem_parent;				// Родитель элемента на замену
				FindMin(rem, rem_parent);

				// Если у удаляемого узла один сын - правый
				if (!rem_parent)
				{
					t = rem;
				}
				else
				{
					t = rem;
					rem->Left = p->Left;
					rem->Right = p->Right;
					rem_parent->Left = NULL;
				}
			}
		}
	}

public:
	AVLTree()
	{
		mainNode = NULL;
	}

	Handle Insert(int K)
	{
		Handle h;
		mainNode = raw_Insert(mainNode, K, h);
		return h;
	}

	int& GetElementByIndex(size_t K)
	{
		pNode p = raw_GetElementByIndex(mainNode, K);
		return p->Key;
	}

	int& GetValue(Handle h)
	{
		return h->Key;
	}

	void PrintKvalue(size_t k)
	{
		pNode p = raw_GetElementByIndex(mainNode, k);
		if (p)
			std::cout << p->Key;
		else
			std::cout << "NONE";
		std::cout << std::endl;
	}

	void Remove(Handle h)
	{
		if (!Find(mainNode, h->Key))
			return;
		raw_Remove(mainNode, h, NULL);	
	}

	void Print()
	{
		raw_Print(mainNode);
		std::cout << std::endl;
	}

	~AVLTree()
	{
		DeleteTree(mainNode);
	}
};

int main()
{
	AVLTree* t = new AVLTree();
	std::vector<Handle> v;
	int n = 15;
	for(size_t i=0; i < n; ++i)
	{
		v.push_back(t->Insert(i));
		t->PrintKvalue(1);
	}
	std::cout << std::endl;
	for(size_t i=0; i < n; ++i)
	{
		t->Remove(v[i]);
	}
	delete t;
	return 0;
}