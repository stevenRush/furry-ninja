#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>

template<typename Type>
class Node
{
	private:
	enum DummyType
	{
		dtNotADummy,
		dtEndDummy,
		dtReverseEndDummy,
		dtTreeEndDummy,
		dtTreeReverseEndDummy
	};

	Type value_;
	Node<Type>* first_;
	Node<Type>* last_;
	Node<Type>* parent_;
	Node<Type>*	next_;
	Node<Type>* previous_;

	template<typename NodeType, typename ValueType, int direction>
	class BasicTreeIterator;

	template<typename NodeType, typename ValueType, int direction>
	class BasicSiblingIterator : public std::iterator<std::bidirectional_iterator_tag, ValueType>
	{
	private:
		NodeType *pos_;
		DummyType dummyType_;

		explicit BasicSiblingIterator(NodeType* node, DummyType type = dtNotADummy)
		{
			pos_ = node;
			dummyType_ = type;
		}

		void forward()
		{
			if (dummyType_ == dtEndDummy)
				throw std::out_of_range("Index out of range!");
			if (dummyType_ == dtReverseEndDummy)
			{
				dummyType_ = dtNotADummy;
				return;
			}
			if (pos_->next_)
				pos_ = pos_->next_;
			else
			{
				if (direction < 0)
					throw std::out_of_range("Index out of range!");
				dummyType_ = dtEndDummy;
			}
		}

		void backward()
		{
			if (dummyType_ == dtReverseEndDummy)
				throw std::out_of_range("Index out of range!");
			if (dummyType_ == dtEndDummy)
			{
				dummyType_ = dtNotADummy;
				return;
			}
			if (pos_->previous_)
				pos_ = pos_->previous_;
			else
			{
				if (direction > 0)
					throw std::out_of_range("Index out of range!");
				dummyType_ = dtReverseEndDummy;
			}
		}

	public:

		BasicSiblingIterator()
		{ 
			pos_ = NULL;
			dummyType_ = direction > 0 ? dtEndDummy : dtReverseEndDummy;
		}
		
		BasicSiblingIterator(const BasicSiblingIterator<NodeType, ValueType, direction> &other) :
			pos_(other.pos_),
			dummyType_(other.dummyType_)
		{ 	}

		template<typename otherNodeType, typename otherValueType>
		BasicSiblingIterator(const BasicSiblingIterator<otherNodeType, otherValueType, direction> &other)
		{	
			pos_ = other.pos_;
			dummyType_ = other.dummyType_;
		}

		template<typename otherNodeType, typename otherValueType>
		BasicSiblingIterator(const BasicTreeIterator<otherNodeType, otherValueType, direction> &other)
		{
			assert(other.type_ == dtNotADummy);
			pos_ = other.pos_;
			dummyType_ = other.type_;
		}

		template<typename otherNodeType, typename otherValueType, int otherDirection>
		explicit BasicSiblingIterator(const BasicSiblingIterator<otherNodeType, otherValueType, otherDirection> &other)
		{
			assert(other.dummyType_ == dtNotADummy);
			pos_ = other.pos_;
			dummyType_ = other.dummyType_;
			if (direction != otherDirection)
				++*this;
		}

		template<typename otherNodeType, typename otherValueType, int otherDirection>
		explicit BasicSiblingIterator(const BasicTreeIterator<otherNodeType, otherValueType, otherDirection> &other)
		{
			assert(other.type_ == dtNotADummy);
			pos_ = other.pos_;
			dummyType_ = other.type_;
			if (direction != otherDirection)
				++*this;
		}

		BasicSiblingIterator& operator++() //prefix
		{
			if (direction < 0)
				backward();
			else
				forward();
			return *this;
		}

		BasicSiblingIterator operator++(int) //postfix
		{
			BasicSiblingIterator res(pos_);
			++*this;
			return res;
		}

		BasicSiblingIterator& operator--()
		{
			if (direction < 0)
				forward();
			else
				backward();
			return *this;
		}

		BasicSiblingIterator operator--(int)
		{
			BasicSiblingIterator res(pos_);
			--*this;
			return res;
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator==(const BasicSiblingIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			if (dummyType_ == dtNotADummy && rvalue.dummyType_ == dtNotADummy)
				return pos_ == rvalue.pos_;
			return dummyType_ == rvalue.dummyType_;
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator!=(const BasicSiblingIterator<otherNodeType, otherValueType, direction>& rvalue) const
		{
			return !operator==(rvalue);
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator==(const BasicTreeIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			return dummyType_ == rvalue.type_ && pos_ == rvalue.pos_;
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator!=(const BasicTreeIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			return !operator==(rvalue);
		}

		ValueType& operator*() const
		{
			if (dummyType_ != dtNotADummy)
				throw std::out_of_range("Illegal indirection!");
			return pos_->value_;
		}

		ValueType* operator->() const
		{
			if (dummyType_ != dtNotADummy)
				throw std::out_of_range("Illegal indirection!");
			return &(pos_->value_);
		}

		NodeType* Ptr() const
		{
			if (dummyType_ != dtNotADummy)
				return NULL;
			return pos_;
		}

		NodeType& Ref() const
		{
			if (dummyType_ != dtNotADummy)
				throw std::out_of_range("Illegal indirection!");
			return *pos_;
		}

		friend class Node;

	};

	template<typename NodeType, typename ValueType, int direction>
	class BasicTreeIterator : public std::iterator<std::bidirectional_iterator_tag, ValueType>
	{
	private:

		NodeType* pos_;
		DummyType type_;

		explicit BasicTreeIterator(NodeType* pos, DummyType type = dtNotADummy)
		{	
			pos_ = pos;
			type_ = type;
		}

		void forward()
		{
			if (type_ == dtTreeEndDummy)
				throw std::out_of_range("Index out of range!");
			if (type_ == dtTreeReverseEndDummy)
			{
				type_ = dtNotADummy;
				return;
			}
			if (pos_->first_)
			{
				pos_ = pos_->first_;
				return;
			}
			while(pos_->parent_ && !pos_->next_)
				pos_ = pos_->parent_;
			if (!pos_->parent_)
			{
				type_ = dtTreeEndDummy;
				return;
			}
			pos_ = pos_->next_;
		}

		void backward()
		{
			if (type_ == dtTreeReverseEndDummy)
				throw std::out_of_range("Index out of range!");
			if (type_ == dtTreeEndDummy)
			{
				type_ = dtNotADummy;
				while(pos_->last_)
					pos_ = pos_->last_;
				return;
			}
			if (!pos_->previous_)
			{
				if (pos_->parent_)
					pos_ = pos_->parent_;
				else
					type_ = dtTreeReverseEndDummy;
				return;
			}
			pos_ = pos_->previous_;
			while(pos_->last_)
				pos_ = pos_->last_;
		}

	public:
		BasicTreeIterator() :
			pos_(NULL),
			type_(dtTreeEndDummy)
		{	}

		BasicTreeIterator(const BasicTreeIterator<NodeType, ValueType, direction> &other) :
			pos_(other.pos_),
			type_(other.type_)
		{	}

		template<typename otherNodeType, typename otherValueType>
		BasicTreeIterator(const BasicTreeIterator<otherNodeType, otherValueType, direction> &other)
		{	
			pos_ = other.pos_;
			type_ = other.type_;
		}

		template<typename otherNodeType, typename otherValueType>
		BasicTreeIterator(const BasicSiblingIterator<otherNodeType, otherValueType, direction> &other)
		{
			assert(other.dummyType_ == dtNotADummy);
			pos_ = other.pos_;
			type_ = other.dummyType_;
		}

		template<typename otherNodeType, typename otherValueType, int otherDirection>
		explicit BasicTreeIterator(const BasicTreeIterator<otherNodeType, otherValueType, otherDirection> &other)
		{
			assert(other.type_ == dtNotADummy);
			pos_ = other.pos_;
			type_ = other.type_;
			if (direction != otherDirection)
				++*this;
		}

		template<typename otherNodeType, typename otherValueType, int otherDirection>
		explicit BasicTreeIterator(const BasicSiblingIterator<otherNodeType, otherValueType, otherDirection> &other)
		{
			assert(other.dummyType_ == dtNotADummy);
			pos_ = other.pos_;
			type_ = other.dummyType_;
			if (direction != otherDirection)
				++*this;
		}

		BasicTreeIterator& operator++() // prefix
		{
			if (direction > 0)
				forward();
			else
				backward();
			return *this;
		}

		BasicTreeIterator& operator--()
		{
			if (direction > 0)
				backward();
			else
				forward();
			return *this;
		}

		BasicTreeIterator operator++(int) //postfix
		{
			BasicTreeIterator res(*this);
			++*this;
			return res;
		}

		BasicTreeIterator operator--(int) //postfix
		{
			BasicTreeIterator res(*this);
			--*this;
			return res;
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator==(const BasicTreeIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			if (rvalue.type_ == dtNotADummy && type_ == dtNotADummy)
				return pos_ == rvalue.pos_;
			return type_ == rvalue.type_;
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator!=(const BasicTreeIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			return !operator==(rvalue);
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator==(const BasicSiblingIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			return type_ == rvalue.dummyType_ && pos_ == rvalue.pos_;
		}

		template<typename otherNodeType, typename otherValueType>
		bool operator!=(const BasicSiblingIterator<otherNodeType, otherValueType, direction> &rvalue) const
		{
			return !operator==(rvalue);
		}

		ValueType& operator*() const
		{
			if (type_ != dtNotADummy)
				throw std::out_of_range("Illegal indirection!");
			return pos_->value_;
		}

		NodeType* operator->() const
		{
			if (type_ != dtNotADummy)
				throw std::out_of_range("Illegal indirection!");
			return pos_;
		}

		NodeType* Ptr() const
		{
			if (type_ != dtNotADummy)
				return NULL;
			return pos_;
		}

		NodeType& Ref() const
		{
			if (type_ != dtNotADummy)
				throw std::out_of_range("Illegal indirection!");
			return *pos_;
		}

		friend class Node;
	};

public:
	typedef BasicSiblingIterator<Node<Type>, Type, 1> Iterator;
	typedef BasicSiblingIterator<const Node<Type>, const Type, 1> ConstIterator;
	typedef BasicSiblingIterator<Node<Type>, Type, -1> ReverseIterator;
	typedef BasicSiblingIterator<const Node<Type>, const Type, -1> ConstReverseIterator;

	typedef BasicTreeIterator<Node<Type>, Type, 1> TreeIterator;
	typedef BasicTreeIterator<const Node<Type>, const Type, 1> ConstTreeIterator;
	typedef BasicTreeIterator<Node<Type>, Type, -1> ReverseTreeIterator;
	typedef BasicTreeIterator<const Node<Type>, const Type, -1> ConstReverseTreeIterator;

private:
	void CopyRecursive(const Node<Type> &from)
	{
		for(Node<Type>::ConstIterator it = from.begin(); it != from.end(); ++it)
			PushBack(*it);
		Node<Type>::ConstIterator src = from.begin();
		for(Node<Type>::Iterator dst = begin(); dst != end(); ++dst, ++src)
			dst.Ref().CopyRecursive(src.Ref());
	}

	void PrintRecursive(std::ostream &stream, std::string prefix) const
	{
		stream << prefix << value_ << std::endl;
		prefix += " ";
		for(Node<Type>::ConstIterator cit = begin(); cit != end(); ++cit)
			cit.Ref().PrintRecursive(stream, prefix);
	}

	void LinkChild(const Iterator & where, Node<Type>* child)
	{
		child->parent_ = this;
		Node<Type> *pos = where.Ptr();
		if (pos == last_)
		{
			if (last_)
			{
				child->previous_ = last_;
				last_->next_ = child;
				last_ = child;
			}
			else
			{
				first_ = last_ = child;
				child->next_ = child->previous_ = NULL;
			}
		}
		else
		{
			child->previous_ = pos->previous_;
			if (pos->previous_)
				pos->previous_->next_= child;
			else
				first_ = child;
			pos->previous_ = child;
			child->next_ = pos;
		}
	}

	void Unlink(Node<Type>* pos)
	{
		if (pos->next_)
			pos->next_->previous_ = pos->previous_;
		if (pos->previous_)
			pos->previous_->next_ = pos->next_;
		if (first_ == pos)
			first_ = pos->next_;
		if (last_ == pos)
			last_ = pos->previous_;
		pos->parent_ = NULL;
	}

	ReverseTreeIterator MakeReverseStart(Node<Type>* initPos)
	{
		ReverseTreeIterator it(initPos);
		while(it.Ref().last_)
			it.pos_ = it.Ref().last_;
		return it;
	}

	bool IsValid(const Iterator & it) const
	{
		return it.dummyType_ == dtNotADummy;
	}

	void DeleteTree()
	{
		Node<Type>* temp = first_;
		while(temp)
		{
			Node<Type>* next = temp->next_;
			delete temp;
			temp = next;
		}
		first_ = NULL;
		last_ = NULL;
	}

public:
	Node(const Node<Type> &rvalue) :
		value_(rvalue.value_),
		parent_(NULL),
		next_(NULL),
		previous_(NULL),
		first_(NULL),
		last_(NULL)
//	Node(const Node<Type> &rvalue)
	{
		CopyRecursive(rvalue);
	}

	explicit Node(const Type & value = Type()) :
		first_(NULL),
		last_(NULL),
		parent_(NULL),
		next_(NULL),
		previous_(NULL),
		value_(value)
//  Node(Type value)
	{

	}

	~Node()
	{
		DeleteTree();
	}

	Iterator end()
	{
		return Iterator(last_, dtEndDummy);
	}

	ReverseIterator rend()
	{
		return ReverseIterator(first_, dtReverseEndDummy);
	}

	Iterator begin()
	{
		if (first_)
			return Iterator(first_);
		else
			return Iterator(first_, dtEndDummy);
	}

	ReverseIterator rbegin()
	{
		if (last_)
			return ReverseIterator(last_);
		else
			return ReverseIterator(last_, dtReverseEndDummy);
	}

	ConstIterator end() const
	{
		return ConstIterator(last_, dtEndDummy);
	}

	ConstReverseIterator rend() const
	{
		return ConstReverseIterator(first_, dtReverseEndDummy);
	}

	ConstIterator begin() const
	{
		if (first_)
			return ConstIterator(first_);
		else
			return ConstIterator(first_, dtEndDummy);
	}

	ConstReverseIterator rbegin() const
	{
		if (last_)
			return ConstReverseIterator(last_);
		else
			return ConstReverseIterator(last_, dtReverseEndDummy);
	}

	TreeIterator tbegin()
	{
		return TreeIterator(this);
	}

	TreeIterator tend()
	{
		return TreeIterator(this, dtTreeEndDummy);
	}

	ConstTreeIterator tbegin() const
	{
		return ConstTreeIterator(this);
	}

	ConstTreeIterator tend() const
	{
		return ConstTreeIterator(this, dtTreeEndDummy);
	}
	
	ReverseTreeIterator rtbegin()
	{
		return MakeReverseStart(const_cast<Node<Type>* >(this));
	}

	ReverseTreeIterator rtend()
	{
		return ReverseTreeIterator(this, dtTreeReverseEndDummy);
	}

	ConstReverseTreeIterator rtbegin() const
	{
		return MakeReverseStart(const_cast<Node<Type>* >(this));
	}

	ConstReverseTreeIterator rtend() const
	{
		return ConstReverseTreeIterator(this, dtTreeReverseEndDummy);
	}

	Type& back()
	{
		return last_->value_;
	}

	Type& front()
	{
		return first_->value_;
	}

	Iterator Insert(const Iterator & where, const Node<Type> & node)
	{
		if (!IsValid(where))
			throw std::out_of_range("Index out of range!");
		Node<Type> *newChild = new Node<Type>(node);
		LinkChild(where, newChild);
		return Iterator(newChild);
	}

	Iterator Insert(const Iterator & where, const Type & value)
	{
		if (!IsValid(where))
			throw std::out_of_range("Index out of range!");
		Node<Type> *newChild = new Node<Type>(value);
		LinkChild(where, newChild);
		return Iterator(newChild);
	}

	Iterator PushBack(const Node<Type> & node)
	{
		return Insert(--end(), node);
	}

	Iterator PushBack(const Type & value)
	{
		return Insert(--end(), value);
	}

	void Remove(const Iterator & where)
	{
		if (!IsValid(where))
			throw std::out_of_range("Out of range!");
		Node<Type> * pos = where.Ptr();
		Unlink(pos);
		delete pos;
	}

	Iterator MoveChild(const Iterator & where, Node<Type> & element)
	{
		if (this == &element)
			return Iterator(this);
		if (element.parent_)
			element.parent_->Unlink(&element);
		LinkChild(where, &element);
		return Iterator(&element);
	}

	Iterator MoveChildBack(Node<Type> & element)
	{
		return MoveChild(--end(), element);
	}

	const Node<Type> * GetParent() const
	{
		return parent_;
	}

	Node<Type> * GetParent()
	{
		return parent_;
	}

	Node<Type>& operator=(const Node<Type>& rvalue)
	{
		if (this == &rvalue)
			return *this;
		value_ = rvalue.value_;
		DeleteTree();
		CopyRecursive(rvalue);
		return *this;
	}

	template<typename T>
	friend std::ostream &operator<<(std::ostream &stream, const Node<T> &node);

	bool operator==(const Node<Type> &other) const
	{
		if (value_ != other.value_)
			return false;
		Node<Type>::ConstIterator otherIt, it;
		for(it = begin(), otherIt = other.begin(); otherIt != other.end() && it != end(); ++it, ++otherIt)
		{
			if (*it != *otherIt)
				return false;
		}
		if (otherIt != end() || it != end())
			return false;
		return true;
	}

	bool operator!=(const Node<Type> &other) const
	{
		return !operator==(other);
	}

	void Swap(Node<Type> &other)
	{
		for(Node<int>::Iterator it = begin(); it != end(); ++it)
			it.Ref().parent_ = &other;
		for(Node<int>::Iterator it = other.begin(); it != other.end(); ++it)
			it.Ref().parent_ = this;
		std::swap(first_, other.first_);
		std::swap(last_, other.last_);
		std::swap(value_, other.value_);
	}
};

template<typename Type>
std::ostream &operator<<(std::ostream &stream, const Node<Type> &node)
{
	node.PrintRecursive(stream, "");
	return stream;
}

namespace std
{
	template<typename Type>
	void swap(Node<Type> & left, Node<Type> & right)
	{
		left.Swap(right);
	}
}
