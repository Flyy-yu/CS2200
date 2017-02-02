/**
 * CS2200 Project 3: Virtual Memory
 * 
 * cpu.c  CPU-specific functions for the VM simulator. This is all the hardware
 *        "guts" that are used to manage VM, including the TLB, page registers,
 *        and such.
 *
 * Author: Sean Caulfield <sean.caulfield@cc.gatech.edu>
 * RCS: $Id: cpu.c,v 1.10 2005/03/11 21:05:10 sean Exp $
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "pagetable.h"

/*******************************************************************************
 * Current page table register, used by the processor to find the running
 * processes page table upon a TLB miss.
 */
pte_t   *current_pagetable; /* CPU page register */
