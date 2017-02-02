/*
 * student.h
 * Multithreaded OS Simulation for CS 2200, Project 6
 *
 * YOU WILL NOT NEED TO MODIFY THIS FILE
 *
 */

#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "os-sim.h"

/* Function declarations */
extern void idle(unsigned int cpu_id);
extern void preempt(unsigned int cpu_id);
extern void yield(unsigned int cpu_id);
extern void terminate(unsigned int cpu_id);
extern void wake_up(pcb_t *process);

#endif /* __STUDENT_H__ */
