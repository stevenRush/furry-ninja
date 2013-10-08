#include "Tree.h"
#include <iostream>
#include <vector>

using namespace std;

class TestClass
{
private:
	int m_Value;
public:
	static bool isDeleteLogging;

	TestClass(int value) :
		m_Value(value)
	{	}

	TestClass() 
	{	}

	~TestClass()
	{
		if (isDeleteLogging)
			cout << "Destroying TestClass object with value = " << m_Value << endl;
	}
private:
	friend std::ostream &operator<<(std::ostream &stream, const TestClass & object);
};

std::ostream &operator<<(std::ostream &stream, const TestClass & object)
{
	stream << object.m_Value;
	return stream;
}

bool TestClass::isDeleteLogging = false;

#define CHECK_THROWS(statement) try { statement } catch (const std::out_of_range & e) { cout << e.what() << endl; }

int main()
{
	string delim = "------------------------------------------------\n";
	Node<int> a(0);
	for(size_t i = 1; i <= 3; ++i)
		a.PushBack(i);
	for(Node<int>::Iterator it = a.begin(); it != a.end(); ++it)
		it.Ref().PushBack(*it * 10);
	Node<int> b = a;
	cout << "Main: \n";
	{
		cout << "Tree: \n" << a;
	}
	cout << delim << "Child Iterators: \n";
	{
		cout << "Child Iterator: (prefix increment)" << endl;
		for(Node<int>::Iterator it = a.begin(); it != a.end(); ++it)
			cout << *it << " ";
		cout << endl << "Child ConstIterator: (postfix increment)" << endl;
		for(Node<int>::ConstIterator cit = a.begin(); cit != a.end(); cit++)
			cout << *cit << " ";
		cout << endl << "Child ReverseIterator: " << endl;
		for(Node<int>::ReverseIterator rit = a.rbegin(); rit != a.rend(); ++rit)
			cout << *rit << " ";
		cout << endl << "Child ConstReverseIterator: " << endl;
		for(Node<int>::ConstReverseIterator crit = a.rbegin(); crit != a.rend(); ++crit)
			cout << *crit << " ";
		cout << endl << "Child Iterator: (prefix decrement)" << endl;
		Node<int>::Iterator it = a.end();
		do
		{
			--it;
			cout << *it << " ";
		} while(it != a.begin());
		cout << endl << "Child ConstIterator: (postfix decrement)" << endl;
		Node<int>::ConstIterator cit = a.end();
		do
		{
			cit--;
			cout << *cit << " ";
		} while (cit != a.begin());
		cout << endl << "Child ReverseIterator (decrement): " << endl;
		Node<int>::ReverseIterator rit = a.rend();
		do
		{
			--rit;
			cout << *rit << " ";
		} while(rit != a.rbegin());
		cout << endl << "Child ConstReverseIterator (decrement): " << endl;
		Node<int>::ConstReverseIterator crit = a.rend();
		do
		{
			crit--;
			cout << *crit << " ";
		} while (crit != a.rbegin());
		cout << endl;
	}
	cout << delim << "Tree Iterators: \n";
	{
		int c = 0;
		Node<int> a(c++);
		for(size_t i = 0; i < 5; ++i)
		{
			Node<int>::Iterator it = a.PushBack(c++);
			for(size_t j = 0; j < 5; ++j)
			{
				Node<int>::Iterator jt = it.Ref().PushBack(c++);
				for(size_t k = 0; k < 5; ++k)
					jt.Ref().PushBack(c++);
			}
		}
		cout << "Tree before iterators' work: \n" << a;
		cout << "\nTreeIterator(prefix increment): increasing every element by 1..." << endl;
		for(Node<int>::TreeIterator it = a.tbegin(); it != a.tend(); ++it)
			++*it;
		cout << "\nConstTreeIterator(prefix increment):" << endl;
		for(Node<int>::ConstTreeIterator cit = a.tbegin(); cit != a.tend(); ++cit)
			cout << *cit << " ";
		cout << "\n\nTreeIterator(postfix increment): increasing every element by 2..." << endl;
		for(Node<int>::TreeIterator it = a.tbegin(); it != a.tend(); it++)
			*it += 2;
		cout << endl << "ConstTreeIterator(postfix increment):" << endl;
		for(Node<int>::ConstTreeIterator cit = a.tbegin(); cit != a.tend(); cit++)
			cout << *cit << " ";
		cout << endl << "\nReverseTreeIterator(prefix increment): increasing every element by 1..." << endl;
		for(Node<int>::ReverseTreeIterator rit = a.rtbegin(); rit != a.rtend(); ++rit)
			++*rit;
		cout << endl << "ConstReverseTreeIterator(prefix increment): " << endl;
		for(Node<int>::ConstReverseTreeIterator crit = a.rtbegin(); crit != a.rtend(); ++crit)
			cout << *crit << " ";
		cout << endl << "\nReverseTreeIterator(postfix increment): increasing every element by 1..." << endl;
		for(Node<int>::ReverseTreeIterator rit = a.rtbegin(); rit != a.rtend(); rit++)
			++*rit;
		cout << endl << "ConstReverseTreeIterator(postfix increment): " << endl;
		for(Node<int>::ConstReverseTreeIterator crit = a.rtbegin(); crit != a.rtend(); crit++)
			cout << *crit << " ";
		cout << "\n\nTreeIterator(prefix decrement): increasing every element by 1..." << endl;
		for(Node<int>::TreeIterator it = --a.tend(); it != --a.tbegin(); --it)
			++*it;
		cout << "\nConstTreeIterator(postfix decrement):" << endl;
		for(Node<int>::ConstTreeIterator cit = --a.tend(); cit != --a.tbegin(); cit--)
			cout << *cit << " ";
		cout << endl;
	}
	cout << delim << "Children addition: \n";
	{
		cout << "Before: \n" << a;
		cout << "Adding 3 children with value = 15 to tree..." << endl;
		for(size_t i = 0; i < 3; ++i)
			a.PushBack(15 + i);
		cout << "After: \n" << a;
		cout << "Adding children to the middle... " << endl;
		Node<int>::Iterator it = a.begin();
		advance(it, 2);
		a.Insert(it, 1555);
		cout << "After: \n" << a;
		cout << "Adding children to begining..." << endl;
		a.Insert(a.begin(), 144);
		a.begin().Ref().PushBack(1444);
		cout << "After: \n" << a;
	}
	cout << delim << "Children removing: \n";
	{
		cout << "Before: \n" << a;
		cout << "Removing 3 last children... " << endl;
		for(size_t i=0; i < 3; ++i)
			a.Remove(--a.end());
		cout << "After: \n" << a;
		cout << "Removing child from the middle...\n";
		Node<int>::Iterator it = a.begin();
		advance(it, 3);
		a.Remove(it);
		cout << "After:\n" << a;
		cout << "Removing all 3-level nodes... " << endl;
		for(Node<int>::Iterator it = a.begin(); it != a.end(); ++it)
			it.Ref().Remove(it.Ref().begin());
		cout << "After: \n" << a;
	}
	cout << delim << "Tree copying and moving: \n";
	{
		cout << "Before: \nb:\n" << b;
		cout << "a: \n" << a;
		cout << "Copying a to b..." << endl;
		b.PushBack(a);
		cout << "b after copying: \n" << b;
		cout << "Move last b's child to a... " << endl;
		a.MoveChildBack(b.rbegin().Ref());
		cout << "After moving: \na:\n" << a << "b:\n" << b;
		b.PushBack(45);
		b.PushBack(46);
		cout << "Before: \nb:\n" << b;
		cout << "a: \n" << a;
		cout << "Move b's last children to begining of a and to the middle of a..." << endl;
		a.MoveChild(a.begin(), b.rbegin().Ref());
		Node<int>::Iterator it = a.begin();
		advance(it, 2);
		a.MoveChild(it, b.rbegin().Ref());
		cout << "After moving: \na:\n" << a << "b:\n" << b;
	}
	cout << delim << "Tree comparison: \n";
	{
		cout << "a: \n" << a << "b: \n" << b;
		cout << "a == b: " << (a == b ? "True" : "False") << endl;
		cout << "a != b: " << (a != b ? "True" : "False") << endl;
		cout << "Creating copy of a..." << endl;
		Node<int> aCopy = a;
		cout << "a: \n" << a << "copy of a: \n" << aCopy;
		cout << "a == copy: " << (a == aCopy ? "True" : "False") << endl;
		cout << "a != copy: " << (a != aCopy ? "True" : "False") << endl;
	}
	cout << delim << "Tree swap: \n" << endl;
	{
		Node<int> a(0);
		Node<int> b(1);
		for(size_t i = 0; i < 5; ++i)
		{
			a.PushBack(0);
			b.PushBack(1);
		}
		cout << "a: \n" << a << "b: \n" << b;
		cout << "Saving iterators...\n";
		Node<int>::Iterator ait = a.begin();
		Node<int>::Iterator bit = b.begin();
		cout << "Swapping a and b..." << endl;
		std::swap(a, b);
		cout << "After swap: \n";
		cout << "a: \n" << a << "b: \n" << b;
		cout << "Iterator from a: " << endl;
		for(; ait != b.end(); ++ait)
			cout << *ait << " ";
		cout << endl;
		cout << "Iterator from b: " << endl;
		for(; bit != a.end(); ++bit)
			cout << *bit << " ";
		cout << endl;
	}
	cout << delim << "Iterator comparison: \n";
	{
		Node<int>::TreeIterator tit = a.tbegin();
		Node<int>::Iterator it = a.begin();
		// TreeIterator starts walking from the root and then goes to first child of root, so:
		cout << "begin() != tbegin(): " << (it != tit ? "TRUE" : "FALSE") << endl;
		++tit;
		cout << "begin() == ++tbegin(): " << (it == tit ? "TRUE" : "FALSE") << endl;

		Node<int>::ConstIterator cit = a.begin();
		it = a.begin();
		cout << "ConstIterator = begin() == Iterator = begin(): " << (it == cit ? "TRUE" : "FALSE") << endl;
		cout << "ConstIterator = begin() != Iterator = begin(): " << (it != cit ? "TRUE" : "FALSE") << endl;
	}
	cout << delim << "Some iterator magic: \n";
	{
		Node<int>::Iterator it = ++a.begin();
		cout << "it == ++--it: " << (it == ++--it ? "TRUE" : "FALSE") << endl;
		cout << "it == --++it: " << (it == --++it ? "TRUE" : "FALSE") << endl;
		Node<int>::TreeIterator tit = a.tbegin();
		cout << "tit == ++--tit: " << (tit == ++--tit ? "TRUE" : "FALSE") << endl;
		cout << "tit == --++tit: " << (tit == --++tit ? "TRUE" : "FALSE") << endl;
	}
	cout << delim << "Iterator assignment: \n";
	{
		Node<int>::Iterator it = a.begin();
		Node<int>::ConstIterator cit = a.begin();
		Node<int>::TreeIterator tit = a.tbegin();
		Node<int>::ConstTreeIterator ctit = a.tbegin();
		cout << "Assigning Iterator to Iterator... \n";
		Node<int>::Iterator it1 = it;
		cout << "it = " << *it << " it1 = " << *it1 << endl;
		cout << "Assigning Iterator to ConstIterator... \n";
		Node<int>::ConstIterator it2 = it;
		cout << "it = " << *it << " it2 = " << *it2 << endl;
		cout << "Assigning Iterator to ReverseIterator... \n";
		Node<int>::ReverseIterator it3 (it);
		cout << "it = " << *it << " --it3 = " << *--it3 << endl;
		cout << "Assigning Iterator to ConstReverseIterator... \n";
		Node<int>::ConstReverseIterator it4(it);
		cout << "it = " << *it << " --it4 = " << *--it4 << endl;
		cout << "Assigning Iterator to TreeIterator... \n";
		Node<int>::TreeIterator it5 = it;
		cout << "it = " << *it << " it5 = " << *it5 << endl;
		cout << "Assigning Iterator to ConstTreeIterator... \n";
		Node<int>::ConstTreeIterator it6 = it;
		cout << "it = " << *it << " it6 = " << *it6 << endl;
		cout << "Assigning Iterator to ReverseTreeIterator... \n";
		Node<int>::ReverseTreeIterator it7 (it);
		cout << "it = " << *it << " --it7 = " << *--it7 << endl;
		cout << "Assigning Iterator to ConstReverseTreeIterator...\n\n";
		Node<int>::ConstReverseTreeIterator it8(it);
		cout << "it = " << *it << " --it8 = " << *--it8 << endl;

		cout << "Assigning TreeIterator to Iterator... \n";
		Node<int>::Iterator it9 = tit;
		cout << "tit = " << *tit << " it9 = " << *it9 << endl;
		cout << "Assigning TreeIterator to ConstIterator... \n";
		Node<int>::ConstIterator it10 = tit;
		cout << "tit = " << *tit << " it10 = " << *it10 << endl;
		cout << "Assigning TreeIterator to ReverseIterator... \n";
		Node<int>::ReverseIterator it11(tit);
		cout << "tit = " << *tit << " --it11 = " << *--it11 << endl;
		cout << "Assigning TreeIterator to ConstReverseIterator... \n";
		Node<int>::ConstReverseIterator it12(tit);
		cout << "tit = " << *tit << " --it12 = " << *--it12 << endl;
		cout << "Assigning TreeIterator to TreeIterator... \n";
		Node<int>::TreeIterator it13 = tit;
		cout << "tit = " << *tit << " it13 = " << *it13 << endl;
		cout << "Assigning TreeIterator to ConstTreeIterator... \n";
		Node<int>::ConstTreeIterator it14 = tit;
		cout << "tit = " << *tit << " it14 = " << *it14 << endl;
		cout << "Assigning TreeIterator to ConstTreeIterator... \n";
		Node<int>::ConstTreeIterator it15 = tit;
		cout << "tit = " << *tit << " it15 = " << *it15 << endl;
		cout << "Assigning TreeIterator to ConstReverseTreeIterator...\n\n";
		Node<int>::ConstReverseTreeIterator it16(tit);
		cout << "tit = " << *tit << " --it16 = " << *--it16 << endl;

		cout << "Assigning ConstIterator to ConstIterator...\n";
		Node<int>::ConstIterator it17 = cit;
		cout << "cit = " << *cit << " it17 = " << *it17 << endl;
		cout << "Assigning ConstIterator to ConstReverseIterator\n";
		Node<int>::ConstReverseIterator it18(cit);
		cout << "cit = " << *cit << " --it18 = " << *--it18 << endl;
		cout << "Assigning ConstIterator to ConstTreeIterator...\n";
		Node<int>::ConstTreeIterator it19 = cit;
		cout << "cit = " << *cit << " it19 = " << *it19 << endl;
		cout << "Assigning ConstIterator to ConstReverseTreeIterator...\n\n";
		Node<int>::ConstReverseTreeIterator it20(cit);
		cout << "cit = " << *cit << " --it20 = " << *--it20 << endl;

		cout << "Assigning ConstTreeIterator to ConstIterator...\n";
		Node<int>::ConstIterator it21 = ctit;
		cout << "ctit = " << *ctit << " it21 = " << *it21 << endl;
		cout << "Assigning ConstTreeIterator to ConstReverseIterator\n";
		Node<int>::ConstReverseIterator it22(ctit);
		cout << "ctit = " << *ctit << " --it22 = " << *--it22 << endl;
		cout << "Assigning ConstTreeIterator to ConstTreeIterator...\n";
		Node<int>::ConstTreeIterator it23 = ctit;
		cout << "ctit = " << *ctit << " it23 = " << *it23 << endl;
		cout << "Assigning ConstTreeIterator to ConstReverseTreeIterator...\n";
		Node<int>::ConstReverseTreeIterator it24(ctit);
		cout << "ctit = " << *ctit << " --it24 = " << *--it24 << endl;
	}
	cout << delim << "Exceptions testing: " << endl;
	{
		// wont compile
		//it = cit;
		//cit.Ref().PushBack(3);
		//tit = ctit;
		//it = ctit;
		//tit = cit;
		//ctit.Ref().PushBack(5);

		const Node<int> c(4);
		//c.PushBack(5);
		//Node<int>::Iterator constIt = c.begin();
		//Node<int>::Iterator constIt = c.end();

		// will cause an exception
		Node<int>::ReverseIterator it25 = a.rend();
		Node<int>::Iterator it26 = a.end();
		CHECK_THROWS(cout << *it25 << endl;)
		CHECK_THROWS(++it25;)
		CHECK_THROWS(cout << *it26 << endl;)
		CHECK_THROWS(++it26;)
		CHECK_THROWS(--a.begin();)
		CHECK_THROWS(--a.rbegin();)
		CHECK_THROWS(++a.end();)
		CHECK_THROWS(++a.rend();)
		CHECK_THROWS(Node<int> ref = a.end().Ref();)
	}
	cout << delim << "Memory management: \n";
	{
		int c = 0;
		Node<TestClass> a(TestClass(c++));
		for(size_t i = 0; i < 2; ++i)
		{
			Node<TestClass>::Iterator it = a.PushBack(TestClass(c++));
			for(size_t j = 0; j < 2; ++j)
			{
				Node<TestClass>::Iterator jt = it.Ref().PushBack(TestClass(c++));
				for(size_t k = 0; k < 2; ++k)
					jt.Ref().PushBack(TestClass(c++));
			}
		}
		cout << "Tree: \n" << a;
		cout << "Everything is deleting!" << endl;
		TestClass::isDeleteLogging = true;
		cout << "Deleting subtree..." << endl;
		a.begin().Ref().Remove(a.begin().Ref().begin());
		cout << "Deleting whole tree..." << endl;
	}

	return 0;
}
