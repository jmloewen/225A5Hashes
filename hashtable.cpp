// File:        hashtable.h
// Author:      Geoffrey Tien
// Date:        March 25, 2016
// Description: Definition of a hash table class for CMPT 225 assignment 5
//              must include hashtableprivate.h file for additional private member functions added by student

#pragma once

#include <cstdlib>
#include <math.h> // needed for Hash function
#include <string>
#include <vector>
#include "slinkedlist.h"
#include "useraccount.h"
#include "hashtable.h"

using namespace std;

static const int DEFAULTSIZE = 101;

//a hash table contains a Linked List of UserAccounts 'table', and two integers 'size' (cursize) and 'maxsize'.
 
// write your own private member functions in the following file
#include "hashtableprivate.h"

// default constructor
// creates an array of size maxsize
HashTable::HashTable()
{
	//SLinkedList<UserAccount> * table = new SLinkedList<UserAccount>();
	
	maxsize = SmallestPrime(DEFAULTSIZE);	//by default, maxsize = the smallest prime greater than 2*n (n=101).
	table = new SLinkedList<UserAccount>[maxsize];

	SLinkedList<UserAccount> * temp;

	size = 0;
	for (int i = 0; i < maxsize; i++)
	{
		temp = new SLinkedList<UserAccount>();
		table[i] = *temp;
	}
	size = 0;
}

// parameterized constructor
// creates an array of size = smallest prime number > 2n
HashTable::HashTable(int n)
{
	size = 0;
	maxsize = SmallestPrime(0);
	SLinkedList<UserAccount> * table = new SLinkedList<UserAccount>[maxsize];

	for (int i = 0; i < maxsize; i++)
	{
		SLinkedList<UserAccount> * temp = new SLinkedList<UserAccount>();
		table[i] = *temp;
	}

}

// copy constructor
// Creates deep copy of sourceht
HashTable::HashTable(const HashTable& sourceht)
{
}

// hash function, uses Horner's method
// Assume input string consists only of lower-case a to z
int HashTable::Hash(string input) const
{
	int hashval = 0;
	for (int i = 0; i < (signed)input.length(); i++)
	{
		//hashval = itself + the ascii - ascii conversion * horner conversion % arraysize.
		hashval = (hashval + ((input[i] - 96) * 32)) % maxsize;
	}
	return hashval;
}

// helper function to find smallest prime number greater than supplied parameter
int HashTable::SmallestPrime(int n) const
{
	int nextPrime = n * 2;
	while (!IsPrime(nextPrime++))
	{
	}
	return nextPrime - 1;
}

//All composites between 0 and n>3 are either a multiple of 2, 3, or some factor of 6 +- 1 
//(this covers remainders 0 (, 2, 3, and 4, leaving remainders 1 and 5 as primes.  
//These are the only values we need to test for).
bool HashTable::IsPrime(int n) const
{
	if (n == 1 || n == 2 || n == 3)
	{
		return true;
	}
	if (n % 2 == 0 || n % 3 == 0)
	{
		return false;
	}

	for (int i = 6; i < sqrt(n); i+=6)
	{
		if (n % (i-1) == 0 || n % (i+1) == 0)
		{
			return false;
		}
	}

	return true;
}

// Resizes the hashtable into a larger array.
// Return false if n is smaller than current array size or if n is negative.
// Else, set array size to the smallest prime number larger than n
//   and re-hash all contents into the new array, delete the old array and return true.
bool Resize(int n)
{
	//here we want o find the next greater prime, then re-hash all existing values in accordance.
	return false;
}

// destructor
HashTable::~HashTable()
{
//	delete table;
	//delete every array entry individually if it is non-null.
}

// overloaded assignment operator
HashTable& HashTable::operator=(const HashTable& sourceht)
{
	return *this;
}

// Insertion
// If item does not already exist, inserts at back of hashed list and returns true
//   otherwise returns false
// If load factor (before insertion) is above 2/3, expand into a new
//   table of smallest prime number size at least double the present table size
//   and then insert the item.
bool HashTable::Insert(UserAccount acct)
{
	//if search returns false (the item doesnt exist), enter.  else, return false.
	if (!Search(acct))
	{
		//hash the username, then search for that hash - if that hash does not exist, hash the password and insert the useraccount into the array.
		int unamehash = Hash(acct.GetUsername());
		int pwhash = Hash(acct.GetPassword());

		//create the new useraccount.
		UserAccount * newUser = new UserAccount(acct);
		

		
		newUser->SetPassword(acct.GetPassword(), acct.GetPassword());

		table[unamehash].InsertBack(*newUser);
		//here is where we hash.
		//first call the hash function, then insert at the appropriate index.
		//int index = Hash(acct->password);
		size++;
		return true;
	}
	return false;

	

}

// Removal
// If item exists, removes and returns true
//   otherwise returns false
bool HashTable::Remove(UserAccount acct)
{
	//search and then remove at the appropriate index.	
	return false;
}

// Search
// Returns true if item exists, false otherwise
bool HashTable::Search(UserAccount acct) const
{

	//call hash and get the hash value of the username.
	int unamehash = Hash(acct.GetUsername());

	//search the array of linked lists for unamehash.
	//look at all entries in the array.
	bool containsval = false;
	for (int i = 0; i < size; i++)
	{
		//iterate through all items in the designated linked list.
		containsval = table[i].Contains(acct);
		if (containsval)
		{
			return true;
		}
	}


	return false;
}

// Retrieval
// Returns a pointer to a UserAccount object inside the hash table (linked list)
//   if a matching parameter is found, otherwise return NULL
UserAccount* HashTable::Retrieve(UserAccount acct)
{
	return NULL;
}

// Returns the number of items stored in the hash table
int HashTable::Size() const
{
	return size;
}

// Returns the size of the underlying array
int HashTable::MaxSize() const
{
	return maxsize;
}

// Returns the load factor as size / maxsize.
// Note that due to separate chaining, load factor can be > 1.
double HashTable::LoadFactor() const
{
	return double(size/maxsize);
}