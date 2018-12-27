// Sopheap Sok
// so683950
// CIS 3360 0R02, Spring 2018
// 2/5/2018
// Programming Assignment 1: Encryption and Decryption using Vigenere with Cipher Block Chaining (CBC) 
// Encryption source code

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_BLOCK_SIZE 10
#define MAX_CHARACTERS 4096

void stripPunctuators(char *str);
char *processInputFile(char *filename);
void toCiphertext(char *plaintext, int blockSize, char *key, char *IV);
void paddingX(char *plaintext, int blockSize);



int main(int argc, char **argv)
{
	char key[MAX_BLOCK_SIZE + 1];
	char IV[MAX_BLOCK_SIZE + 1];
	int blockSize;
	char *plaintext = malloc(sizeof(char)*(MAX_CHARACTERS + 1));

	// Make sure the command line syntax is correct.
	if (argc < 4)
	{
		printf("Please input your text file, key, and IV, in respected order, for encryption.\nKey and IV must be all lower case alphabetic characters, ranging from 2-10 characters long, and both be the same length.\n");

		fprintf(stderr, "Proper syntax: %s <input file name> <key> <IV>\n", argv[0]);
		
	}
	
	else
	{		
		strcpy(key, argv[2]);
		strcpy(IV, argv[3]);
		
		// Make sure the length of the key and the IV is the same, otherwise ask user to enter new ones.
		if (strlen(key) != strlen(IV))
		{
			printf("Key and IV lengths do not match:\nKey: %s\n IV: %s\nEnter key and IV again.\n", key, IV);
			scanf("%s %s", key, IV);
		}
		
		// Find out the length of the key for it is the block size.
		blockSize = strlen(key);
		
		printf("CBC Vigenere Encryption\n");
		printf("Plaintext file: %s\nKey: %s\nIV: %s\nBlock size: %d\n\n", argv[1], key, IV, blockSize); 
		
		// Send the text file capture from the command line to processInputFile and capture the char array sent back.
		plaintext = processInputFile(argv[1]);
		
		// Add Null to the end of key and IV array.
		key[blockSize] = '\0';
		IV[blockSize] = '\0';
		
		// Send the char array and blockSize to paddingX for padding with x if necessary.
		paddingX(plaintext, blockSize);
		
		// Send the now padded array, blockSize, key, and IV to toCiphertext to produce the encryption.
		toCiphertext(plaintext, blockSize, key, IV);
	}
	
	return 0;
}

// Function takes a file, scan it in to a buffer and process it.
char *processInputFile(char *filename)
{
	FILE *input;
	char buffer[MAX_CHARACTERS];
	int i, n = 0, j = 0;
	char *plaintext = calloc((MAX_CHARACTERS + 1), sizeof(char));
	
	// Make sure the file is an actual file.
	if ((input = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in processInputFile().\n", filename);
		return NULL;
	}
	
	// Scan from file to buffer and lower all capital letters and strip all punctuations, then continuously insert into a char array.
	while (fscanf(input, "%s", buffer) != EOF)
	{
		
		while(buffer[i]) 
		{
			buffer[i] = tolower(buffer[i]);
			i++;
		}
		
		i = 0;
		
		stripPunctuators(buffer);
		
		
		while(buffer[i]) 
		{
			plaintext[j++] = buffer[i];
			i++;
		}
		
		i = 0;
		
		n += strlen(buffer);	
	}
	
	// Print to screen the processed plaintext and its length, then close the file. 
	printf("Plaintext: %s\n", plaintext);
	printf("\nNumber of plaintext characters: %d", n );
	fclose(input);
	
	return plaintext;
}

// Auxiliary function that removes all punctuation from the string.
void stripPunctuators(char *str)
{
	int i, n, j = 0;
	char *buffer = malloc(sizeof(char)*(MAX_CHARACTERS + 1));
	
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

// Pad the plaintext with x if needed.
void paddingX(char *plaintext, int blockSize)
{
	// Find out if the text need to be padded.
	int l = strlen(plaintext);
	int n = l % blockSize;
	
	// Padding is not needed, print out zero and return.
	if (n == 0)
	{
		printf("\nPadding added: %d\n\n", n);
		return;
	}	
	
	// Find out how many x is needed for padding.
	int x = blockSize - n;
	int i;
	char addx = 'x';
	
	// Add the x to the end of the plaintext array and adding a Null at the end.
	for (i=0; i<x; i++)
		plaintext[l++] = addx;
	plaintext[l] = '\0';
	printf("\nPadding added: %d\n\n", x);
	
	return;
}

// Function takes the plaintext and encrypt it.
void toCiphertext(char *plaintext, int blockSize, char *key, char *IV)
{
	FILE *ciphertext;
	char previous[MAX_BLOCK_SIZE + 1], ch, current;
	int n, i = 0, j, C;
	
	// Open the file ciphertext.txt to write to.
	ciphertext = fopen("ciphertext.txt", "w");
	
	printf("Ciphertext: ");
	
	// Perform the first encryption algorithm where the first block of characters, key, and IV is used.
	for (j=0; j<blockSize; j++)
	{
		previous[j] = plaintext[i++];
		
		C = ((previous[j] - 'a') + (key[j] - 'a') + (IV[j] - 'a')) % 26;
		ch = C + 'a';
		
		// Insert each character to file and also print to screen.
		fprintf(ciphertext, "%c", ch);
		printf("%c", ch);
		
		// Capture the first block of ciphertext to be use.
		previous[j] = ch;
		
	}
	
	C = 0;
	n = strlen(plaintext);
	
	// Continue the algorithm using the previous block of ciphertext instead of the IV.
	for (i=(blockSize); i<n;)
	{
		for (j=0; j<blockSize; j++)
		{	
			current = plaintext[i++];
			
			C = ((current - 'a') + (previous[j] - 'a') + (key[j] - 'a')) % 26;
			ch = C + 'a';
			
			// Continue to print each ciphertext to file and screen.
			fprintf(ciphertext, "%c", ch);
			printf("%c", ch);
		
			// Capture the block of ciphertext for use in the next iteration. 
			previous[j] = ch;
			C = 0;
		}		
	}
	
	printf("\n\nName of ciphertext file: ciphertext.txt\n");
	
	// Close the file written to and free the plaintext array.
	fclose(ciphertext);
	free(plaintext);
	return;
}




























