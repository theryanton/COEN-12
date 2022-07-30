/* Ryan Ton
 * COEN 12
 * Lab 1
 * Tuesday 5:15pm
 */

#define MAX_WORD_LENGTH 30

#include <stdio.h>
#include <stdlib.h>

void counter (char *fname);

int main (int argc, char *argv[])
{
	if (argc == 1) // Checks for correct number of arguments
	{
		printf("There's no file currently found.\n");
	}
	else
	{
		counter (argv[1]);
	}
	return 0;
}

void counter (char *fname) // Function used to perform while loops and count words
{
	FILE *inputfile;
	int count = 0;
	char ch[MAX_WORD_LENGTH];

	if ((inputfile = fopen(fname, "r")) == NULL) // If the file doesn't exist, return error
	{
		printf("No data found.\n");
	}
	else
	{
		while (fscanf(inputfile,"%s",ch) == 1) // While loop checks if words are successfully read, increasing the count by 1 each time
		{
			count++;
		}
		printf("Words: %d\n", count); // Returns count of all items successfully stored in memory
	}
	return;
}
