// Sopheap Sok
// so683950
// CIS 3360 0R02, Spring 2018
// 2/10/2018
// Programming Assignment 1: Encryption and Decryption using Vigenere with Cipher Block Chaining (CBC) 
// Decryption source code

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_BLOCK_SIZE 10
#define MAX_CHARACTERS 4096

char *processInputFile(char *filename);
void toPlaintext(char *ciphertext, int blockSize, char *key, char *IV);
int mod(int a, int b);

int main(int argc, char **argv)
{
	char key[MAX_BLOCK_SIZE + 1];
	char IV[MAX_BLOCK_SIZE + 1];
	int blockSize;
	char *ciphertext = malloc(sizeof(char)*(MAX_CHARACTERS + 1));

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
		
		printf("CBC Vigenere Decryption\n");
		printf("Ciphertext file: %s\nKey: %s\nIV: %s\nBlock size: %d\n\n", argv[1], key, IV, blockSize); 
		
		// Send the text file capture from the command line to processInputFile and capture the char array sent back.
		ciphertext = processInputFile(argv[1]);
		
		// Add Null to the end of key and IV array.
		key[blockSize] = '\0';
		IV[blockSize] = '\0';
		
		// Send the now ciphertext array, blockSize, key, and IV to toPlaintext to be decrypted.
		toPlaintext(ciphertext, blockSize, key, IV);
	}
	
	return 0;
}

// Function takes a file, scan it in to a buffer and process it.
char *processInputFile(char *filename)
{
	FILE *input;
	int n;
	char *ciphertext = calloc((MAX_CHARACTERS + 1), sizeof(char));
	
	// Make sure the file is an actual file.
	if ((input = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in processInputFile().\n", filename);
		return NULL;
	}
	
	// Scan the string from the file into an array.
	fscanf(input, "%s", ciphertext);
	
	// Find out the length of the ciphertext.
	n = strlen(ciphertext);
	
	// Print out the ciphertext and its length to the screen and clsoe the file.
	printf("Ciphertext: %s\n", ciphertext);
	printf("\nNumber of ciphertext characters: %d\n\n", n );
	fclose(input);
	
	return ciphertext;
}

// Function takes the ciphertext and decrypt it.
void toPlaintext(char *ciphertext, int blockSize, char *key, char *IV)
{
	char previous[MAX_BLOCK_SIZE + 1], ch, current;
	int n, i = 0, j, C, P;
	
	printf("Plaintext: ");
	
	// Perform the first decryption algorithm where the first block of characters, key, and IV is used.
	for (j=0; j<blockSize; j++)
	{
		previous[j] = ciphertext[i++];
		
		C = (previous[j] - 'a') - (key[j] - 'a') - (IV[j] - 'a');
		
		// Send the value store in C to be mod by 26.
		P = mod(C, 26);
		
		ch = P + 'a';
		
		// Print each character onto the screen.
		printf("%c", ch);
		
	}
	
	P = 0;
	C = 0;
	n = strlen(ciphertext);
	
	// Continue the algorithm using the previous block of ciphertext instead of the IV.
	for (i=(blockSize); i<n;)
	{
		for (j=0; j<blockSize; j++)
		{	
			current = ciphertext[i++];
			
			C = (current - 'a') - (key[j] - 'a') - (previous[j] - 'a');	
			P = mod(C, 26);
		
			ch = P + 'a';
			
			printf("%c", ch);
		
			// Capture the block of ciphertext for use in the next iteration. 
			previous[j] = current;
			C = 0;
			P = 0;
		}		
	}
	printf("\n");
	
	// Free the dynamically allocated memory. 
	free(ciphertext);
	return;
}

// Function mod to values.
int mod(int a, int b)
{
	return (a % b + b) % b;
}
