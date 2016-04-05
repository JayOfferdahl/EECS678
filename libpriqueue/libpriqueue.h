/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_

/**
  Node Data Structure
*/
typedef struct node_t node_t;

struct node_t
{
	void *ptr;

	node_t *next;
};

/**
  Priqueue Data Structure
*/
typedef struct _priqueue_t
{
	// The size of the tree
	size_t size;

	// The function to associate priority with
	int (*comparer)(const void *, const void *);

	// Pointers to the head object
	node_t *head;
} priqueue_t;


void   priqueue_init     (priqueue_t *q, int(*comparer)(const void *, const void *));

int    priqueue_offer    (priqueue_t *q, void *ptr);
void * priqueue_peek     (priqueue_t *q);
void * priqueue_poll     (priqueue_t *q);
void * priqueue_at       (priqueue_t *q, int index);
int    priqueue_remove   (priqueue_t *q, void *ptr);
void * priqueue_remove_at(priqueue_t *q, int index);
int    priqueue_size     (priqueue_t *q);
void   priqueue_print    (priqueue_t *q);

void   priqueue_destroy  (priqueue_t *q);

#endif /* LIBPQUEUE_H_ */
