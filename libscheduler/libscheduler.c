/** @file libscheduler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"
#include "../libpriqueue/libpriqueue.h"

/**
 * Global priqueue
 */
priqueue_t q;

int FCFScompare(const void * a, const void * b)
{
  return 1;
}

int SJFcompare(const void * a, const void * b)
{
  return ((*(job_t*)a).arrivalTime - (*(job_t*)b).arrivalTime);
}

int PRIcompare(const void * a, const void * b)
{
  return (*(job_t *)a).priority - (*(job_t *)b).priority;
}

int PPRIcompare(const void * a, const void * b)
{
  int compare = (*(job_t *)a).priority - (*(job_t *)b).priority;

  if(compare == 0)
    return (*(job_t *)a).arrivalTime - (*(job_t *)b).arrivalTime;
  else
    return compare;
}

/**
  Initalizes the scheduler.
 
  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/
void scheduler_start_up(int cores, scheme_t scheme)
{
  m_cores = cores;
  m_coreArr = malloc(cores * sizeof(job_t));

  // Initializes core array so that all cores are in unused state at startup
  int i;
  for (i = 0; i < cores; i++)
  {
    m_coreArr[i] = NULL;
  }

  m_type = scheme;

  if (m_type == FCFS)
  {
    priqueue_init(&q, FCFScompare);
  }
  else if (m_type == SJF)
  {
    priqueue_init(&q, SJFcompare);
  }
  else if (m_type == PRI)
  {
    priqueue_init(&q, PRIcompare);
  }
  else if (m_type == PPRI)
  {
    priqueue_init(&q, PPRIcompare);
  }
}


/**
  Called when a new job arrives.
 
  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made. 
 
 */
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
  int firstIdleCoreFound = scheduler_idle_core_finder();

  job_t *temp = malloc(sizeof(job_t));

  temp->pid = job_number;
  temp->arrivalTime = running_time;
  temp->priority = priority;

  if (m_type == FCFS || m_type == SJF || m_type == PRI)
  {
    if (firstIdleCoreFound != -1)
    {
      // Signal that the core at firstIdleCoreFound is being used
      m_coreArr[firstIdleCoreFound] = temp;
      return firstIdleCoreFound;
    }
  }
  else if (m_type == PPRI)
  {
    if (firstIdleCoreFound != -1)
    {
      // Signal that the core at firstIdleCoreFound is being used
      m_coreArr[firstIdleCoreFound] = temp;
      return firstIdleCoreFound;
    }
    // No idle cores, preempt a job with lower priority, if any
    else
    {
      int i, lowestPriSoFar = m_coreArr[0]->priority, lowestPriCore = 0;

      for(i = 0; i < m_cores; i++) {
        // Lower priority than we've seen
        if(m_coreArr[i]->priority > lowestPriSoFar)
        {
          lowestPriSoFar = m_coreArr[i]->priority;
          lowestPriCore = i;
        }
      }

      // We have the lowest priority and the core it's running on, compare to new job
      if(lowestPriSoFar > temp->priority)
      {
        // Send the job running on the found core to the priqueue, put temp in its place
        priqueue_offer(&q, m_coreArr[lowestPriCore]);
        m_coreArr[lowestPriCore] = temp;
        return lowestPriCore;
      }
      // Else, put temp on the queue and signal no scheduling changes
    }
  }

  // If at this step, no scheduling changes should be made
  priqueue_offer(&q, temp);
  return -1;
}

int scheduler_idle_core_finder(void)
{
  int i;
  for (i = 0; i < m_cores; i++)
  {
    if (m_coreArr[i] == NULL)
    {
      return i;
    }
  }

  return -1;
}

/**
  Called when a job has completed execution.
 
  The core_id, job_number and time parameters are provided for convenience. You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.
 
  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time)
{
  if (m_type == FCFS || m_type == SJF || m_type == PRI || m_type == PPRI)
  {
    // Free up the core where the finished job has completed
    free(m_coreArr[core_id]);
    m_coreArr[core_id] = NULL;
  }

  if (priqueue_size(&q) != 0)
  {
    job_t* temp = (job_t*)priqueue_poll(&q);
    m_coreArr[core_id] = temp;

    return temp->pid;
  }
  return -1;
}


/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.
 
  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator. 
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time)
{
  return -1;
}


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
  return 0.0;
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
  return 0.0;
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
  return 0.0;
}


/**
  Free any memory associated with your scheduler.
 
  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{

}


/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)  
  
  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{
  priqueue_print(&q);
}
