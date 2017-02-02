#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "page-splitting.h"
#include "tlb.h"
#include "statistics.h"
#include "useful.h"

#include "memory.h"

/*******************************************************************************
 * Main memory.
 */
uint8_t *memory;

/*******************************************************************************
 * Loads the data given by the virtual address, using the virutal -> physical
 * translation system of the CPU.
 *
 * @param va The virtual address to read.
 * @return The data at the given address.
 */
word_t mem_load(vaddr_t va) {
   vpn_t vpn;
   pfn_t pfn, offset;
   word_t data;

   vpn = VADDR_PAGENUM(va);
   pfn = tlb_lookup(vpn, 0);
   offset = VADDR_OFFSET(va);
   data = memory[pfn * page_size + offset];

   printf("LOADING %3.3hu as data from virtual address %5.5hu.\nVirtual Address Information: (VPN: %5.5hu PFN: %5.5hu OFFSET: %5.5hu)\n", 
           (unsigned short)data, va, vpn, pfn, offset);

   count_reads++;

   return data;
}

/**
 * Stores the data given by the virtual address, using the virutal -> physical
 * translation system of the CPU.
 *
 * @param va The virtual address to write.
 * @param data The data to write to the address.
 */
void mem_store(vaddr_t va, word_t data) {
   vpn_t vpn;
   pfn_t pfn, offset;

   vpn = VADDR_PAGENUM(va);
   pfn = tlb_lookup(vpn, 1);
   offset = VADDR_OFFSET(va);

   printf("STORING %3.3u as data in virtual address %5.5u.\nVirtual Address information: (VPN: %5.5u PFN: %5.5u OFFSET: %5.5u)\n", 
          data, va, vpn, pfn, offset);

   count_writes++;

   memory[pfn * page_size + offset] = data;
}

/*******************************************************************************
 * Initializes all memory controlled by the processory -- that is, the physical
 * memory and the TLB.
 */
void mem_init(void) {
   if ((memory    = calloc(mem_size, sizeof(unsigned char))) == NULL) {
      PERROR("calloc");
      exit(EXIT_FAILURE);
   }
}

/*******************************************************************************
 * Frees all the memory used to represent physical memory
 */
void mem_free(void) {
   free(memory);
}
