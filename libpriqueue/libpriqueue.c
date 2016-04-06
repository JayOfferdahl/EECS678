/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q->size = 0;
  q->head = NULL;
  q->comparer = comparer;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
  // Empty queue, set q->head to a new node
  if(q->size == 0) {
    // Make a new node and fill it with the input pointer
    node_t *temp = malloc(sizeof(node_t));
    temp->ptr = ptr;
    temp->next = NULL;

    // Set the head to the new node & return
    q->head = temp;
    return q->size++;
  }
  else {
    node_t *add = malloc(sizeof(node_t)), *temp = q->head;
    add->ptr = ptr;
    add->next = NULL;

    int index = 0;
    void *tempPtr;
    q->size++;

    // Cycle through the queue to find the right spot, starting at the head
    while(temp != NULL) {
      // If input ptr has higher priority than element we're at, shift down
      if(q->comparer((ptr),(temp->ptr)) < 0) {
        while(temp != NULL) {
          // Swap values in temp and add (temp-temp, so to speak)
          tempPtr = temp->ptr;
          temp->ptr = add->ptr;
          add->ptr = tempPtr;

          // If at the last element, we've already swapped, so put the caboose on
          if(temp->next == NULL) {
            temp->next = add;
            break;
          }

          temp = temp->next;
        }
        break;
      }

      // Comparison failed, move onto the next spot
      index++;

      // If at the end of the list, we've already compared, put the new value on the back
      if(temp->next == NULL) {
        temp->next = add;
        break;
      }
      temp = temp->next;
    }
    return index;
  }
}

/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
  // If the head exists, return it
  if(q->size != 0)
    return q->head;
  else
    return NULL;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
  if(q->size != 0) {
    node_t *temp = q->head;

    if(q->head->next != NULL)
      q->head = q->head->next;
    else {
      q->head = NULL;
    }

    q->size--;

    // Grab the value pointer, free this memory, and return
    void* tempPtr = temp->ptr;
    free(temp);
    return tempPtr;
  }
  else
    return NULL;
}

/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  // Test for valid input
  if(index < 0 || index > (int) q->size - 1)
    printf("Invalid index: priqueue_at called with index: %i\n", index);
  else {
    node_t* temp = q->head;
    int i = 0;

    while(temp != NULL) {
      // If we're at the right spot, return the pointer
      if(i++ == index)
        return temp->ptr;

      temp = temp->next;
    }
  }

  return NULL;
}

/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
  return 0;
}

/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
  node_t *elementToRemove;
  node_t *trailerPointer;
  node_t *leaderPointer;

  elementToRemove = q->head;

  // Checks to see if queue is empty or if index doesn't exist
  if (elementToRemove == NULL || (int)q->size - 1 < index)
  {
    return NULL;
  }
  else
  {
    if (index == 0)
    {
      // Fix the front of the queue
      q->head = q->head->next;
      q->size--;
      return elementToRemove->ptr;
    }

    trailerPointer = elementToRemove;
    elementToRemove = elementToRemove->next;

    if (elementToRemove->next != NULL)
    {
      leaderPointer = elementToRemove->next;
    }

    int i;
    for (i = 1; i < index; i++)
    {
      if (leaderPointer->next != NULL)
      {
        leaderPointer = leaderPointer->next;
      }
      else
      {
        leaderPointer = NULL;
      }
      elementToRemove = elementToRemove->next;
      trailerPointer = trailerPointer->next;
    }

    trailerPointer->next = leaderPointer;
    q->size--;
    return elementToRemove->ptr;
  }

  return 0;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  node_t *temp = q->head, *next;
  q->head = NULL;

  // While not at the end of the list, grab the next element and free the current one
  while(temp != NULL) {
    next = temp->next;

    free(temp);

    temp = next;
    q->size--;
  }
}

/**
  Returns the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
  return q->size;
}


/**
  Prints out the priority queue
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_print(priqueue_t *q)
{
  node_t* temp = q->head;

  while(temp != NULL) {
    printf("%i\n", *(int *)temp->ptr);
    temp = temp->next;
  }
}