// Huy Pham
// COP 3502, Spring 2019
// hu079378	

#include "ListyString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create node function
ListyNode *create_ListyNode(char data)
{
	ListyNode *node = malloc(sizeof(ListyNode));
	if (node == NULL)
		return NULL;

	node->data = data;
	node->next = NULL;
	return node;
	
}

int main (int argc, char **argv)
{
	processInputFile(argv[1]);
	return 0;
}

int processInputFile(char *filename)
{
	// variables
	char buffer[1024], str[1024], key[1];
	FILE *ifp = NULL;
	ListyString *LS;
	int length;
	
	if (filename == NULL)
		return 1;
	
	// check if file fails to open
	if ((ifp = fopen(filename, "r")) == NULL)
		return 1;
	
	// scan in the string
	// make string into list
	fscanf(ifp, "%s", str);
	LS = createListyString(str);
	
	// scan through and check each command
	// modify strings based on each command
	while (fscanf(ifp, "%s", buffer) != EOF)
	{
		if (buffer[0] == '@')
		{
			fscanf(ifp, "%s%s", key, str);
			replaceChar(LS, key[0], str);
		}
		if (buffer[0] == '+')
		{
			fscanf(ifp, "%s", str);
			listyCat(LS, str);
		}
		if (buffer[0] == '-')
		{
			fscanf(ifp, "%s", key);
			replaceChar(LS, key[0], NULL);
		}
		if (buffer[0] == '~')
			reverseListyString(LS);
			
		if (buffer[0] == '?')
		{
			length = listyLength(LS);
			printf("%d", length);
		}
		if (buffer[0] == '!')
		{
			printListyString(LS);
		}
	}
	
	// close file and set pointer to NULL
	fclose(ifp);
	ifp = NULL;
	return 0;
}

ListyString *createListyString(char *str)
{
	// variables
	int i, str_len;
	ListyString *LS;
	ListyNode *temp;
	
	// dynamically allocate space
	LS = malloc(sizeof(ListyString));
	
	// check if string was empty or NULL
	if (str == NULL || str == "" || strlen(str) == 0)
	{
		LS->head = NULL;
		LS->length = 0;
		return LS;
	}
	str_len = strlen(str);
	
	// creating the head
	// loop through and create the rest
	LS->head= create_ListyNode(str[0]);
	temp = LS->head;
	for (i = 1; i < str_len; i++)
	{	
		temp->next = create_ListyNode(str[i]);
		temp= temp->next;
	}
	// update length
	LS->length = str_len;
	return LS;
	
}

ListyString *destroyListyString(ListyString *listy)
{
	// variables
	ListyNode *temp, *current;
	int i;
	
	if (listy == NULL || listy->head == NULL)
		return NULL;
	
	temp = listy->head;
	// loop through and free each node
	for (i = 0; i < listy->length; i++)
	{
		current = temp;
		temp = temp->next;
		free(current);
	}
	// free the struct 
	free(listy);
	
	return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
	// variables
	int i;
	ListyNode *temp, *ctemp;
	
	if (listy == NULL)
		return NULL;
	
	// dynamically allocate the clone list
	ListyString *clone = malloc(sizeof(ListyString));
	
	if (clone == NULL)
		return NULL;
	
	// copy the content of the original list into clone list
	clone->head = create_ListyNode(listy->head->data);
	temp = listy->head;
	ctemp = clone->head;
	for (i = 1; i < listy->length ; i++)
	{
		temp = temp->next;
		ctemp->next = create_ListyNode(temp->data);
		ctemp = ctemp->next;
		
	}
	// copy the length over
	clone->length = listy->length;
	return clone;
}

void replaceChar(ListyString *listy, char key, char *str)
{

	// variables
	ListyNode *temp, *forward, *previous, *current;
	int str_len, i, counter = 0;
	
	if (listy == NULL || listy->head == NULL)
		return;
	
	// if deleting 
	if (str == NULL || str == "" || strlen(str) == 0)
	{	
	
		temp = listy->head;
		while (temp != NULL)
		{
			// if at the start of the list
			// delete node and set the new head
			if (listy->head->data == key)
			{
				current = listy->head;
				listy->head = listy->head->next;
				free(current);
				listy->length --;
				temp = listy->head;
			}
			
			// if anywhere but the head
			// delete and link the previous and next nodes
			else if (temp->next->data == key)
			{
				forward = temp->next->next;
				previous = temp;
				free(temp->next);
				listy->length --;
				previous->next = forward;
				temp = previous;
			}
			// loops through
			temp = temp->next;
			
		}
	}
	// if replacing
	else
	{
		temp = listy->head;
		str_len = strlen(str);
		while (temp != NULL)
		{
			// if the head is equal to key
			// loops through only once
			// makes list at head
			if (listy->head->data == key && counter != 1)
			{
				current = listy->head;
				forward = listy->head->next;
				listy->head = create_ListyNode(str[0]);
				temp = listy->head;
				for (i = 1; i < str_len; i++)
				{
					temp->next = create_ListyNode(str[i]);
					temp = temp->next;
	
				}
				
				// update length
				listy->length += (str_len - 1);
				// combine lists original list
				temp->next = forward;
				// counter to only loop through once
				counter = 1;
				// free old node 
				free(current);
			}
			// if the key isn't in the start of the list
			// makes new list at the key
			// links the previous and next nodes
			else if (temp->data == key)
			{
				current = temp;
				previous->next = create_ListyNode(str[0]);
				forward = previous->next;
				
				for (i = 1; i < str_len; i++)
				{
					forward->next = create_ListyNode(str[i]);
					forward = forward->next;
					
				}
				forward->next = temp->next;
				free(current);
				listy->length += (str_len-1);
				temp = forward;
			}
			// holds the previous as it loops
			previous = temp;
			temp = temp->next;
		
		}
	}
} 

void reverseListyString(ListyString *listy)
{
	// variables
	ListyNode *current, *previous, *next;
	
	if (listy == NULL || listy->head == NULL || listy->length <= 1)
		return ;
	
	current = listy->head; 
	previous = NULL;
	
	while (current != NULL) 
	{
		// proceed to next
		next = current->next;
		// my current point behind it
		current->next = previous;
		// move previous forward
		previous = current; 
		// move current to next
		current = next; 
	} 
	// set tail to head
	listy->head = previous;  
}

ListyString *listyCat(ListyString *listy, char *str)
{
	// variables
	int i, str_len;
	ListyNode *temp;

	
	if (str == NULL || str == "")
		return listy;
	
	if (listy == NULL  && str == NULL)
		return NULL;
	
	if (listy == NULL && str != "")
		return createListyString(str);

	if (listy == NULL && str == "")
		return createListyString("");
	
	
	str_len = strlen(str);
	// if there was no previous list
	// make new list based on str
	if (listy->head == NULL)
	{
		
		listy->head = create_ListyNode(str[0]);
		temp = listy->head;
				
		for (i = 1; i < str_len; i++)
		{
			temp->next = create_ListyNode(str[i]);
			temp = temp->next;
			
		}
		listy->length = str_len;
		
		return listy;
	}
	// if there was a previous list
	// add on to previous list
	if (listy->head != NULL)
	{
		temp = listy->head;
		
		while (temp->next != NULL)
			temp = temp->next;
		
		for (i = 0; i < str_len; i++)
		{
			temp->next = create_ListyNode(str[i]);
			temp = temp->next;
		}
		listy->length += str_len;
	}
	
	return listy;
	
}

int listyCmp(ListyString *listy1, ListyString *listy2)
{
	// variables
	int i;
	ListyNode *temp, *temp2;
	
	if (listy1 == NULL && listy2 == NULL)
		return 0;
	
	if (listy1 == NULL || listy2 == NULL)
		return 1;
	
	if (listy1->head == NULL && listy2->head == NULL)
		return 0;
	
	if (listy1->head == NULL || listy2->head == NULL)
		return 1;
	
	// loop through both lists and compare values
	if ( listy1->length == listy2->length)
	{	
		temp = listy1->head;
		temp2 = listy2->head;
		
		for (i = 0; i < listy1->length; i++)
		{
			if (temp->data != temp2->data)
				return 1;

			temp = temp->next;
			temp2 = temp2->next;
		}
		return 0;
	}
	return 1;
}

int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	
	if(listy->head == NULL)
		return 0;
	
	return listy->length;
}

void printListyString(ListyString *listy)
{
	// variables
	ListyNode *temp;
	
	if (listy == NULL || listy->length == 0)
	{
		printf("(empty string)\n");
		return;
	}
	//loop through and print out list
	for (temp = listy->head; temp != NULL; temp = temp->next) 
		printf("%c", temp->data);
		printf("\n");
}

double difficultyRating(void)
{
	return 3.0;
}

double hoursSpent(void)
{
	return 30.0;
}





