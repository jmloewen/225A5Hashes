// File:        slinkedlist.h
// Author:      Geoffrey Tien
// Date:        March 26, 2016
// Description: Definition of a template singly-linked-list class for CMPT 225 assignment 5

#ifdef _SLINKEDLIST_H_

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

//a slinkedlistcontains a node pointer to 'front', 'back', and an integer 'size'.
//a node contains 'data' of type Template and a pointer to the 'next' node.


// default constructor
template <class T>
SLinkedList<T>::SLinkedList()
{
	front = NULL;
	back = NULL;
	size = 0;
}

// copy constructor, performs deep copy of list elements
template <class T>
SLinkedList<T>::SLinkedList(const SLinkedList<T>& ll)
{
	if (ll.Size() == 0)
	{
		front = NULL;
		back = NULL;
		size = 0;
		return;
	}
	//here, size > 0.
	size = ll.Size();
	CopyList(ll);
}

// helper function for deep copy
// Used by copy constructor and operator=
template <class T>
void SLinkedList<T>::CopyList(const SLinkedList<T>& ll)
{

	Node<T> * temp = new Node<T>(ll.front->data);
	Node<T> * dpr = ll.front->next;	//dpr/datapointer points to the next value.
	front = temp;
	back = temp;	//this is required here in case we only have one value in the LL.
	
	while (dpr != NULL)
	{
		temp->next = new Node<T>(dpr->data);
		temp = temp->next;
		back = temp;
		dpr = dpr->next;
	}
	//front, back, and size are all now set.  We're done.
}

// helper function for deep delete
// Used by destructor and copy/assignment
template <class T>
void SLinkedList<T>::DeleteList()
{
	if (Size() == 0)
	{
		return;
	}
	//else, a list exists.  Let's get rid of it.

	Node<T> * temp = front;
	Node<T> * iter = front->next;

	while (iter != NULL)
	{
		delete temp;
		temp = iter;
		iter = iter->next;
	}
	front = NULL;
	back = NULL;
	size = 0;
}

// destructor
template <class T>
SLinkedList<T>::~SLinkedList()
{
	if (size > 0)
	{
		//currently seems broken somehow.
		Node<T> * curNode = front;
		Node<T> * tempNode = front;
		while (curNode != NULL)
		{
			tempNode = tempNode->next;
			delete curNode;
			curNode = tempNode;
		}
		front = NULL;
		back = NULL;
		size = 0;
	}
}

// MUTATORS

// Inserts an item at the front of the list
// POST:  List contains item at front
// PARAM: item = item to be inserted
template <class T>
void SLinkedList<T>::InsertFront(T item)
{
	Node<T> * newNode = new Node<T>(item);

	if (size == 0)
	{
		back = newNode;
	}

	newNode->next = front;
	front = newNode;
	size++;
}

// Inserts an item at the back of the list
// POST:  List contains item at back
// PARAM: item = item to be inserted
template <class T>
void SLinkedList<T>::InsertBack(T item)
{
	Node<T> * newNode = new Node<T>(item);

	if (size == 0)
	{
		front = newNode;
		back = newNode;
	}
	else
	{
		back->next = newNode;
		back = newNode;
	}
	size++;
}

// Removes the first occurrence of the supplied parameter
// Removes and returns true if found, otherwise returns false if parameter is not found or list is empty
template <class T>
bool SLinkedList<T>::Remove(T item)
{
	//by the time we're here, the hashing is complete.  Search within the linked list for our item.

	if (!Contains(item))
	{
		return false;
	}

	//item exists, remove it.
	Node<T> * temp = front;
	Node<T> * prev = NULL;
	size--;
	while (temp != NULL)
	{
		//item has been found
		if (temp->data == item)
		{
			//if prev is null, the item is the first item in our list.
			if (prev == NULL)
			{
				prev = front->next;
				front = prev;
			}
			//if the next value is null, we're at the end of the list.
			else if (temp->next == NULL)
			{
				
				back = prev;
				back->next = NULL;
			}
			else
			{
				//else, we're in the middle somewhere.
				prev->next = temp->next;
			}
			delete temp;
			return true;
		}
		prev = temp;
		temp = temp->next;
	}
	//we should never get here.  Getting here means the item exists but we somehow didnt find it.
	bool bad = true;
	size++;
	return false;
}

// Removes all items in the list
template <class T>
void SLinkedList<T>::RemoveAll()
{
	if (front == NULL)
	{
		return;
	}
	Node<T> * cur = front;
	Node<T> * temp = front->next;
	while (size > 0)
	{
		delete cur;
		cur = temp;
		if (temp != NULL)
		{
			temp = temp->next;
		}
		size--;
	}
	front = NULL;
	back = NULL;
}

// ACCESSORS

// Returns size of list
template <class T>
int SLinkedList<T>::Size() const
{
	return size;
}

// Returns whether the list is empty
template <class T>
bool SLinkedList<T>::IsEmpty() const
{
	if (size > 0)
	{
		return false;
	}
	return true;
}

// Returns existence of item
template <class T>
bool SLinkedList<T>::Contains(T item) const
{
	if (size == 0)
	{
		return false;
	}

	Node<T> * temp = front;

	//temp is not null, and there exists a next item.
	while (temp->next != NULL)
	{
		if (temp->data == item)
		{
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// Returns a pointer to the in-place list item or NULL if item not found
template <class T>
T* SLinkedList<T>::Retrieve(T item)
{
	//by the time we're here, the hashing is complete.  Search within the linked list for our item.

	if (!Contains(item))
	{
		return NULL;
	}

	//item exists, remove it.
	Node<T> * temp = front;
	while (temp != NULL)
	{
		//item has been found
		if (temp->data == item)
		{
			return &temp->data;
		}
		temp = temp->next;
	}
	//we should never rach this point.
	return NULL;
}

// Returns a vector containing the list contents using push_back
template <class T>
vector<T> SLinkedList<T>::Dump() const
{
	vector<T> abc;
	Node<T> * cur = front;

	for (int i = 0; i < size; i++)
	{
		abc.push_back(cur->data);
		cur = cur->next;
	}

	return abc;
}

// OVERLOADED OPERATORS

// must work in the following cases:
// list2 = list1 -> general case
// list2 = list2 -> should do nothing
template <class T>
SLinkedList<T>& SLinkedList<T>::operator=(const SLinkedList<T>& ll)
{
	if (this == &ll)
	{
		return *this;
	}
	//create a

	SLinkedList<T> * temp = new SLinkedList<T>(ll);
	front = temp->front;
	back = temp->back;
	size = temp->size;
	return *temp;
}

#endif