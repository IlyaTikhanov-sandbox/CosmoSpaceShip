#pragma once

/*
	 -------------------------------------------------------------------
	|                                                                   |
	|    Stack Class                                                    |
	|    ===========================================================    |
	|    This Stack has been implemented with templates to allow it     |
	|    to accomodate virtually any data type, and the size of the     |
	|    Stack is determined dynamically at runtime.                    |
	|                                                                   |
	|    There is also a new function: peek(), which, given a whole     |
	|    number 'Depth', returns the Stack element which is 'Depth'     |
	|    levels from the top.                                           |
	|                                                                   |
	 -------------------------------------------------------------------
*/

#include <assert.h>

//-------------------------------------------------
// Main structure of Stack Class:
//-------------------------------------------------

template <class Elem>
class Stack
{
public:
	Stack(int MaxSize = 10);
	Stack(const Stack<Elem> &OtherStack);
	~Stack(void);

	void        push(const Elem &Item); // Adds Item to the top
	inline Elem        pop(void);              // Returns Item from the top
	inline const Elem &peek(int Depth) const;  // Peek a depth downwards
	inline Elem const checkTop() const;
	inline bool empty();
	void clearStack();
	int GetCurrElem() const;

protected:
	Elem     *Data;           // The actual Data array
	int       CurrElemNum;    // The current number of elements
	const int MAX_NUM;        // Maximum number of elements
};

//-------------------------------------------------
// Implementation of Stack Class:
//-------------------------------------------------

// Stack Constructor function
template <class Elem>
Stack<Elem>::Stack(int MaxSize) :
	MAX_NUM(MaxSize)    // Initialize the constant
{
	Data = new Elem[MAX_NUM];
	CurrElemNum = 0;
}

// Stack Destructor function
template <class Elem>
Stack<Elem>::~Stack(void)
{
	delete[] Data;
}

// Push() function
template <class Elem>
inline void Stack<Elem>::push(const Elem &Item)
{
	// Error Check: Make sure we aren't exceeding the maximum storage space
	if (CurrElemNum < MAX_NUM)
	{
		Data[CurrElemNum++] = Item;
	}
	else
	{
		for (int i = 0; i < MAX_NUM - 1; ++i)
		{
			Data[i] = Data[i + 1];
		}
		Data[CurrElemNum] = Item;
	}
}

// Pop() function
template <class Elem>
inline Elem Stack<Elem>::pop(void)
{
	// Error Check: Make sure we aren't popping from an empty Stack
	assert(CurrElemNum > 0);

	return Data[--CurrElemNum];
}

// Peek() function
template <class Elem>
inline const Elem &Stack<Elem>::peek(int Depth) const
{
	// Error Check: Make sure the depth doesn't exceed the number of elements
	assert(Depth < CurrElemNum);

	return Data[CurrElemNum - (Depth + 1)];
}

template<class Elem>
inline Elem const Stack<Elem>::checkTop() const
{
	return Data[CurrElemNum - 1];
}

template<class Elem>
inline bool Stack<Elem>::empty()
{
	return CurrElemNum == 0 ? 1 : 0;
}

template<class Elem>
inline void Stack<Elem>::clearStack()
{
	CurrElemNum = 0;
}

template<class Elem>
inline int Stack<Elem>::GetCurrElem() const
{
	return CurrElemNum;
}
