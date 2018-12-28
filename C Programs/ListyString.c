// Sopheap Sok
// 10/26/2017
// Program: ListyString
// Program implement linked lists to represent strings. Many functions are included that manipulate these linked lists
// to transform the strings they represent. Functions such as: processInputFile, createListyString, destroyListyString,
// cloneListyString, replaceChar, reverseListyString, listyCat, listyCmp, listyLength, and printListyString are included.
// Extra functions are implemented to aid some of the functions mentioned above. These auxiliary functions includes:
// head_insert, destroy_list, and listy_to_str. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

// Functional Prototypes
ListyString *head_insert(ListyString *listy, char data);
ListyNode *destroy_list(ListyNode *head);
char *listy_to_str(ListyString *listy);

// Function read and process the input file passed in, calling other functions as needed 
// to perform the string manipulations.
int processInputFile(char *filename)
{
	FILE *fpt;
	char buffer[1026];
	ListyString *listy; 
	char key[2];

	// Open and read the file in.
	fpt = fopen(filename, "r");
	
	// Check for bad filename being passed in.
	if (fpt == NULL)
		return 1;
	
	// Transfer file data into a buffer.
	fscanf(fpt, "%s", buffer);
	
	// Call createListyString to convert the string in buffer to a linked list.
	listy = createListyString(buffer);
	
	// Transfer file data again into buffer for processing. 
	while (fscanf(fpt, "%s", buffer) != EOF)
	{	
		switch (buffer[0])
		{	
			// When the @ symbol is encounter, the key and the string needed for replacement
			// is scanned in and sent off to replaceChar with the linked list created.
			case '@':
				fscanf(fpt, "%s %s", key, buffer);
				replaceChar(listy, key[0], buffer);
				break;
			
			// When the + symbol is encounter, the string need for concatenation is scanned in
			// and sent off to listyCat with the linked list created.
			case '+':
				fscanf(fpt, "%s", buffer);
				listyCat(listy, buffer);
				break;
			
			// When the - symbol is encounter, the key need for deletion is scanned in
			// and sent off to replaceChar with the linked list created as well as an
			// empty string "".
			case '-':
				fscanf(fpt, "%s", key);
				replaceChar(listy, key[0], "");
				break;
			
			// When the ~ symbol is encounter, the linked list is sent off to the
			// reverseListyString function.
			case '~':
				reverseListyString(listy);
				break;
				
			// When the ? symbol is encounter, the linked list is sent off to the
			// listyLength function for length identification, then return to be print
			// out to the screen.	
			case '?':
				listyLength(listy);
				printf("%d\n", listy->length);
				break;
			
			// When the ! symbol is encounter, the linked list is sent off to the
			// printListyString function to print out.			
			case '!':
				printListyString(listy);
				break;
				
			default:
				printf("Error! No string manipulations found.\n");
				break;
			
		}
	}	
	return 0;
}

// Function is an auxiliary function to aid createListyString and reverseListyString functions. 
// Function takes a linked list and a character and create a new node and insert the character
// into the linked list and move the head pointer to the new head.
ListyString *head_insert(ListyString *listy, char data)
{
	if (listy == NULL)
		return NULL;
	
	// Dynamically allocate space for the new head node. 
	ListyNode *new_head = malloc(sizeof(ListyNode));
	
	// Insert data into the new node and connect to the linked list, letting the head pointer 
	// equal to the new head.
	new_head->data = data;
	new_head->next = listy->head;
	listy->head = new_head;
	
	
	return listy;	
}

// Function convert str to a linked list.
ListyString *createListyString(char *str)
{	
	int n, i;
	
	// Dynamically allocate space for the linked list and set the head equal to NULL and 
	// the length equal to 0.
	ListyString *listy = calloc(1, sizeof(ListyString));
	
	if (str == NULL || str == "")
		return listy;
	
	// Store the length of the str into n for use in the for loop.
	n = strlen(str);
	
	// If head_insert is given str in order the linked list will represent the reverse of str.
	// Due to this, the character are given backwards. 
	for (i=(n-1); i>=0; i--)
	{	
		listy = head_insert(listy, str[i]);
	}
	
	// Update the length of the linked list.
	listy->length = n;
	return listy;
}

// Auxiliary code from fancy-linked-lists.c. 
// Function takes the head of a linked list and frees all the nodes in that list, and returns NULL.
ListyNode *destroy_list(ListyNode *head)
{
	if (head == NULL)
		return NULL;

	destroy_list(head->next);
	free(head);

	return NULL;
}

// Code from fancy-linked-lists.c.  
// Takes a LinkedList struct and frees all the dynamically allocated memory
// associated with that struct -- the linked list inside it, as well as the
// struct itself.
ListyString *destroyListyString(ListyString *listy)
{

	if (listy == NULL || listy->head == NULL)
		return NULL;
	
	destroy_list(listy->head);
	free(listy);
	
	return NULL;
}

// Auxiliary function that takes a linked list and convert it back into an array of characters.
char *listy_to_str(ListyString *listy)
{
	int i, j;
	ListyNode *tempt;
	
	if (listy == NULL)
		return NULL;
	
	// Return an empty string is linked listy is empty.
	if (listy->head == NULL && listy->length == 0)
		return "";
	
	// Dynamically allocate space for the array.
	char *str = malloc(sizeof(char)*(listy->length+1));
	
	// Set the last array block to NULL.
	str[listy->length] = '\0';

	// Loop through the linked list and transfer the data into the array.
	for (i=0; i<listy->length; i++)
	{
		tempt = listy->head;
		
		for (j=0; j<i; j++)
		{
			tempt = tempt->next; 
		}
		
		str[i] = tempt->data;
	}
		
	return str;
}

// Function create a copy of the linked list listy by making use of the listy_to_str and
// createListyString functions.
ListyString *cloneListyString(ListyString *listy)
{	
	ListyString *listy2;
	
	if (listy == NULL)
		return NULL;
	
	// Listy is first sent to listy_to_str to convert into an array, then gets sent off to
	// the createListyString function to make into a new linked list.
	return listy2 = createListyString(listy_to_str(listy));
}

// This function takes a linked list and replaces all instances of a certain character
// (key) with the specified string (str). If str is NULL or the empty string (“”),
// this function simply removes all instances of key from the linked list. 
void replaceChar(ListyString *listy, char key, char *str)
{
	ListyNode *temp, *temp2, *head;
	int i, n;
	
	if (listy == NULL || listy->head == NULL)
		return;
	
	// When str is "" or NULL, all instances of the key is removed from the linked list.
	if (str[0] == '\0' || str == NULL)
	{	
		// If there is only one node in the linked list and it is equal to the key,
		// the node is free and the head pointer is set to NULL and length set to 0.
		if (listy->head->data == key && listy->head->next == NULL)
		{
			temp = listy->head;
			listy->head = NULL;
			free(temp);
			listy->length = 0;
			
			return;
		}
		
		// Removes all instances of the key if found in the head and keep moving the head pointer
		// to the next node in the linked list until the head does not contain the key.
		while (listy->head->data == key)
		{
			
			temp = listy->head;
			listy->head = listy->head->next;
			free(temp);
			
			listy->length -=1;
		}
		
		// The next two while loop will together loop through the linked listy and remove all
		// instances of the key not found in the head node.
		
		// Removes all instances of the key in every other node starting after the head node.
		temp = listy->head->next;
		while (temp != NULL && temp->next != NULL)
		{
			if (temp->next->data == key)
			{
	
				temp2 = temp->next;
				temp->next = temp2->next;
				free(temp2);
				listy->length -= 1;
			}
			temp = temp->next;
		}	
		
		// Removes all instances of the key in every other node starting at the head node.
		temp = listy->head;
		while (temp != NULL && temp->next != NULL)
		{
			if (temp->next->data == key)
			{
	
				temp2 = temp->next;
				temp->next = temp2->next;
				free(temp2);
				listy->length -= 1;
			}
			temp = temp->next;
		}
		
	}
	
	// If str contains characters, this will loop through the linked list and replace all instances
	//	of the key with the str.
	else
	{
		n = strlen(str);
		head = listy->head;
		
		// Loops through till the end of the list.
		while (head != NULL)
		{
			if (head->data == key)
			{	
				// Replace the node data with the first new character.
				head->data = str[0];
				
				// If more than one character is stored in str, new node is created and the next
				// character is inserted and connect to the linked list.
				for (i=1; i<n; i++)
				{
					temp = malloc(sizeof(ListyNode));
					
					temp->data = str[i];
					temp->next = NULL;
					
					temp->next = head->next;
					head->next = temp;
					head = head->next;
					listy->length += 1;

				}
				if (n>1)
					head = temp;
			}
		
			head = head->next;
		}
		
	}
}

// Function reverse the linked list contained within listy by making use of listy_to_str
// and head_insert funtions.
void reverseListyString(ListyString *listy)
{
	char *str;
	int n, i;
	
	if (listy == NULL || listy->head == NULL)
		return;
	
	// Use listy_to_str to convert the linked list into an array.
	str = listy_to_str(listy);
	
	// Free the old listy and make an new one, where head is NULL and length is 0.
	free(listy);
	listy = calloc(1, sizeof(ListyString));
	
	n = strlen(str);
	
	// Make use of head_insert function to reverse the str and store in the new listy.
	for (i=0; i<n; i++)
	{	
		listy = head_insert(listy, str[i]);
	}
	
	// Update the length of the linked list.
	listy->length = n;
}

// Function concatenate str to the end of the linked list string inside listy.
ListyString *listyCat(ListyString *listy, char *str)
{
	ListyNode *temp, *tail;
	int n, i;
	
	if (listy != NULL && str == NULL)
		return listy;
	
	else if (listy == NULL && str == NULL)
		return NULL;
	
	else if (listy == NULL && str == "")
		return createListyString(str);
	
	else if (listy == NULL)
		return createListyString(str);
	
	else if (listy->head == NULL && listy->length == 0 && str == NULL)
		return listy;
	
	// If list is empty and str is not, the str is inserted into the empty list.
	else
	{	
		if (listy->head == NULL && listy->length == 0 && str != NULL)
		{
			ListyNode *new_node;
			
			new_node = malloc(sizeof(ListyNode));
			
			new_node->data = str[0];
			new_node->next = NULL;
			listy->head = new_node;
			
			// Loop to the end of the list and set a tail pointer.
			for (temp = listy->head; temp->next != NULL; temp = temp->next)
				;	
			tail = temp;
			
			n = strlen(str);
			
			// Tail insertion of the str.
			for (i=1; i<n; i++)
			{
				temp = malloc(sizeof(ListyNode));
				
				temp->data = str[i];
				temp->next = NULL;
				
				tail->next = temp;
				tail = tail->next;
			}
			
			// Update length of list.
			listy->length = n;
		}
		
		// If linked list is not empty and str is not empty, this create new notes and insert
		// the new characters into the linked list.
		else 
		{
			// Loop to the end of the list and set a tail pointer.
			for (temp = listy->head; temp->next != NULL; temp = temp->next)
				;	
			tail = temp;
			
			n = strlen(str);
			
			// Tail insertion of the str.
			for (i=0; i<n; i++)
			{	
				temp = malloc(sizeof(ListyNode));
				
				temp->data = str[i];
				temp->next = NULL;
				
				
				tail->next = temp;
				tail = tail->next;
				
			}
			
			// Update length of list.
			listy->length = listy->length + n;

		}
	}
	
	return listy;
	
}

// Function compare the two ListyStrings. Return 0 (zero) if they represent equivalent strings. 
int listyCmp(ListyString *listy1, ListyString *listy2)
{
	char *str1, *str2;
	
	if (listy1 == NULL && listy2 == NULL)
		return 0;
	
	if (listy1 == NULL && listy2->head == NULL || listy1->head == NULL && listy2 == NULL)
		return 1;
	
	// Convert both linked list to arrays.
	str1 = listy_to_str(listy1);
	str2 = listy_to_str(listy2);
	
	// Make use of strcmp to compair the strings.
	return strcmp(str1, str2);
}

// Return the length of the ListyString.
int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	
	if (listy->head == NULL)
		return 0;
	
	return listy->length;
	
}

// Code from linked-lists.c
// Function print the string stored in listy, followed by a newline character, ‘\n’. 
void printListyString(ListyString *listy)
{
	if (listy == NULL || listy->head == NULL)
	{
		printf("(empty string)\n");
		return;
	}
	
	else
	{
		ListyNode *temp = listy->head;
		
		for ( ; temp != NULL; temp = temp->next) 
			printf("%c", temp->data);
		printf("\n");
		return;
	}
}

// Function capture the name of an input file (passed as a command line argument),
// call the processInputFile() function, then return 0. 
int main(int argc, char **argv)
{
	
	// Code from MergeSort.c
	if (argc < 2)
	{
		fprintf(stderr, "Proper syntax: %s <input file name>\n", argv[0]);
		return 1;
	}
	

	processInputFile(argv[1]); 
	
	return 0;
}























