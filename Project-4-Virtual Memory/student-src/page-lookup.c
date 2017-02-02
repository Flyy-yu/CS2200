#include "swapfile.h"
#include "statistics.h"
#include "pagetable.h"

/*******************************************************************************
 * Looks up an address in the current page table. If the entry for the given
 * page is not valid, increments count_pagefaults and traps to the OS.
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t pagetable_lookup(vpn_t vpn, int write) {
	pfn_t pfn; /* CHANGE ME */
	pte_t *pagee;
	pagee = current_pagetable + vpn;
	if (pagee->valid != 1)
	{
		count_pagefaults = count_pagefaults + 1;
		pfn=pagefault_handler(vpn,write);
	}
	else{
		pfn = pagee->pfn;
	}
   /* FIX ME - Part 2 
    * Determine the PFN corresponding to the passed in VPN.
    * Perform the lookup using the current_pagetable.
    */


   return pfn;
}
