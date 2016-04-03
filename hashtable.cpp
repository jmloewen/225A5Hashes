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
	size = 0;
	maxsize = 101;	//by default, maxsize = the smallest prime greater than 2*n (n=101).
	table = new SLinkedList<UserAccount>[101];
}

// parameterized constructor
// creates an array of size = smallest prime number > 2n
HashTable::HashTable(int n)
{
	size = 0;
	maxsize = SmallestPrime(n * 2);
	table = new SLinkedList<UserAccount>[maxsize];
}

// copy constructor
// Creates deep copy of sourceht
HashTable::HashTable(const HashTable& sourceht)
{
	size = sourceht.size;	//get size, maxsize.
	maxsize = sourceht.maxsize;
	table = new SLinkedList<UserAccount>[maxsize];//create an array of the same size.
	SLinkedList<UserAccount> *curList;
	//fill this table with values from sourceht.


	for (int i = 0; i < maxsize; i++)
	{
		curList = &sourceht.table[i];
		//run copy on each linked list.
		//idk if this works?  Maybe it will?
		table[i] = *(new SLinkedList<UserAccount>(*curList));
	}
}

// hash function, uses Horner's method
// Assume input string consists only of lower-case a to z
int HashTable::Hash(string input) const
{
	int hashval = 0;
	for (int i = 0; i < (signed)input.length(); i++)
	{
		//hashval = itself + the ascii - ascii conversion * horner conversion % arraysize.
		hashval = (hashval + ((input[i] - 96) * 32));
	}

	return hashval % maxsize;
}

// helper function to find smallest prime number greater than supplied parameter
int HashTable::SmallestPrime(int n) const
{
	//incrementing n guarantees that even if we're sent a prime, we'll look at the next value instead.
	int nextPrime = n+1;

	//edge cases - these must be here to make our search more efficient, and to cut out even checks.
	if (n == 0 || n == 1 || n == 2)
	{
		return n+1;
	}

	//if we're on an even number, increment it so that it's odd (no evens are primes but '2').
	if (nextPrime % 2 == 0)
	{
		nextPrime++;
	}

	while (!IsPrime(nextPrime))
	{
		//increment nextprime by 2
		nextPrime += 2;
	}
	return nextPrime;
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

	//Theorem: All primes that are not '1', '2', or '3', exist as some factor of 6 + 1 or 6 - 1.
	//Any value that is not 1 above or below a multiple of 6 does not need to be checked.
	//Explanation: if it is '6', or '2', or '4' * n, it is a multiple of 2 and therefore not prime.
	//if it is '6' or '3' * n, it is a multiple of 3 and therefore not prime.
	//therefore, the only values we need to check are the values +-1 of 6 (5 and 7, or 'remainder 1').
	//Once we have surpassed the threshold of the sqrt of n, no values beyond sqrt of n can be a factor of n.
	//This is because in order for it to be a factor of n, it requires a second factor to multiply by.
	//this second value would have to be less than or equal to sqrt(n), otherwise the product would be larger than n.
	//as a result, this value must have already been checked, and therefore does not exist (since we would have left the loop if it did).
	//This means we can get away with checking only values up to the square root of n + 1 
	//(the + 1to take care of potential rounding errors).
	for (int i = 6; i < sqrt(n) + 1; i+=6)
	{
		if ((n % (i - 1) == 0) || (n % (i + 1) == 0))
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
bool HashTable::Resize(int n)
{
	if (n < maxsize || n < 0)
	{
		return false;
	}

	//run constructor on the new table.
	HashTable * nuHash = new HashTable(n);
	//we now have an empty hashtable of size prime>n*2.
	vector<UserAccount> temp;
	for (int i = 0; i < size; i++)
	{
		//dump each linked list and write its contents to the new hash table.

		if ((table[i]).Size() > 0)	//if list's size is greater than 0, enter the while.
		{
			temp = table[i].Dump();	//point vector in the right direction.
			//for all values in this linked list, insert each value into our new hash table.
			for (int j = 0; j < (signed)temp.size(); j++)
			{
				//UserAccount * user = new UserAccount(temp.at(j));
				nuHash->Insert(temp.at(j));
			}
		}
	}
	//all values have been migrated at this point.
	table = nuHash->table;
	maxsize = nuHash->maxsize;
	
	return true;
}

// destructor
HashTable::~HashTable()
{
	for (int i = 0; i < maxsize; i++)
	{
		table[i].RemoveAll();
	}
	size = 0;
	maxsize = 0;
}

// overloaded assignment operator
HashTable& HashTable::operator=(const HashTable& sourceht)
{
	if (this == &sourceht)
	{
		return *this;
	}
	//create a new hash table and copy over the values.
	HashTable * nuHash = new HashTable(sourceht);
	nuHash->maxsize = sourceht.MaxSize();
	nuHash->size = sourceht.Size();
	return *nuHash;
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
		if (LoadFactor() > (double)2/3)
		{
			Resize(maxsize);
		}
		//hash the username, then search for that hash - if that hash does not exist, hash the password and insert the useraccount into the array.
		int unamehash = Hash(acct.GetUsername());

		//create the new useraccount.
		UserAccount *newUser = new UserAccount(acct);
		newUser->SetPassword(acct.GetPassword(), acct.GetPassword());
		newUser->SetUserLevel(acct.GetUserLevel());

		table[unamehash].InsertBack(*newUser);

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
	if (!Search(acct))
	{
		return false;
	}
	//get the hash location.
	int unamehash = Hash(acct.GetUsername());
	//remove the user from the appropriate linked list.
	table[unamehash].Remove(acct);
	size--;
	return true;
}

// Search
// Returns true if item exists, false otherwise
bool HashTable::Search(UserAccount acct) const
{
	//call hash and get the hash value of the username.
	int unamehash = Hash(acct.GetUsername());

	bool containsval = table[unamehash].Contains(acct);
	return containsval;
}

// Retrieval
// Returns a pointer to a UserAccount object inside the hash table (linked list)
//   if a matching parameter is found, otherwise return NULL
UserAccount* HashTable::Retrieve(UserAccount acct)
{
	//this is simple - hash the account, go to that linked list and return the retrieved value.
	int unamehash = Hash(acct.GetUsername());
	return table[unamehash].Retrieve(acct);
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
	return (double)size/maxsize;
}