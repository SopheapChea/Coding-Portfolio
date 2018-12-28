// Sopheap Sok
// 11/25/2017
// Program: SneakyRooks
// Program uses base conversions and the basic idea of hash tables to track location of rooks and
// determine if rooks are safe or can attack one another. 


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SneakyRooks.h"

// Functional Prototypes
int power(int n);

// Function decides if rooks are safe or can attack one another given the coordinates of each rooks,
// the number of rooks on the board, and the size of the board.
int allTheRooksAreSafe(char **rookStrings, int numRooks, int boardSize)
{
	Coordinate *List;
	int n = 0, i = 0, *row, *col, rindex = -1, cindex = -1;
	
	if (rookStrings[0] == '\0' || rookStrings == NULL)
		return 0;
	
	// Dynamically allocate memory to keep track of each rooks' location.
	// row and col array keep track of rooks' location in relation to each other.
	row = calloc(boardSize+1 ,sizeof(int));	
	col = calloc(boardSize+1 ,sizeof(int));	
	List = calloc(numRooks, sizeof(Coordinate));
	
	for (i=0; i<numRooks; i++)
	{
		// Call parseCoordinateString() function on each rook to determine their location.
		parseCoordinateString(rookStrings[i], &List[i]);
		
		// Keep track if there are more than one rook in each row. If two rooks "hash" to 
		// the same row, the rooks can attack one another, and all memory is free and zero is returned.
		// If no rook is currently in the given row, 1 is inserted into the given position in the array to flag
		// that the row is already occupied by a rook.
		rindex = List[i].row;
		if (row[rindex] != 1)
			row[rindex] = 1;
		
		else 
		{
			free(row);
			free(col);
			free(List);
			return 0;
		}	
		
		// Same idea as the row. If two rooks "hash" to the same column, they are not safe and zero is returned.
		// If no rook is currently in the given column, 1 is inserted into the given position in the array to flag
		// that the column is already occupied by a rook. 
		cindex = List[i].col;
		if(col[cindex] != 1)
			col[cindex] = 1;
		
		else 
		{
			free(row);
			free(col);
			free(List);
			return 0;
		}
	}
	
	// All rooks are on the board and safe from one another. All memory is free and 1 is returned.
	free(row);
	free(col);
	free(List); 
	
	return 1;
}

// Simple power function that determines the result of 26 raise to a given number.
int power(int n)
{
	int i, result = 1;
	
	for (i=0; i<n; i++)
		result *=26;
	
	
	return result;
}

// Function break down a string containing the coordinate of a rook to its given row and column. 
void parseCoordinateString(char *rookString, Coordinate *rookCoordinate)
{
	int i = 0, n = 0, sum = 0, j = 1, k, a;
	
	// Determine the length of the string.
	a = strlen(rookString);
	
	// Loop through the length of the string and determine how many letters are in the string.
	for (i=0; i<a; i++)
	{
		if (isalpha(rookString[i]))
			n++;
	}
	
	// Loop through each letter and convert from base 26 to base 10.
	for (i=0; i<n; i++)
	{	
		j = (rookString[i]-'a')+1;
		k = power(n-1-i);
		j *= k;
		sum += j;
	}
	
	// After looping through the letters, i should now be pointing to the start of the numerical
	// portion of the string. Converting the array of numbers to an integer with atoi and store in
	// the row coordinate of the rookCoordinate struct.
	rookCoordinate->row = atoi(&rookString[i]);
	
	// The now base 10 coordinate for column is stored in the col coordinate of the rookCoordinate struct.
	rookCoordinate->col = sum;
	
	return;
}

