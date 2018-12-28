// Sopheap Sok
// 8/25/2017
// Program: Varg 
// Program make use of variadic functions, in two different ways, to determent the first most frequent character
// in a list of arguments.

#include <stdarg.h>
#include <stdio.h>
#include "Varg.h"


// Function takes in a non-negative integer, n, along with a list of n characters from 'a' to 'z' and 
// return the first most frequently occurring character inputted.
char mostFrequentChar(int n, ...)
{
	int max = 0;
	int i;
	char character, ch;
	int alp[26];
	va_list argp;
	va_start(argp, n);
	
	// Stores the frequency of 'a' to 'z' in an array from 0 to 25 and letting each equal to zero.
	for (i=0; i< 26; i++)
	{	
		alp[i] = 0;	
	}
	
	// Continually pull in characters one by one until the n variable is reached.
	for (i=0; i<n; i++)
	{
		// The arguments get pull in and stored in ch, which will then be mapped to the alp array.
		ch = va_arg(argp, int);
		alp[ch - 'a']++;	
		
		// When each argument gets mapped to the alp array it also gets compared to max and if the value is 
		// greater than what is currently store in max, max will get updated and the argument will be stored 
		// in character. 
		if (max < alp[ch-'a'])
		{
			max = alp[ch-'a'];
			character = ch;
		}
	}
	
	va_end(argp);
	return character;
}

// Function takes in a list of characters from 'a' to 'z' and '\0' as the last argument and determines the first most 
// frequent character.
char fancyMostFrequentChar(char c, ...)
{
	int max = 0;
	int i;
	int alp[26];
	char ch, character = c;
	va_list argp;
	
	va_start(argp, c);
	
	// This insures that if the only character inputted is '\0', it will be return. 
	if (c == '\0')
		return c;
	
	for (i=0; i< 26; i++)
	{	
		alp[i] = 0;	
	}
	
	// Since the first character inputted is not usually read because it's not part of the "..." portion,
	// the below code must be included. 
	alp[c-'a']++;
	
	// This section was seen in the mostFrequentChar() function. The only different is instead of a for loop a while loop
	// is used and told to read in arguments until the '\0' character is encounter.
	while ((ch = va_arg(argp, int)) != '\0')
	{
		alp[ch -'a']++;	
			
		if (max < alp[ch-'a'])
		{
			max = alp[ch-'a'];
			character = ch;
		}
	}	
	
	va_end(argp);
	return character;
}
