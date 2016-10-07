/** @file main.c
 */

#include <stdio.h>
#include <stdlib.h>

/**
  Process Data Structure
*/
typedef struct process_t
{
	// The process ID associated with the process
	int pid;

	// The time at which the process arrived
	int arrTime;

	// The priority associated with the process
	int priority;
} process_t;

/**
 * Returns an integer of the difference of a and b
 * @param  a - the first input to be compared
 * @param  b - the second input to be compared
 * @return   - the integer value of the difference, negative if b > a,
 *             positive if a > b, or 0 if a = b
 */
int compareArr(const void * a, const void * b)
{
	return (*(process_t *)a).arrTime - (*(process_t *)b).arrTime;
}

/**
 * Returns an integer of the difference of a and b
 * @param  a - the first input to be compared
 * @param  b - the second input to be compared
 * @return   - the integer value of the difference, negative if a < b,
 *             positive if b < a, or 0 if a = b
 */
int comparePri(const void * a, const void * b)
{
	int compare = (*(process_t *)a).priority - (*(process_t *)b).priority;

	if(compare == 0)
		return (*(process_t *)a).arrTime - (*(process_t *)b).arrTime;
	else
		return compare;
}

#define PROCESSES 7

int main()
{
	// Read data from file into array
	process_t* processes = malloc(sizeof(process_t) * PROCESSES);

	int pid, arrTime, priority, i = 0;
	
	FILE *file = fopen("process.txt", "r");
	if(file) {
	    while(i < PROCESSES) {
	        fscanf(file, "%d %d %d", &pid, &arrTime, &priority);
	        processes[i].pid = pid;
	        processes[i].arrTime = arrTime;
	        processes[i].priority = priority;

	        i++;
	    }
	    if(i != PROCESSES)
	    	printf("There was an error reading in from the file...i = %d\n", i);
	    i = 0;
	    fclose(file);
	}
	
	// Arrival time in ascending order (use compare1)
	qsort(processes, PROCESSES, sizeof(process_t), (*compareArr));
	
	// Print out contents of sorted file based on arrival time and priority
	printf("The sorted process list: Arrival time\n\nPID\tArrTime\tPriority\n");
	for(i = 0; i < PROCESSES; i++) {
		printf("%d\t%d\t%d\n", processes[i].pid, processes[i].arrTime, processes[i].priority);
	}

	// Priority in descending order, if equal priortity, smaller arrival time comes first
	qsort(processes, PROCESSES, sizeof(process_t), (*comparePri));

	// Print out contents of sorted file based on arrival time and priority
	printf("\nThe sorted process list: Priority\n\nPID\tArrTime\tPriority\n");
	for(i = 0; i < PROCESSES; i++) {
		printf("%d\t%d\t%d\n", processes[i].pid, processes[i].arrTime, processes[i].priority);
	}

	free(processes);

	printf("\nExiting...\n");
	return 0;
}
