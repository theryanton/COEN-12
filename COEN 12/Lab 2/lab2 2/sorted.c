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

struct set // Data structure defines a sorted set, functions such as creating/destroying set, adding/deleting elements, and searching are performed upon it
{
	int count; // Number of elements
	int length; // Array length
	char **data; // Array of strings
};
typedef struct set SET;

SET *createSet (int maxElts) // Creates a sorted set to keep track of number of elements, array length, and the array itself; Runtime: O(1)
{
	SET *sp;
	sp = malloc(sizeof(SET)); // Allocates memory within struct
	assert (sp!=NULL);

	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data != NULL);
	return sp;
}

void destroySet (SET *sp) // Function destroys the set by freeing all the memory associated with the structure; Runtime: O(n)
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

int search (SET *sp, char *elt, bool *found) // Performs a binary search using boundaries defined by variables low, mid & high; Runtime: O(logn)
{
	int low, mid, high, difference;

	low = 0;
	high = sp->count-1;
	while (low<=high)
	{
		mid = (low + high)/2;
		difference = strcmp(elt, sp->data[mid]);
		if (difference < 0) // If word being searched for comes before the word in the array acting as a bound, the high bound is changed to consider only the first half of the array
		{
			high = mid - 1;
		}
		else if (difference > 0) // If word being searched for comes after the word in the array acting as a bound, the low bound is changed to consider only the second half of the array
		{
			low = mid + 1;
		}
		else // If the word is found, we assign the boolean variable to true
		{
			*found = true;
			return mid;
		}
	}
	*found = false; // Element is unfounded, assign boolean variable as false
	return low;
}

void addElement (SET *sp, char *elt) // Using binary search, looks for whether an element is inside the array. If not, we add it to the array by shifting everything above where that element should go to the right and insert it into that spot; Runtime: O(n)
{
	assert (sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched); // Index where element belongs
	int i;
	if (searched == false)
	{
		for (i = sp->count; i>index; i--) // For loop shifts each element to the right
		{
			sp->data[i] = sp->data[i-1];
		}
		sp->data[index] = strdup(elt); // Places element within its new empty slot within the array
		sp->count++; // Increments count of elements within array
	}
}

void removeElement (SET *sp, char *elt) // Using binary search, looks for a specified element. If found, memory is freed at the location and all elements are shifted one to the left; Runtime: O(n)
{
	assert (sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched); // Index where element belongs
	int i;
	if (searched == true)
	{
		free(sp->data[index]); // Frees element at location
		for (i = index+1; i<sp->count; i++) // For loop shifts all elements to the left
		{
			sp->data[i-1] = sp->data[i];
		}
		sp->count--; // Decrements count of elements within array
	}
}

char *findElement (SET *sp, char *elt) // Uses the search function to check whether the element we are looking for is found within the array or not; Runtime: O(logn)
{
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);
	if(searched == true)
	{
		return elt;
	}
	else
	{
		return NULL;
	}
}

char **getElements (SET *sp) // Returns a copy of the array that the user is able to modify; Runtime: O(n)
{
	assert (sp!=NULL);
	char **arr;
	arr = malloc(sizeof(char*)*sp->count);
	int i;
	for (i = 0; i < sp->count; i++)
	{
		arr[i] = strdup(sp->data[i]); // Copies data to user array
	}
	return arr;
}
