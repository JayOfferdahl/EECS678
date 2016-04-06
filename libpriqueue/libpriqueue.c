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