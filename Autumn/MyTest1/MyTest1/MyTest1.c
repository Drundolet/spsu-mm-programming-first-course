// MyTest1.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


typedef struct ArrayOfArray
{
	int oneArray[5];
	int countOfElement;
	struct MyArray *next;
} arrayOfArray;


void addNewArray(arrayOfArray **first)
{
	if (*first == NULL)
	{
		arrayOfArray *newFirst = (arrayOfArray*)malloc(sizeof(arrayOfArray));
		newFirst->next = NULL;
		newFirst->countOfElement = 0;
		int newOneArray[5];
		*newFirst->oneArray = newOneArray;
		
		*first = newFirst;
		
		return;
	}

	arrayOfArray *temp = *first;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	arrayOfArray *newElement = (arrayOfArray*)malloc(sizeof(arrayOfArray));
	newElement->next = NULL;
	newElement->countOfElement = 0;
	int newOneArray[5];
	*newElement->oneArray = newOneArray;
	
	temp->next = newElement;
}

int getValueOfIndex(arrayOfArray *first, int index)
{
	int findArray = index / 5;
	int findIndex = index - findArray*5;

	arrayOfArray *temp = first;
	int i = 0;
	while (i < findArray)
	{
		i++;
		temp = temp->next;
	}

	return temp->oneArray[findIndex];
}

void addToEnd(arrayOfArray **first, int value)
{
	if (*first == NULL)
	{
		addNewArray(&*first);
		(*first)->oneArray[0] = value;
		(*first)->countOfElement++;
		
		return;
	}

	arrayOfArray *temp = *first;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	if (temp->countOfElement < 5)
	{
		temp->oneArray[temp->countOfElement] = value;
		temp->countOfElement++;
	}
	else
	{
		addNewArray(&*first);
		temp = temp->next;
		temp->oneArray[0] = value;
		temp->countOfElement++;
	}
}

void print(arrayOfArray *first)
{
	arrayOfArray *temp = first;
	while (temp->next != NULL)
	{
		int i = 0;
		while (i < temp->countOfElement)
		{
			printf("%d", temp->oneArray[i]);
			putchar(' ');
			i++;
		}
		temp = temp->next;
	}
	
	if (temp != NULL)
	{
		int i = 0;
		while (i < temp->countOfElement)
		{
			printf("%d", temp->oneArray[i]);
			putchar(' ');
			i++;
		}
	}
}

int main()
{
	arrayOfArray *testArray = NULL;        // Test
	addToEnd(&testArray, 1);
	addToEnd(&testArray, 2);
	addToEnd(&testArray, 3);
	addToEnd(&testArray, 4);
	addToEnd(&testArray, 5);
	addToEnd(&testArray, 6);
	addToEnd(&testArray, 7);
	addToEnd(&testArray, 8);
	addToEnd(&testArray, 9);
	addToEnd(&testArray, 10);
	addToEnd(&testArray, 11);
	addToEnd(&testArray, 12);
	print(testArray);
	putchar('\n');
	
	int k = getValueOfIndex(testArray, 3);
	printf("%d", k);
	putchar(' ');
	k = getValueOfIndex(testArray, 0);
	printf("%d", k);
	putchar(' ');
	k = getValueOfIndex(testArray, 5);
	printf("%d", k);
	putchar(' ');
	k = getValueOfIndex(testArray, 8);
	printf("%d", k);
	
	return 0;
}

