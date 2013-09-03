/* Simple Event notifier implementation
 * There are three objects: with 0, 1 and 2 arguments in events
 * EventNotifier - 0 arguments
 * EventNotifier1 - 1 argument
 * EventNotifier2 - 2 arguments
 ************************************
 * Usage: 
 *	EventNotifier1<int> e;
 *	e += func1;
 *  e(15);
 *	e -= func1;
 ************************************
 *	+=  Adds a subscriber
 *	-=  Removes a subscriber
 *	()	Raises event and sends notification to subscribers
 */
#pragma once

#include<set>

class EventNotifier
{
private:
	typedef std::set<void (*)()> DelegateSet;
	DelegateSet m_Delegates;
public:
	EventNotifier(void) {}
	~EventNotifier(void) {}

	void operator+=(void (*)());
	void operator-=(void (*)());
	void operator()();
};


template<typename T = void>
class EventNotifier1
{
	private:
	typedef std::set<void (*)(T)> DelegateSet;
	DelegateSet m_Delegates;
public:
	EventNotifier1(void) {}
	~EventNotifier1(void) {}

	void operator+=(void (*)(T));
	void operator-=(void (*)(T));
	void operator()(T);
};


template<typename T1, typename T2>
class EventNotifier2
{
private:
	typedef std::set<void (*)(T1,T2)> DelegateSet;
	DelegateSet m_Delegates;
public:
	EventNotifier2(void) {}
	~EventNotifier2(void) {}

	void operator+=(void (*)(T1, T2));
	void operator-=(void (*)(T1, T2));
	void operator()(T1, T2);
};

// EventNotifier with no arguments
void EventNotifier::operator+=(void (*Delegate)())
{
	m_Delegates.insert(m_Delegates.end(), Delegate);
}

void EventNotifier::operator-=(void (*Delegate)())
{
	DelegateSet::iterator i = m_Delegates.find(Delegate);
	if (i != m_Delegates.end())
		m_Delegates.erase(i);
}

void EventNotifier::operator()()
{
	for(DelegateSet::iterator i = m_Delegates.begin(); i != m_Delegates.end(); ++i)
		(*i)();
}


// EventNotifier with one argument
template<typename T>
void EventNotifier1<T>::operator+=(void (*Delegate)(T))
{
	m_Delegates.insert(m_Delegates.end(), Delegate);
}

template<typename T>
void EventNotifier1<T>::operator-=(void (*Delegate)(T))
{
	DelegateSet::iterator i = m_Delegates.find(Delegate);
	if (i != m_Delegates.end())
		m_Delegates.erase(i);
}

template<typename T>
void EventNotifier1<T>::operator()(T arg)
{
	for(DelegateSet::iterator i = m_Delegates.begin(); i != m_Delegates.end(); ++i)
		(*i)(arg);
}


// EventNotifier with two arguments
template<typename T1, typename T2>
void EventNotifier2<T1, T2>::operator+=(void (*Delegate)(T1, T2))
{
	m_Delegates.insert(m_Delegates.end(), Delegate);
}

template<typename T1, typename T2>
void EventNotifier2<T1, T2>::operator-=(void (*Delegate)(T1, T2))
{
	DelegateSet::iterator i = m_Delegates.find(Delegate);
	if (i != m_Delegates.end())
		m_Delegates.erase(i);
}

template<typename T1, typename T2>
void EventNotifier2<T1, T2>::operator()(T1 arg1, T2 arg2)
{
	for(DelegateSet::iterator i = m_Delegates.begin(); i != m_Delegates.end(); ++i)
		(*i)(arg1, arg2);
}