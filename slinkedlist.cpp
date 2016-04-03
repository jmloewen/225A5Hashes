// File:        slinkedlist.h
// Author:      Jonathan Loewen
// Date:        March 26, 2016
// Description: Definition of a template singly-linked-list class for CMPT 225 assignment 5
//Principal coding on March 26th-27th, 2016.
//Final fixes to Insertion and resize on April 2nd-3rd 2016.
//Final Commenting on April 3rd, 2016.

#ifdef _SLINKEDLIST_H_

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

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
	//if we're here, size > 0 on the sent LL.
	size = ll.Size();
	CopyList(ll);
}

// helper function for deep copy
// Used by copy constructor and operator=
template <class T>
void SLinkedList<T>::CopyList(const SLinkedList<T>& ll)
{

	Node<T> * temp = new Node<T>(ll.front->data);	//instantiate a node.
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
	RemoveAll();	//call remove function, null out values.
	front = NULL;
	back = NULL;
	size = 0;
}

// MUTATORS

// Inserts an item at the front of the list
// POST:  List contains item at front
// PARAM: item = item to be inserted
template <class T>
void SLinkedList<T>::InsertFront(T item)
{
	//instantiate a node.
	Node<T> * newNode = new Node<T>(item);

	if (size == 0)
	{
		back = newNode;
	}

	//put the node into position and increment.
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
	//instantiate a node.
	Node<T> * newNode = new Node<T>(item);

	//put the node into place and increment.
	if (size == 0)
	{
		front = newNode;
	}
	else
	{
		back->next = newNode;
	}
	back = newNode;
	size++;
}

// Removes the first occurrence of the supplied parameter
// Removes and returns true if found, otherwise returns false if parameter is not found or list is empty
template <class T>
bool SLinkedList<T>::Remove(T item)
{
	//by the time we're here, the hashing is complete.  Search within the linked list for our item.

	//if the list does not contain this item, there's nothing to remove.
	if (!Contains(item))
	{
		return false;
	}

	//item exists, remove it.

	//two pointers to the node previous to the one we want to remove, and the one we want to remove
	Node<T> * temp = front;
	Node<T> * prev = NULL;
	size--;

	//loop through the linked list.
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
	//we should never get here.  Getting here means the item exists but we somehow didnt find and remove it - we have to re-increment and return false.
	size++;
	return false;
}

// Removes all items in the list
template <class T>
void SLinkedList<T>::RemoveAll()
{
	//if there is nothing in the list, return.
	if (front == NULL)
	{
		return;
	}

	//iteratively delete the list.
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
	if (front == NULL)
	{
		return true;
	}
	return false;
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
	while (temp != NULL)
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

	//item exists, go get it.
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
	//we should never reach this point.
	return NULL;
}

// Returns a vector containing the list contents using push_back
template <class T>
vector<T> SLinkedList<T>::Dump() const
{
	//create a vector, push all values into it.
	vector<T> retvec;
	Node<T> * cur = front;

	for (int i = 0; i < size; i++)
	{
		retvec.push_back(cur->data);
		cur = cur->next;
	}
	return retvec;
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
	//create a new list with identical values.
	SLinkedList<T> * temp = new SLinkedList<T>(ll);
	front = temp->front;
	back = temp->back;
	size = temp->size;
	return *temp;
}

#endif