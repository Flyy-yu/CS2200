/*
 * student.c
 * Multithreaded OS Simulation for CS 2200, Project 6
 * Summer 2015
 *
 * This file contains the CPU scheduler for the simulation.
 * Name:peiyu wang
 * GTID:903006854
 */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os-sim.h"


/*
 * current[] is an array of pointers to the currently running processes.
 * There is one array element corresponding to each CPU in the simulation.
 *
 * current[] should be updated by schedule() each time a process is scheduled
 * on a CPU.  Since the current[] array is accessed by multiple threads, you
 * will need to use a mutex to protect it.  current_mutex has been provided
 * for your use.
 */
static pcb_t **current;
static pthread_mutex_t current_mutex;

//queue

static pcb_t* q_head;
static pthread_mutex_t ready_mutex;

static pthread_cond_t not_idle;
static int round_robin;
static int static_prior;
static int cpu_count;
static int timeslice;

static void addqueue(pcb_t *pcb){
    //printf("%s\n","add" );
   pthread_mutex_lock(&ready_mutex);
    pcb_t *curr;
    curr = q_head;
     pcb->next = NULL;
    if (curr == NULL)
    {
        q_head = pcb;
    }
    else{
        while(curr->next !=NULL){
            curr = curr->next;
        }
        curr->next =pcb;
    }
    pthread_cond_broadcast(&not_idle);
   pthread_mutex_unlock(&ready_mutex);
}

static pcb_t* get(){
    //printf("%s\n", "get");
    pcb_t *getone;
    pthread_mutex_lock(&ready_mutex);

    getone = q_head;
    if (getone!=NULL)
    {
        q_head = getone->next;
    }
   pthread_mutex_unlock(&ready_mutex);
return getone;




}

static pcb_t* gethighestone(){
    //printf("%s\n", "highstar");

    pcb_t *currentone,*highestone;
    int lvl = 0;
    currentone = q_head;
    
    if (q_head == NULL)
    {
        return NULL;
    }
    else if(q_head->next==NULL){
        q_head=NULL;
        return currentone;
    }
    else{
        while (currentone !=NULL){
            
            if (currentone->static_priority>lvl)
            {
                lvl = currentone->static_priority;
                highestone = currentone;

            }
            currentone = currentone->next;
        }



        currentone = q_head;
        if (highestone == q_head) {
            q_head = q_head->next;
            return highestone;
        }

    while (currentone->next != highestone) {
       
         currentone = currentone->next;
            
        }
        currentone->next = highestone->next;
        }


return highestone;

}


/*
 * schedule() is your CPU scheduler.  It should perform the following tasks:
 *
 *   1. Select and remove a runnable process from your ready queue which 
 *	you will have to implement with a linked list or something of the sort.
 *
 *   2. Set the process state to RUNNING
 *
 *   3. Call context_switch(), to tell the simulator which process to execute
 *      next on the CPU.  If no process is runnable, call context_switch()
 *      with a pointer to NULL to select the idle process.
 *	The current array (see above) is how you access the currently running
 *	process indexed by the cpu id. See above for full description.
 *	context_switch() is prototyped in os-sim.h. Look there for more information 
 *	about it and its parameters.
 */
static void schedule(unsigned int cpu_id){
 
 
//printf("%s\n","schedule" );
pcb_t *pcblock;

if (static_prior==1)
{
    pcblock = gethighestone();
}else{
    pcblock = get();
}
//printf("%s\n", "afterget");
pthread_mutex_lock(&ready_mutex);

if (pcblock!=NULL)
{
    pcblock->state = PROCESS_RUNNING;
}

 

current[cpu_id]=pcblock;

pthread_mutex_unlock(&ready_mutex);
 

context_switch(cpu_id,pcblock,timeslice);
//printf("%s\n","cs" );
}



/*
 * idle() is your idle process.  It is called by the simulator when the idle
 * process is scheduled.
 *
 * This function should block until a process is added to your ready queue.
 * It should then call schedule() to select the process to run on the CPU.
 */
extern void idle(unsigned int cpu_id)
{
    //printf("%s\n", "idel");
    pthread_mutex_lock(&ready_mutex);
    while (q_head ==NULL){
        pthread_cond_wait(&not_idle,&ready_mutex);
    }
   pthread_mutex_unlock(&ready_mutex);

    schedule(cpu_id);

   
  
}


/*
 * preempt() is the handler called by the simulator when a process is
 * preempted due to its timeslice expiring.
 *
 * This function should place the currently running process back in the
 * ready queue, and call schedule() to select a new runnable process.
 */
extern void preempt(unsigned int cpu_id)
{
    //printf("%s\n","preempt" );
    pcb_t *pcblock;
    pthread_mutex_lock(&current_mutex);
    pcblock=current[cpu_id];
    pcblock->state = PROCESS_READY;
    pthread_mutex_unlock(&current_mutex);
     
    addqueue(pcblock);
    schedule(cpu_id);


    /* FIX ME */
}


/*
 * yield() is the handler called by the simulator when a process yields the
 * CPU to perform an I/O request.
 *
 * It should mark the process as WAITING, then call schedule() to select
 * a new process for the CPU.
 */
extern void yield(unsigned int cpu_id)
{
    //printf("%s\n","yield" );
     pthread_mutex_lock(&current_mutex);
    pcb_t *pcblock;
    pcblock = current[cpu_id];
    pcblock->state = PROCESS_WAITING;
    
    pthread_mutex_unlock(&current_mutex);
    schedule(cpu_id);
    /* FIX ME */
}


/*
 * terminate() is the handler called by the simulator when a process completes.
 * It should mark the process as terminated, then call schedule() to select
 * a new process for the CPU.
 */
extern void terminate(unsigned int cpu_id)
{
    //printf("%s\n","terminate" );
        pthread_mutex_lock(&current_mutex); 
        pcb_t *pcblock;
        pcblock = current[cpu_id];
        pcblock->state = PROCESS_TERMINATED;
        pthread_mutex_unlock(&current_mutex);

        schedule(cpu_id);
    /* FIX ME */
}


/*
 * wake_up() is the handler called by the simulator when a process's I/O
 * request completes.  It should perform the following tasks:
 *
 *   1. Mark the process as READY, and insert it into the ready queue.
 *
 *   2. If the scheduling algorithm is static priority, wake_up() may need
 *      to preempt the CPU with the lowest priority process to allow it to
 *      execute the process which just woke up.  However, if any CPU is
 *      currently running idle, or all of the CPUs are running processes
 *      with a higher priority than the one which just woke up, wake_up()
 *      should not preempt any CPUs.
 *	To preempt a process, use force_preempt(). Look in os-sim.h for 
 * 	its prototype and the parameters it takes in.
 */
extern void wake_up(pcb_t *process){
//printf("%s\n","wake_up" );
  
   int lows = 10;
     int stop;
     int doornot=0;
process->state = PROCESS_READY;
addqueue(process);

if (static_prior==1)
{ pthread_mutex_lock(&current_mutex);
    
    for (int i = 0; i < cpu_count; i=i+1)
    {
    if (current[i]==NULL)
    {
        doornot = 1;
        break;
        
    }
  if (current[i]->static_priority < lows)
  {
      lows = current[i]->static_priority;
      stop = i;
  }
    }
 pthread_mutex_unlock(&current_mutex);

if (doornot !=1 && lows<process->static_priority )
{
   force_preempt(stop);
   //printf("%d\n", stop);
}

}
 
   

}


/*
 * main() simply parses command line arguments, then calls start_simulator().
 * You will need to modify it to support the -r and -p command-line parameters.
 */
int main(int argc, char *argv[])
{
  

   timeslice= -1;
    /* Parse command-line arguments */
    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "CS 2200 Project 4 -- Multithreaded OS Simulator\n"
            "Usage: ./os-sim <# CPUs> [ -r <time slice> | -p ]\n"
            "    Default : FIFO Scheduler\n"
            "         -r : Round-Robin Scheduler\n"
            "         -p : Static Priority Scheduler\n\n");
        return -1;
    }
    cpu_count = atoi(argv[1]);

    if (argc > 2)
    {
        
    if (strcmp(argv[2],"-r")==0)
    {
        round_robin=1;
    }
    if (strcmp(argv[2],"-p")==0)
    {
        static_prior=1;
    }

    if (argc > 3)
    {
         timeslice=atoi(argv[3]);
    }

    }
    /* FIX ME - Add support for -r and -p parameters*/

    /* Allocate the current[] array and its mutex */
    current = malloc(sizeof(pcb_t*) * cpu_count);
    assert(current != NULL);
    pthread_mutex_init(&current_mutex, NULL);


    pthread_mutex_init(&ready_mutex, NULL);
    q_head = NULL;
    
    pthread_cond_init(&not_idle, NULL);
     
    /* Start the simulator in the library */
    start_simulator(cpu_count);

    return 0;
}





