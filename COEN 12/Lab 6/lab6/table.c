/* Ryan Ton
 * COEN 12
 * Lab 6
 * Tuesday 5:15pm
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#define EMPTY 0
#define FILLED 1
#define DELETED 2

struct set
{
	int count; // Number of elements
	int length; // Length of arrays
	void **data; // Array recording strings
	char *flags; // Array of flags that help traversing array
	int (*compare) (); // Implemented function within the set used to compare elements stored in set's arrays
	unsigned (*hash) (); // Basically strhash, but implemented within set
};
typedef struct set SET;

SET *createSet (int maxElts, int (*compare)(), unsigned (*hash)() ) 
{
	SET *sp;
	sp = malloc (sizeof(SET));
	assert (sp != NULL);

	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(void*)*maxElts);
	sp->flags = malloc(sizeof(char*)*maxElts);
	sp->compare = compare;
	sp->hash = hash;

	int i;
	for (i=0; i<maxElts; i++)
	{
		sp->flags[i] = EMPTY;
	}
	assert (sp->data != NULL);
	return sp;
}

void destroySet (SET *sp) // Destroys set by freeing all memory allocated that's associated with set; Runtime: O(1)
{
	free (sp->flags); // First frees the array of flags
	free (sp->data); // Then frees the array recording the data
	free (sp); // Finally frees the set itself
}

int numElements (SET *sp) // Returns count of elements within set; Runtime: O(1)
{
	assert (sp != NULL);
	return sp->count;
}

int search (SET *sp, void *elt, bool *found) // Performs a search through linear probing, runs by assigning a starting hash value and keeping track of a deleted location. As it iterates thorugh array with for loop, if the location is FILLED, the element has been found and the functiom returns simply its index. If DELETED, a variable is used to record the location of the first deleted element. Otherwise, if EMPTY, this means the element was not found in this array, and if no deleted location was recorded, the function returns the empty index at the end. If a deleted location was found, the function will return its index instead; Expected runtime: O(1), Worst runtime: O(n)
{
	int i, locn;
	int head = (*sp->hash)(elt) % sp->length;
	int deletedlocn = -1; // Variable keeping track of deleted location

	for (i=0; i<sp->length; i++) // For loop used to traverse array
	{
		locn = (head + i) % sp->length; // Assigning starting hash value
		if (sp->flags[locn] == FILLED) // FILLED means found, therefore we return its location
		{
			if ((sp->compare)(sp->data[locn],elt) == 0)
			{
				*found = true;
				return locn;
			}
		}
		else if (sp->flags[locn] == DELETED) // DELETED means the element we're searching for could still be later down the array
		{
			if (deletedlocn == -1)
			{
				deletedlocn = locn; // Therefore, we use deletedlocn to remember the first deleted location found
			}
		}
		else // Otherwise, EMPTY, element was not found throughout the array
		{
			break;
		}
	}
	*found = false;
	if (deletedlocn == -1) // If deletedlocn has not changed at all, we need to return the new empty index at the end
	{
		return locn;
	}
	return deletedlocn; // Otherwise, return the first deleted location found
}

void addElement (SET *sp, void *elt) // Performs a search via search function to look for whether the inserted element already exists. If not found, we insert the new element into an index and assign the corresponding flag to FILLED; Expected runtime: O(1), Worst runtime: O(n)
{
	assert (sp!=NULL && elt!=NULL);
	bool searched;
	int index = search (sp, elt, &searched); // Uses search function to search for correct index where element belongs

	if (searched == false) // If not found
	{
		sp->data[index] = elt; // New element inserted into index
		sp->flags[index] = FILLED; // Corresponding flag set to FILLED
		sp->count++; // Count of elements incremented
	}
}

void removeElement (SET *sp, void *elt) // Performs a search via search function to look for whether a specific element exists within array. If found, we free the memory at the location and assign its corresponding flag to DELETED; Expected runtime: O(1), Worst runtime: O(n)
{
	assert (sp!= NULL && elt != NULL);
	bool searched;
	int index = search(sp, elt, &searched); // Uses search function to search for whether a specific element exists within array

	if (searched == true) // If found
	{
		sp->flags[index] = DELETED; // Frees corresponding memory at the location, setting flag to DELETED
		sp->count--; // Count of elements decremented
	}
}

void *findElement (SET *sp, void *elt) // Utilizing search function, if search returns as true, we have therefore found the element. Otherwise, returns NULL; Expected runtime: O(1), Worst runtime: O(n)
{
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);
	if (searched==true)
	{
		return sp->data[index];
	}
	else
	{
		return NULL;
	}
}

void *getElements (SET *sp) // Returns a copy of the sorted array of elements within the set; Runtime: O(n^2)
{
	assert(sp!=NULL);
	void **copyArray = malloc(sizeof(void*)*sp->count); // Declaring array and allocating memory to be size equal to number of elements
	assert (copyArray!=NULL);
	int i;
	int last = 0; // Last index in array

	for (i = 0; i < sp->length; i++)
	{
		if (sp->flags[i] == FILLED)
		{
			copyArray[last]=sp->data[i]; // Copies each element within array one by one 
			last++; // Keeps track of the size of the array, increases as elements are copied
		}
	}
	quickSort(copyArray,0,sp->count-1,sp->compare); // Now that we have an exact duplicate of the elements array, call quickSort to sort the array
	return copyArray;
}

int partition (void **elts, int lo, int hi, int (*compare)()) // Function is recursively called by quickSort to sort all the elements in the array so that the values on the left of the pivot are less than its value, and the ones on the right are greater; Runtime: O(n)
{
	void *pivot = elts[hi]; // Furthest right element chosen as pivot
	void *swap; // Temporary variable used to help us swap elements
	int i;
	int smallerValue = lo;

	for (i=lo; i<hi; i++) // Checks that all values to left of partition are less than the pivot value and all values to the right are greater
	{
		if ((*compare)(elts[i], pivot) < 0)
		{
			swap = elts[i];
			elts[i] = elts[smallerValue];
			elts[smallerValue] = swap;
			smallerValue++;
		}
	}
	swap = pivot;
	elts[hi] = elts[smallerValue];
	elts[smallerValue] = swap;
	return smallerValue; // Returns correct index for swapped element
}

void quickSort (void **elts, int lo, int hi, int (*compare) ()) // Recursive function calls on the partition function to continuously sort elements in the array; Runtime: O(n^2) 
{
	if (lo < hi) // Recursively calls its own function to sort its respective subarrays to the left and right of the pivot
	{
		int partIndex = partition(elts, lo, hi, compare);
		quickSort(elts, lo, partIndex - 1, compare); // Left side
		quickSort(elts, partIndex + 1, hi, compare); // Right side
	}
	return;
}
