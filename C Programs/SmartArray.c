// Sopheap Sok
// so683950
// COP 3502C 0001, Fall 2017
// 9/9/2017
// Program #2: SmartArray
// Program is a smart array which grows to accommodate new elements whenever it gets too full. 
// Program will trim array to size to prevent wasted space.
// Program includes get() and put() functions to access individual elements.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SmartArray.h"

// Function dynamically allocate space for a new SmartArray.
SmartArray *createSmartArray(int length)
{	
	// Initialize a pointer and set it equal to NULL.
	SmartArray *ptr = NULL;
	ptr = malloc(sizeof(SmartArray));
	
	// If malloc fail, NULL is return.
	if (ptr == NULL)
		return NULL;
	
	if (length < DEFAULT_INIT_LEN)
		length = DEFAULT_INIT_LEN;
	
	// Dynamically allocate space for the array and set them all to NULL.
	ptr->array = calloc(length, sizeof(char *));  
	
	// Set the capacity or maximum number of strings to the length, which 
	// is determine in the if statement above. 
	ptr->capacity = length;
	
	// Since there is currently no string in the array the size must be set to 0.
	ptr->size = 0;
	
	// If calloc fails, NULL is return.	
	if (ptr->array == NULL)
		return NULL;

	printf("-> Created new SmartArray of size %d.\n", length);
	
	return ptr;
}

// Function free any dynamically allocated memory associated with the SmartArray 
// struct and return NULL.
SmartArray *destroySmartArray(SmartArray *smarty)
{
	int i;
	
	if (smarty == NULL)
		return NULL;
	
	// Loops through the array and free any memory that may have been stored.
	for (i=0; i<smarty->size; i++)
		free(smarty->array[i]);
	
	// Free array addresses.
	free(smarty->array);
	
	// Free pointer. 
	free(smarty);
	
	return NULL;
}

// Function dynamically resize the array of length length. 
SmartArray *expandSmartArray(SmartArray *smarty, int length)
{	
	// Insure that if there is no need for expansion function will return without output. 
	if (smarty == NULL || length <= smarty->capacity)
		return createSmartArray(length);
	
	smarty->capacity = length;
	
	// Resize the dynamically allocate memory block to the new length. 
	smarty->array = realloc(smarty->array, sizeof(char*)*length);
	
	printf("-> Expanded SmartArray to size %d.\n", length);
	
	// If realloc fails, NULL is return.
	if (smarty->array == NULL)
		return NULL;
	
	return smarty;
}

// Function trim the array's capacity to the length of the current size. 
SmartArray *trimSmartArray(SmartArray *smarty)
{	
	if (smarty == NULL)
		return NULL;
	
	// If the array needs to be trim, realloc will resize the array to that
	// of the current size.
	if (smarty->capacity > smarty->size)
	{	
		smarty->array = realloc(smarty->array, sizeof(char*)*smarty->size);
	
		smarty->capacity = smarty->size;
		
		printf("-> Trimmed SmartArray to size %d.\n", smarty->size);
		return smarty;
	}
	 
	return smarty;
}

// Function insert a copy of str into the next unused cell of the array. 
char *put(SmartArray *smarty, char *str)
{
	int size, str_length;
	
  	if(smarty == NULL || str == NULL)
		return NULL;
	
	// If the array is full, the expandSmartArray function is called
	// before the new element is inserted.
  	if(smarty->capacity == smarty->size)
  		smarty = expandSmartArray(smarty, smarty->capacity*2+1);
  	
  	size = smarty->size;
    str_length = strlen(str);

	// Dynamically allocate space for str according to it's length. 
  	smarty->array[size] = malloc(str_length+1);
	
	// If malloc fails, NULL is return.
  	if(smarty->array[size] == NULL)
		return NULL;
    
	// Copy str into the array and insert the NULL character at the
	// end so no "garbage" data is added. 
  	strncpy(smarty->array[size], str, str_length);
  	smarty->array[size][str_length] = '\0';
 
  	smarty->size = (smarty->size+1);
 
	// Return a pointer to the copy of the new string that was inserted into
	// the array.
  	return smarty->array[size];
}
	
// Function attempts to return the element at the specified index. 	
char *get(SmartArray *smarty, int index)
{
	// Return NULL if index is out of bounds or smarty pointer is NULL. 
	if (index<0 || index > smarty->capacity-1 || smarty == NULL || smarty->capacity == -1 || smarty->array[index] == NULL)
		return NULL;
	
	return smarty->array[index];
}

// Function attempts to replace a string at position index with a copy of str.
// If there is not a valid string at position index, then NULL is return.
char *set(SmartArray *smarty, int index, char *str)
{
	// Check for invalid index and if smarty and str is NULL.
	if (smarty == NULL || str == NULL || smarty->capacity == -1)
		return NULL;
	
	if (index<0 || index > smarty->capacity-1)
		return NULL;
	
	// Dynamically allocate space for str according to its length. 
	free(smarty->array[index]);
  	smarty->array[index] = malloc(strlen(str)+1);
	
	// If malloc fails, NULL is return.
	if(smarty->array[index] == NULL)
		return NULL;
	
	// Copy str into the array and insert the NULL character at the
	// end so no "garbage" data is added. 
	strncpy(smarty->array[index], str, strlen(str));
	smarty->array[index][strlen(str)] = '\0';

	return smarty->array[index];
}

// Function insert a copy of str at the specified index in the array.
char *insertElement(SmartArray *smarty, int index, char *str)
{
	int i;
	char *array;
	
	if (smarty == NULL || str == NULL)
		return NULL;
	
	// If the array is full, the expandSmartArray function is called
	// before the new element is inserted.
	if (smarty->capacity == smarty->size)
		smarty = expandSmartArray(smarty, smarty->capacity*2+1);
	
	// The specified index is greater than the array’s size, the element
	// being inserted is placed in the first empty position in the array.
	if (index > smarty->size)
	{	
		// Dynamically allocate space for str according to its length. 
		free(smarty->array[index]);
		smarty->array[index] = malloc(strlen(str)+1);
		
		// Copy str into the array.
		strncpy(smarty->array[index], str, strlen(str));
		
		return smarty->array[smarty->size];
	}
	
	// A copy is inserted and any elements to the right of index are shifted
	// one space to the right.
	else
	{	
		for (i = smarty->size; i > index; i--)
			smarty->array[i] = smarty->array[i-1];
		
		// Dynamically allocate space for str according to its length. 
		smarty->array[index] = malloc(strlen(str) + 1);
		
		// If malloc fails, NULL is return.
		if (smarty->array[index] == NULL)
			return NULL;
		
		// Copy str into the array.
		strncpy(smarty->array[index], str, strlen(str));
		
		smarty->size = smarty->size+1;
		
		return smarty->array[index];
	}
	return;
}

// Function remove the string at the specified index in the array.
int removeElement(SmartArray *smarty, int index)
{
	int i;
	
	// 0 is return when smarty is NULL or when the element fails to be removed.
	if (smarty == NULL || index > smarty->size-1)
		return 0;
	
	free(smarty->array[index]);
 
	// Strings to the right of index are shifted one space to the left, 
	// so as not to leave a gap in the array. 
  	for(i = index; i < smarty->size-1; i++)
  		smarty->array[i] = smarty->array[i+1];
	
	smarty->size = smarty->size-1;
  	
	return 1;
}

// Function returns the number of elements currently in the array. 
int getSize(SmartArray *smarty)
{
	if (smarty == NULL)
		return -1;
	
	return smarty->size;
}

// Function prints all strings currently in the array.
void printSmartArray(SmartArray *smarty)
{
	int i;
	
	// Check if array is empty or smarty is NULL.
	if (smarty == NULL || smarty->array == 0 || smarty->size == 0)
		printf("(empty array)\n");
		
	for (i=0; i<getSize(smarty); i++)
		printf("%s\n", get(smarty,i));
	
	return;
}

// Function returns a double from 1.0 to 5.0 according to what the hoursSpent() function returns.
double difficultyRating(void)
{
	if (hoursSpent() >= 12.0)
		return 4.5; 
	else return 4.0;
}

// Function returns a double depending on how many hours is spent on the assignment. 
double hoursSpent(void)
{
	return 14.0;
}