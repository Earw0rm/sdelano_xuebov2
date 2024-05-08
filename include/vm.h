#ifndef _VM_H
#define _VM_H
#include "common.h"


// The permission
// bits, R, W, and X, indicate whether the page is readable, writable, and executable, respectively.
// When all three are zero, the PTE is a pointer to the next level of the page table; otherwise, it is
// a leaf PTE
// * u   - user mode accessing. If the SUM bit in the sstatus register is set, supervisor mode software may also access pages with U=1.
// * rwx - read write execute.
// * g - global
// * a - accessed. Indicates the virtual page has been read, written, or fetched from since the last time the A bit was cleared
// * d - dirty. Indicates the virtual page has been written since the last time the D bit was cleared

#define VAPDIND(va, level) ((va & (511 << (12 + 9*level))) >> (12 + 9*level))

#define PTE2PA(pte) (((pte) >> 10) << 12)
#define PA2PTE(pa) ((((uint64_t)pa) >> 12) << 10)

#define PTE_V 1
#define PTE_X (1 << 3)
#define PTE_W (1 << 2)
#define PTE_R (1 << 1)
#define PTE_D (1 << 7)
#define PTE_A (1 << 6)

#define PTE_XWRDA (PTE_X | PTE_W | PTE_R )

#define PTE_FLAG_MASK ((1 << 8) - 1)

#define ADDRROUNDDOWN(addr) ((addr) & ~(4096 - 1))



pte_t * walk(uint64_t va, pagetable_t pgtbl, bool alloc);
int8_t mapva(uint64_t va, uint64_t pa, pagetable_t pgtbl, uint16_t flags, bool alloc);
void ppgtbl(pagetable_t pgtbl);

uint64_t kpgtbl_init(void);
#endif 