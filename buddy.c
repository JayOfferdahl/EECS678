/**
 * Buddy Allocator
 *
 * For the list library usage, see http://www.mcs.anl.gov/~kazutomo/list/
 */

/**************************************************************************
 * Conditional Compilation Options
 **************************************************************************/
#define USE_DEBUG 0

/**************************************************************************
 * Included Files
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"
#include "list.h"

/**************************************************************************
 * Public Definitions
 **************************************************************************/
#define MIN_ORDER 12
#define MAX_ORDER 20

#define PAGE_SIZE (1<<MIN_ORDER)
/* page index to address */
#define PAGE_TO_ADDR(page_idx) (void *)((page_idx*PAGE_SIZE) + g_memory)

/* address to page index */
#define ADDR_TO_PAGE(addr) ((unsigned long)((void *)addr - (void *)g_memory) / PAGE_SIZE)

/* find buddy address */
#define BUDDY_ADDR(addr, o) (void *)((((unsigned long)addr - (unsigned long)g_memory) ^ (1<<o)) \
									 + (unsigned long)g_memory)

#if USE_DEBUG == 1
#  define PDEBUG(fmt, ...) \
	fprintf(stderr, "%s(), %s:%d: " fmt,			\
		__func__, __FILE__, __LINE__, ##__VA_ARGS__)
#  define IFDEBUG(x) x
#else
#  define PDEBUG(fmt, ...)
#  define IFDEBUG(x)
#endif

/**************************************************************************
 * Public Types
 **************************************************************************/
typedef struct {
	struct list_head list;

	// Integer to hold the order size the current block
	int block_size_order;

	// The index of this page
	int index;

	// The memory stored at this page
	char *mem;
} page_t;

/**************************************************************************
 * Global Variables
 **************************************************************************/
/* free lists*/
struct list_head free_area[MAX_ORDER+1];

/* memory area */
char g_memory[1<<MAX_ORDER];

/* page structures */
page_t g_pages[(1<<MAX_ORDER)/PAGE_SIZE];

/**************************************************************************
 * Public Function Prototypes
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

/**
 * Initialize the buddy system
 */
void buddy_init()
{
	int i;
	int n_pages = (1<<MAX_ORDER) / PAGE_SIZE;

	for (i = 0; i < n_pages; i++) {
		// Initialize the page list
		INIT_LIST_HEAD(&g_pages[i].list);

		// Initialize the page variables
		g_pages[i].block_size_order = -1;
		g_pages[i].index = i;
		g_pages[i].mem = PAGE_TO_ADDR(i);
	}
	// To begin, it's all one block
	g_pages[0].block_size_order = MAX_ORDER;

	/* initialize freelist */
	for (i = MIN_ORDER; i <= MAX_ORDER; i++) {
		INIT_LIST_HEAD(&free_area[i]);
	}

	/* add the entire memory as a freeblock */
	list_add(&g_pages[0].list, &free_area[MAX_ORDER]);
}

/**
 * Allocate a memory block.
 *
 * On a memory request, the allocator returns the head of a free-list of the
 * matching size (i.e., smallest block that satisfies the request). If the
 * free-list of the matching block size is empty, then a larger block size will
 * be selected. The selected (large) block is then splitted into two smaller
 * blocks. Among the two blocks, left block will be used for allocation or be
 * further splitted while the right block will be added to the appropriate
 * free-list.
 *
 * @param size size in bytes
 * @return memory block address
 */
void *buddy_alloc(int size)
{
	// Check if out of bounds
	if(size > (1 << MAX_ORDER) || size <= 0) {
	#if USE_DEBUG
		printf("Error: Invalid allocation request: %i is not a valid request size\n", size);
	#endif
		return NULL;
	}

	// Determine what order is needed to allocate this memory
	int size_order = MIN_ORDER, i;

	while(size_order <= MAX_ORDER && (1 << size_order) < size) {
		size_order++;
	}

#if USE_DEBUG
	printf("Requested size is %i, order of %i\n", size, size_order);
#endif

	// Find the next available memory, if we are at a free block that's the correct
	// order, we'll use that block. If not, we'll recursively break it down
	for(i = size_order; i <= MAX_ORDER; i++) {

		// If there's an available block, begin partitioning it
		if(!list_empty(&free_area[i])) {

		#if USE_DEBUG
			printf("Found empty block of order %i, partitioning...\n", i);
		#endif

			page_t *left, *right;
			int request_page_index;
			void *request_page_addr;

			// Break this block down until we have an appropriate size. Once 
			// we've determined we have a block big enough, return it's address
			// If we're at the smallest block to allocate the request, return the address
			if(i == size_order) {
			#if USE_DEBUG
				printf("Block of order %i, has been partitioned, returning address...\n", i);
			#endif
				// Grab onto the left side of this list
				left = list_entry(free_area[i].next, page_t, list);

				// Delete this entry
				list_del(&(left->list));
			}
			// Else, break the block down and appropriately add half to free_area
			else {
				// Recursively grab onto the most left page of the breakdown
				#if USE_DEBUG
					printf("Entering recursion...\n");
				#endif
				left = &g_pages[ADDR_TO_PAGE(buddy_alloc((1 << (size_order + 1))))];

				// Calculate the block's index to the right of left
				request_page_index = left->index + (1 << size_order) / PAGE_SIZE;
				right = &g_pages[request_page_index];

				// Add the right list to free area
				list_add(&(right->list), &free_area[size_order]);
			}

			// Recursion finished (At this step)...update left block properties
			left->block_size_order = size_order;

			// Calculate the requested page's address and return it
			request_page_addr = PAGE_TO_ADDR(left->index);
			return request_page_addr;
		}
	}

	// No blocks big enough to fulfill this request are available.
	return NULL;
}

/**
 * Free an allocated memory block.
 *
 * Whenever a block is freed, the allocator checks its buddy. If the buddy is
 * free as well, then the two buddies are combined to form a bigger block. This
 * process continues until one of the buddies is not free.
 *
 * @param addr memory block address to be freed
 */
void buddy_free(void *addr)
{
	// Store the page index of the memory we want to free
	int request_page_index = ADDR_TO_PAGE(addr),
		request_page_order = g_pages[request_page_index].block_size_order;

	page_t *temp;
	struct list_head *current;

	// Check this block size and all those larger than it
	for(; request_page_order <= MAX_ORDER; request_page_order++) {
		temp = NULL;

		// Loop
		for(current = free_area[request_page_order].next; 
			current != &free_area[request_page_order];
			current = current->prev) {

			// Get the entry from this page
			temp = list_entry(current, page_t, list);
			if(!temp) {
				g_pages[request_page_index].block_size_order = -1;
				list_add(&g_pages[request_page_index].list, &free_area[request_page_order]);
				return;
			}
			else if(temp->mem == BUDDY_ADDR(addr, request_page_order)) {
				g_pages[request_page_index].block_size_order = -1;
				list_add(&g_pages[request_page_index].list, &free_area[request_page_order]);
				return;
			}
		}

		if((char*) addr > temp->mem)
			addr = temp->mem;

		list_del(&(temp->list));
	}
}

/**
 * Print the buddy system status---order oriented
 *
 * print free pages in each order.
 */
void buddy_dump()
{
	int o;
	for (o = MIN_ORDER; o <= MAX_ORDER; o++) {
		struct list_head *pos;
		int cnt = 0;
		list_for_each(pos, &free_area[o]) {
			cnt++;
		}
		printf("%d:%dK ", cnt, (1<<o)/1024);
	}
	printf("\n");
}

/*
int main(int argc, char *argv[])
{
	char *A, *B, *C, *D;

	buddy_init();

	printf("Buddy initialized...\n");
 	
	A = buddy_alloc(80*1024);
	buddy_dump();

	B = buddy_alloc(60*1024);
	buddy_dump();

	C = buddy_alloc(80*1024);
	buddy_dump();

	buddy_free(A);
	buddy_dump();

	D = buddy_alloc(32*1024);
	buddy_dump();
	
	buddy_free(B);
	buddy_dump();
	
	buddy_free(D);
	buddy_dump();

	buddy_free(C);
	buddy_dump();

	return 0;
}*/