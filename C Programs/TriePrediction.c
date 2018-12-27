// Sopheap Sok
// so683950
// COP 3502C 0001, Fall 2017
// 11/2/2017
// Program #4: TriePrediction
// Program uses the advanced tree data structure, trie, to store strings,
// and provide efficient insertion and lookup. Program also implement text prediction
// with the inclusion of subtries in the trie data structure. 

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"

// Functional Prototypes
TrieNode *createTrieNode(void);
TrieNode *insertString(TrieNode *root, char *str, int flag);
void stripPunctuators(char *str);
TrieNode *findSubtrie(TrieNode *root, char *str);
int countWord(TrieNode *root);
void getMostFrequentWordHelper(TrieNode *root, char *buffer, int k, int *max, char *freq);


// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

// Auxiliary function that removes all punctuation from the string.
void stripPunctuators(char *str)
{
	int i, n, j = 0;
	char *buffer = malloc(sizeof(char)*(MAX_CHARACTERS_PER_WORD + 1));
	
	n = strlen(str);
	
	// Loop through the length of the string and copy only letters from the string
	// in str to buffer. 
	for (i=0; i<n; i++)
	{
		if (isalpha(str[i]))
			buffer[j++] = str[i];	
	}
	
	buffer[j] = '\0';
	
	// Copy the new stripped string back into str and free buffer.
	strcpy(str, buffer);
	free(buffer);
	
	return;
}

// Part of code from tries.c by Dr. Szumlanski.
// Auxiliary function insert a string into the trie and return the subtrie.
TrieNode *insertString(TrieNode *root, char *str, int flag)
{
	int i, n, next;
	TrieNode *sub = NULL, *temp;
	
	if (root == NULL)
		return NULL;
	
	temp = root;
	n = strlen(str); 
	
	// Loop through the length of the string and check if each letter is 
	// in the trie. If not, createTrieNode() function is called to make the 
	// node before moving forward. Root count is increment at the end of each word.
	for (i=0; i<n; i++)
	{	
		next = tolower(str[i]) - 'a';
		
		if (temp->children[next] != NULL)
		{
			temp = temp->children[next];
		}
		
		else
		{
			temp->children[next] = createTrieNode();
			temp = temp->children[next];
		}
		
	}
		
	temp->count++;
	
	// When flagged from buildTrie() function and the subtrie is made sure that it doesn't exist yet,
	// a node is created for the subtrie and sub is set pointing at it.
	if (flag == 1 && temp->subtrie == NULL)
		sub = temp->subtrie = createTrieNode();
	
	// Always return subtrie if it is not NULL.
	if (temp->subtrie != NULL)
		sub = temp->subtrie;
	
	// sub will be NULL if the subtrie is NULL.
	return sub;
}

// Function reads in the corpus text file and builds a trie with its appropriate subtries.
TrieNode *buildTrie(char *filename)
{
	TrieNode *root, *sub;
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	FILE *fpt;
	int n;
	
	// Code from tries.c written by Dr.Szumlanski.
	if ((fpt = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in buildTrie().\n", filename);
		return NULL;
	}
	
	// Create the root of the main trie.
	root = createTrieNode();
	
	// Scan in the first word from the corpus file, strip the punctuations,
	// and insert the word in the trie. First word is always flagged to create a subtrie.
	// A pointer to the subtrie is always return from the insertString() function, whether 
	// it is NULL or not. The TrieNode pointer, sub, will capture it, so when the next word
	// gets scan in, it can be inserted in the previous word subtrie.
	fscanf(fpt, "%s", buffer);
	stripPunctuators(buffer);
	sub = insertString(root, buffer, 1);

	// Insert word one at a time into the trie.
	while (fscanf(fpt, "%s", buffer) != EOF)
	{
		// Find the length of the string in the buffer.
		n = strlen(buffer);
		
		// Detect the last word in the line and insert into the subtrie of the previous word
		// and main trie without making a subtrie.
		if (buffer[n-1] == '.' || buffer[n-1] == '!' || buffer[n-1] == '?')
		{
			stripPunctuators(buffer);
			insertString(sub, buffer, 0);
			insertString(root, buffer, 0);
			
			// Check if pointer is at the end of the file and avoid scanning in the first word again.
			// Insert conditions for the last word in a line and the first word on the next line.
			if (fscanf(fpt, "%s", buffer) != EOF)
			{
				// Insert condition for lines containing only one word.
				// Word is inserted into the main trie only and its subtrie remains NULL. 
				n = strlen(buffer);
				if (buffer[n-1] == '.' || buffer[n-1] == '!' || buffer[n-1] == '?')
				{
					stripPunctuators(buffer);
					insertString(root, buffer, 0);
				}
				
				// If the line contains more than one word, the first word is inserted like the 
				// first word in the corpus file.
				else 
				{
					stripPunctuators(buffer);
					sub = insertString(root, buffer, 1);	
				}
			}
		}
		
		// Middle of the line word insertion, with the word being inserted into the previous
		// word subtrie and insert into the main with a created subtrie.
		else 
		{
			stripPunctuators(buffer);
			insertString(sub, buffer, 0);
			sub = insertString(root, buffer, 1);
		}
		
	}
	
	// Close file and return the root.
	fclose(fpt);
	return root;
}

// Function dynamically create a TrieNode and set everything in the TrieNode to NULL.
TrieNode *createTrieNode(void)
{
	return calloc(1, sizeof(TrieNode));
	
}

// Function takes in the root of a trie and read and process the input file also passed in.
// Function calls other functions as needed to perform given commands.
int processInputFile(TrieNode *root, char *filename)
{
	FILE *fpt;
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	int n, i;
	TrieNode *sub;
	
	// Open and read the file in.
	fpt = fopen(filename, "r");
	
	// Check for bad filename being passed in.
	if (fpt == NULL)
		return 1;
	
	// Scan in each word or symbol in the file one at a time.
	while (fscanf(fpt, "%s", buffer) != EOF)
	{
		switch (buffer[0])
		{
			// When the @ symbol is encounter, the next word and number is scanned in
			// and stored in buffer and n respectively.
			case '@':
			fscanf(fpt, "%s %d", buffer, &n);
			
			// Print out the word just scanned in.
			printf("%s", buffer);
			
			//Check if the word is in the trie.
			if (containsWord(root, buffer) == 1)
			{
				// Find the subtrie of the word and set the pointer sub to it.
				sub = findSubtrie(root, buffer);
				
				// If the subtrie exist, use getMostFrequentWord() function to find the frequent word
				// in the subtrie and store it in buffer.
				if (sub != NULL)
				{
					// Loop through depending on what number was scanned in and stored in n.
					for (i=0; i<n; i++)
					{
						getMostFrequentWord(sub, buffer);
						
						// If the root of the subtrie is NULL, an empty string will be stored in the buffer.
						// Make sure to only print when there are strings in the trie.
						if (strcmp(buffer, "") != 0)	
							printf(" %s", buffer);
						
						// Find the subtrie of the most frequent word found.
						sub = findSubtrie(root, buffer);

					}
					printf("\n");
				}
			}
			
			// The word has no subtrie, so print new line character.
			else printf("\n");

			break;
			
			// When the ! symbol is encounter, the root is given to the
			// printTrie() function to print out the main trie.	
			case '!':
			printTrie(root, 0);
			break;
			
			// If no ! or @ has been encounter, then the command is a string.
			default:
			
			// Print string that was scanned in onto the screen.
			printf("%s\n", buffer);
			
			// Check if the word is in the trie.
			if (containsWord(root, buffer) == 1)
			{	
				// Find the subtrie of the word and print out the subtrie if it is not NULL.
				sub = findSubtrie(root, buffer);
			
				if (sub != NULL)
					printTrie(sub, 1);
				
				// Subtrie is NULL.
				else printf("(EMPTY)\n");
				
			}
			
			// Word is not in the trie.
			else
			{
				printf("(INVALID STRING)\n");
			}
			
			break;

		}
	}
	
	// Close file and return 0.
	fclose(fpt);
	
	return 0;
}

// Function free all dynamically allocated memory associated with the trie.
TrieNode *destroyTrie(TrieNode *root)
{
	int i;
	
	// Base case to exit the recursion.
	if (root == NULL)
		return NULL;
	
	// Travel down all 26 children of the root and free each on the way up.
	for (i=0; i < 26; i++)
	{
		destroyTrie(root->children[i]);

	} 
	
	// Travel to root subtrie if they exist and free.
	if (root->subtrie != NULL)
		destroyTrie(root->subtrie);
	
	// Free the main root.
	free(root);
	
	return NULL;
}

// Function searches the trie for the specified string stored in str.
TrieNode *getNode(TrieNode *root, char *str)
{
	int next, i, n = strlen(str);
	
	if (root == NULL)
		return NULL;
	
	// Loop through the length of the string and travel through the trie
	// to check if each edge corresponds to each letter in the string.
	for (i=0; i<n; i++)
	{
		next = tolower(str[i]) - 'a';
		
		if (root->children[next] != NULL)
			root = root->children[next];
	}
	
	// If at the end of the string the root count is greater than 0 then the
	// string is in the trie and the pointer pointing to the terminal node is return.
	// If not, NULL is return.
	if (root->count >= 1)
		return root;
	
	return NULL;
	
}

// Auxiliary function that is pretty much just the getNode() function, but instead
// of returning a pointer at the end of a root word, it returns a pointer at the root subtrie.
TrieNode *findSubtrie(TrieNode *root, char *str)
{
	// Add a null terminator to the end of the string.
	str[strlen(str)] = '\0';
	
	root = getNode(root, str);
	
	if (root != NULL)
		return root->subtrie;
	
	return NULL;
}

// Similar to printTrieHelper written by Dr.Szumlanski.
// Helper function for the getMostFrequentWord() function that use backtracking in
// Dr.Szumlanski printTrieHelper() function to travel down and back up all 26 root children
// of the trie and store each word and count going down and deleting them if the 
// root count is not greater than what have been seen, which is stored in max, 
// coming back up.
void getMostFrequentWordHelper(TrieNode *root, char *buffer, int k, int *max, char *freq)
{
	int i;
	
	// Base case to exit the recurtion.
	if (root == NULL)
		return;
	
	// Update max counter and copy the current string that have the
	// greatest root count seen so far into another array.
	if (root->count > *max)
	{
		strcpy(freq, buffer);
		*max = root->count;
	}
	
	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		getMostFrequentWordHelper(root->children[i], buffer, k + 1, max, freq);
	}

	buffer[k] = '\0';
}

// Function calls getMostFrequentWordHelper() function to search the trie for the most
// frequently occurring word, copying what was return into the string variable passed to the function.
void getMostFrequentWord(TrieNode *root, char *str)
{
	int max = 0, *p = &max;
	char freq[MAX_CHARACTERS_PER_WORD + 1];
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	
	// Incase str contain garbage data
	str[0] = '\0';
	
	if (root == NULL)
	{
		strcpy(str, "");
		return;
	}
	
	getMostFrequentWordHelper(root, buffer, 0, p, freq);
	
	// Copy the updated freq array into str and return.
	strcpy(str, freq);
	return;
}

// Function searches the trie for the specified string and return 1 if found and 0 otherwise.
// Reuse getNode() function.
int containsWord(TrieNode *root, char *str)
{	
	if (getNode(root, str) != NULL)
		return 1;
	
	return 0;
}

// Auxiliary function that acts as helper for prefixCount() function.
// Count how many string is in the trie given the root and return the total. 
int countWord(TrieNode *root)
{
	int total = 0, i;
	
	// Base case to exit recursion.
	if (root == NULL)
		return 0;
	
	total = root->count;

	// Traverse down each root children adding the root count to the total
	// on the way back up. 
	for (i=0; i<26; i++)
		total += countWord(root->children[i]);
	
	return total;
	
}

// Function counts the number of strings in the trie that begin with the 
// specified string.
int prefixCount(TrieNode *root, char *str)
{
	TrieNode *temp, *sub;
	int next, i, n = strlen(str);
	
	if (root == NULL)
		return 0;
	
	// Traverse through the trie for the prefix.
	// If found, the pointer the pointing to the terminal node of the prefix
	// is given to countWord() function to figure out how many words
	// follow the prefix.
	// If not found, 0 is returned.
	for (i=0; i<n; i++)
	{
		next = tolower(str[i]) - 'a';
		
		if (root->children[next] != NULL)
			root = root->children[next];
		
		else return 0;
	}
	
	return countWord(root);
}

// Function returns a double from 1.0 to 5.0 according to what the hoursSpent() function returns.
double difficultyRating(void)
{
	if (hoursSpent() >= 25.0)
		return 4.8; 
	else return 4.5;
}

// Function returns a double depending on how many hours is spent on the assignment. 
double hoursSpent(void)
{
	return 30.0;	
}

// Function capture the names of the corpus and input files (passed as a command line argument),
// calls buildTrie(), processInputFile(), and destroyTrie() functions, then return 0. 
int main(int argc, char **argv)
{
	TrieNode *root;
	
	if (argc < 3)
	{
		fprintf(stderr, "Proper syntax: %s <corpus file name> <input file name>\n", argv[0]);
		return 1;
	}
	
	root = buildTrie(argv[1]);
	processInputFile(root, argv[2]);
	root = destroyTrie(root);
	
	return 0;
}