/* Ryan Ton
 * COEN 12
 * Lab 2
 * Tuesday 5:15pm
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "set.h"

struct set // Data structure defines an unsorted set, functions such as creating/destroying set, adding/deleting elements, and searching are performed upon it
{
	int count; // Number of elements
	int length; // Array length
	char **data; // Array of strings
};
typedef struct set SET;

SET *createSet (int maxElts) // Creates an unsorted set to keep track of number of elements, array length, and the array itself; Runtime: O(1)
{
	SET *sp;
	sp = malloc(sizeof(SET)); // Allocates memory within struct
	assert (sp!=NULL);

	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert (sp->data != NULL);
	return sp;
}

void destroySet (SET *sp) // Function destroys the set by free all the memory associated with the structure; Runtime: O(n)
{
	int i;
	for (i=0; i<sp->count; i++)
	{
		free (sp->data[i]); // Frees the elements first
	}
	free (sp->data); // Then, frees the array used to keep track of elements within set
	free (sp); // Frees set itself
}

int numElements (SET *sp) // Returns number of elements within set; Runtime: O(1)
{
	assert (sp!=NULL);
	return sp->count;
}

int search (SET *sp, char *elt) // Performs linear search throughoutset, returning index of specified element if found; Runtime: O(n)
{
	assert (sp!=NULL && elt!=NULL);
	int i;

	for (i=0; i<sp->count; i++)
	{
		if (strcmp(sp->data[i], elt)==0) // Compares string at index to element to look for a match
		{
			return i;
		}
	}
	return -1; // If no match is found in the array, -1 is returned
}

void addElement (SET *sp, char *elt) // Uses search function to linearly search for specified element. If no duplicate is found, element will be added to end of set; Runtime: O(n)
{
	assert(sp!=NULL && elt!=NULL);
	if (search(sp, elt) == -1)
	{
		sp->data[sp->count] = strdup(elt); // No duplicate found, element added to end of array
		sp->count++; // Increments count of elements within array
	}
}

void removeElement (SET *sp, char *elt) // Uses search function to linearly search for specified element. If found, memory is freed at the location and places the last element in the previous slot; Runtime: O(n)
{
	assert(sp!=NULL && elt!=NULL);
	int index = search(sp, elt); // Traverses array
	if (index != -1)
	{
		free(sp->data[index]); // Frees element  at location
		sp->data[index] = sp->data[sp->count-1]; // Places last element into that slot
		sp->data[sp->count-1] = NULL;
		sp->count--; // Decrements count of elements within array
	}
}

char *findElement (SET *sp, char *elt) // Function operates similarly to search function, will perform linear searcj throughout the set and return specified element; Runtime: O(n)
{
	assert(sp!=NULL && elt!=NULL);
	int i;
	for(i=0; i<sp->count; i++)
	{
		if (strcmp(sp->data[i], elt) == 0)
		{
			return elt;
		}
	}
	return NULL; // If specified element not found, returns NULL
}

char **getElements (SET *sp) // Returns a copy of the array that the user is able to modify; Runtime: O(n)
{
	assert(sp!=NULL);
	char **arr;
	arr = malloc(sizeof(char*)*sp->count);
	int i;
	for(i=0; i<sp->count; i++)
	{
		arr[i] = strdup(sp->data[i]); // Copies data to user array
	}
	return arr;
}

