#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

// The definition of the ADT "object", it contains its size, its array as well as it elementary functions
typedef struct AdptArray_
{
    int size;
    PElement* elementArr;
    COPY_FUNC copier;
    DEL_FUNC deleter;
    PRINT_FUNC printer;
}AdptArray; // , *PAdptArray;


// this function creates an ADT, it gets 3 elemental functions according to the desired element
PAdptArray CreateAdptArray(COPY_FUNC copierFunc, DEL_FUNC deleterFunc,PRINT_FUNC printerFunc)
{
    PAdptArray ADT = (PAdptArray)malloc(sizeof(AdptArray));
    if (ADT == NULL)
    {
        return FAIL;
    }
    ADT->size = 0;
    ADT->copier = copierFunc;
    ADT->deleter = deleterFunc;
    ADT->printer = printerFunc;
    ADT->elementArr = NULL;
    return ADT;
    
}

// this function deletes the entire ADT, as well as its elements
void DeleteAdptArray(PAdptArray ADT)
{
    if (ADT == NULL)
    {
        return; // no ADT to remove
    }
    PElement curr;
    for (int i = 0; i < ADT->size; i++)
    {
        curr = (ADT->elementArr)[i];
        if (curr != NULL)
        {
            ADT->deleter(curr); // using build-in deleter
        }
    }
    free(ADT->elementArr);
    free(ADT);
}

// inner function that checks if the new element's size equals to the curr ADT elements size
int CheckTypes(PAdptArray ADT, PElement newElement)
{
    int newSize = sizeof(newElement);
    int len = ADT->size;
    PElement curr;
    for (int i = 0; i < len; i++)
    {
        curr = (ADT->elementArr)[i];
        if (curr != NULL)
        {
            // The first appearance of an element is enough for us, because every entry of a new element will meet this function:
            return (sizeof(curr) == newSize);
        }
    }
    return 1; // empty array case
    
}

// this function puts a copy of desired element in the desired index (must get a suitable element, otherwise - 0) 
Result SetAdptArrayAt(PAdptArray ADT, int index, PElement newElement)
{
    if (ADT == NULL)
    {
        return FAIL;
    }
    if (CheckTypes(ADT, newElement) == 0) // if the new element's type doesnt fit the curr type 
    {
        return FAIL;
    }
    if (index + 1 > ADT->size) // so we need to extend the ADT
    {
        PElement* newArr = (PElement*)calloc((index+1), sizeof(PElement)); // create new arr with the updated size
        if (newArr == NULL)
        {
            return FAIL;
        }
        memcpy(newArr, ADT->elementArr, (ADT->size)*sizeof(PElement)); // copy the content of the old arr to new arr
        free(ADT->elementArr);
        ADT->elementArr = newArr; // update the ADT's array to the extended array
        ADT->size = index + 1; // update the size of the new ADT
    }
    if ((ADT->elementArr)[index] != NULL)
    {
        ADT->deleter((ADT->elementArr)[index]); // using build-in deleter
    }
    (ADT->elementArr)[index] = ADT->copier(newElement); // puts a copy in the disired index using build-in copier
    return SUCCESS;
}

// this function returns a copy of the element in the desired index
PElement GetAdptArrayAt(PAdptArray ADT, int index)
{
    if (ADT->size < index + 1 || (ADT->elementArr)[index] == NULL) // out of bound / NULL cases
    {
        return NULL;
    }
    PElement elememt = ADT->copier((ADT->elementArr)[index]); // using build-in copier
    return elememt;
}

// this function returns the size of the ADT
int GetAdptArraySize(PAdptArray ADT)
{
    if (ADT == NULL)
    {
        return -1;
    }
    return ADT->size;
}

// this function runs a loop on each element in the ADT and prints it (if its not NULL)
void PrintDB(PAdptArray ADT)
{
    int len = ADT->size;
    PElement curr;
    for (int i = 0; i < len; i++)
    {
        curr = (ADT->elementArr)[i];
        if (curr != NULL)
        {
            ADT->printer(curr); // using build-in printer
        }
    }
}