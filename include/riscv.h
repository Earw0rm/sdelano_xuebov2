#ifndef _RISCV_H
#define _RISCV_H

#include "common.h"

static inline void 
w_mepc(uint64_t x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}



static inline uint64_t
r_mstatus(void)
{
  uint64_t x;
  asm volatile("csrr %0, mstatus" : "=r" (x) );
  return x;
}

static inline void 
w_mstatus(uint64_t x)
{
  asm volatile("csrw mstatus, %0" : : "r" (x));
}


#endif 