#ifndef _STUDENT_PAGE_SPLITTING_H_
#define _STUDENT_PAGE_SPLITTING_H_

/*******************************************************************************
 * Your assignment for problem 1 is to fix the following macros.
 * HINT: You have access to the global variable page_size, which tells you how
 *       many addresses a single page contains.
 * HINT: While page_size will be a power of two, allowing you to use bit-wise
 *       arithmetic, consider using modulus division and integer division if
 *       that is more intuitive for you.
 */

/*******************************************************************************
 * Get the page number from a virtual address.
 * 
 * @param addr The virtual address.
 * @return     The virtual page number for this addres.
 */
#define VADDR_PAGENUM(addr) (addr/page_size)

/*******************************************************************************
 * Get the offset for a particular address.
 * @param addr The virtual address.
 * @return     The offset into a page for the given virtual address.
 */
#define VADDR_OFFSET(addr) (addr % page_size)

#endif/*_STUDENT_PAGE_SPLITTING_H_*/
