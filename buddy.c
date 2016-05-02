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

	/* TODO: DECLARE NECESSARY MEMBER VARIABLES */
	int block_size_order;
	int occupied;
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
		/* TODO: INITIALIZE PAGE STRUCTURES */

		// Initialize the page list
		INIT_LIST_HEAD(&g_pages[i].list);
		g_pages[i].block_size_order = -1;
		g_pages[i].occupied = 0;

		if(i != 0)
			list_add(&g_pages[i].list, &g_pages[0].list);
	}
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
	// Determine what order is needed to allocate this memory
	int size_order = 1,
		request_size = size,
		request_page,
		request_page_length;

	while((request_size /= 2) > 0)
		size_order++;

#if USE_DEBUG
	printf("Requested size is %i, order of %i\n", size, size_order);
#endif

	// Find the next available memory
	for(int i = size_order; i < MAX_ORDER; i++) {

		// If there's an available block, begin partitioning it
		if(!list_empty(free_area[i])) {

		#if USE_DEBUG
			printf("Found empty block of order %i, partitioning...\n", i);
		#endif

			// Break this block down until we have an appropriate size. Once 
			// we've determined we have a block big enough, return it's address
			for(int j = i; j >= size_order; j--) {
				request_page_length = (1 << j) / PAGE_SIZE;
				request_page = find_free_block_page(j); // TODO: make this function which returns the first index of free memory of this size

				// If we're at the smallest block to allocate the request, return the address
				if(j == size_order) {
					void *request_addr = PAGE_TO_ADDR(request_page);

				#if USE_DEBUG
					printf("Marking pages %i through %i as occupied...\n", request_page, request_page + request_page_length);
				#endif

					// Mark each page as occupied
					while(request_page_length > 0)
						g_pages[request_page + request_page_length--].occupied = 1;

					return request_addr;
				}
				// Else, break the block down and appropriately add half to free_area
				else {
					// Break the list into two halves
					
					// Add the right block to free_area
					list_add(g_pages[request_page + request_page_length].list, free_area[j]);
				}
			}
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
	/* TODO: IMPLEMENT THIS FUNCTION */
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
