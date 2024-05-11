#ifndef _RISCV_H
#define _RISCV_H

#include "common.h"





/**
 * level | Encoding   | Name |
 * -------------------------
 * 0                  | User       | U
 * 1                  | Supervisor | S
 * 2                  | Res        |
 * 3                  | Machine    | M
*/

/*////////////////////////////////////
**************mstatus*****************
*/////////////////////////////////////

//previous interrupt enable
#define MPIE (0 << 7)
// interrupt enable
// set 0 when intrr happens
#define MIE (0 << 3)
// previous privelege
#define MPP (1 << 11)
// Modify PRiVilege
// allow translate load/store operation throo pagetable in MPP mode
#define MPRV (0 << 17)
// Make Executable Readable 
// MXR=0 => only loads from pages marked readable will succeed.
// MXR=1 => loads from pages marked either readable or executable (R=1 or X=1) will succeed.
#define MXR (1 << 19)
// When SUM=0, S-mode memory accesses to pages that are accessible by U-mode will fault. 
// When SUM=1, these accesses are permitted.
#define SUM (1 << 18)
// When TVM=1, attempts to read or write the satp CSR or execute an SFENCE.VMA or SINVAL.VMA
// instruction while executing in S-mode will raise an
// illegal instruction exception. 
// When TVM=0, these operations are permitted in S-mode.
#define TVM (0 << 20)
// When TW=0, the WFI instruction may execute in lower privilege modes when
// not prevented for some other reason.
// When TW=1, then if WFI is executed in any less-privileged
// mode, and it does not complete within an implementation-specific, bounded time limit, the WFI
// instruction causes an illegal instruction exception.
#define TW (0 << 21)

//When TSR=1, attempts to execute SRET while executing in S-mode
//will raise an illegal instruction exception. 
//When TSR=0, this operation is permitted in S-mode.
#define TSR (0 << 22)

#define SPIE (1 << 5)
#define SIE  (0 << 1)

#define INIT_MSTATUS_VAL (MPIE | MPP | MIE | MXR | MPRV | SUM | TVM | TW | TSR | SPIE | SIE)

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

#define SPP_MASK (1 << 8)


static inline uint64_t
r_sstatus(void)
{
  uint64_t x;
  asm volatile("csrr %0, sstatus" : "=r" (x) );
  return x;
}

static inline void 
w_sstatus(uint64_t x)
{
  asm volatile("csrw sstatus, %0" : : "r" (x));
}


/**
 * ////////////////////////////////////////// 
 * *************mepc**************а
 * ////////////////////////////////////////// 
**/


static inline uint64_t
r_mepc(void)
{
  uint64_t x;
  asm volatile("csrr %0, mepc" : "=r" (x) );
  return x;
}



static inline void 
w_mepc(uint64_t x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}


/**
 * ////////////////////////////////////////// 
 * *************mideleg medeleg**************а
 * ////////////////////////////////////////// 
 * 
 * When a trap is delegated to S-mode, the scause register is written with the trap cause; the sepc
 * register is written with the virtual address of the instruction that took the trap; the stval register
 * is written with an exception-specific datum; the SPP field of mstatus is written with the active
 * privilege mode at the time of the trap; the SPIE field of mstatus is written with the value of the
 * SIE field at the time of the trap; and the SIE field of mstatus is cleared. The mcause, mepc, and
 * mtval registers and the MPP and MPIE fields of mstatus are not written.
 * 
 * Delegated interrupts result in the interrupt being masked at the delegator privilege level. For
 * example, if the supervisor timer interrupt (STI) is delegated to S-mode by setting mideleg[5], STIs
 * will not be taken when executing in M-mode. By contrast, if mideleg[5] is clear, STIs can be taken
 * in any mode and regardless of current mode will transfer control to M-mode.
 */



#define INIT_MEDELEG_VAL (0xffff)
#define INIT_MIDELEG_VAL (0xffff)

static inline void 
w_medeleg(uint64_t x)
{
  asm volatile("csrw medeleg, %0" : : "r" (x));
}

static inline void 
w_mideleg(uint64_t x)
{
  asm volatile("csrw mideleg, %0" : : "r" (x));
}

/**
 * 
 * ////////////////////////////////////////// 
 * *************mie mip**************а
 * ////////////////////////////////////////// 
 * The mip register is an MXLEN-bit read/write register containing information on pending interrupts,
 * while mie is the corresponding MXLEN-bit read/write register containing interrupt enable bits.
 * 
 * Each individual bit in register mip may be writable or may be read-only. When bit i in mip is
 * writable, a pending interrupt i can be cleared by writing 0 to this bit. If interrupt i can become
 * pending but bit i in mip is read-only, the implementation must provide some other mechanism for
 * clearing the pending interrupt.
 * 
 * Bits mip.MEIP and mie.MEIE are the interrupt-pending and interrupt-enable bits for machine-
 * level external interrupts. MEIP is read-only in mip, and is set and cleared by a platform-specific
 * interrupt controller.
 * Bits mip.MTIP and mie.MTIE are the interrupt-pending and interrupt-enable bits for machine
 * timer interrupts. MTIP is read-only in mip, and is cleared by writing to the memory-mapped
 * machine-mode timer compare register.
*/
#define INIT_MIE_VAL (0xffffffffffffffffull)
#define INIT_MIP_VAL (0x0)

static inline void 
w_mie(uint64_t x)
{
  asm volatile("csrw mie, %0" : : "r" (x));
}

static inline void 
w_mip(uint64_t x)
{
  asm volatile("csrw mip, %0" : : "r" (x));
}

static inline uint64_t
r_mie(void)
{
  uint64_t x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

static inline uint64_t
r_mip(void)
{
  uint64_t x;
  asm volatile("csrr %0, mip" : "=r" (x) );
  return x;
}

static inline uint64_t
r_mhartid(void)
{
  uint64_t x;
  asm volatile("csrr %0, mhartid" : "=r" (x) );
  return x;
}



static inline void 
w_pmpaddr0(uint64_t x)
{
  asm volatile("csrw pmpaddr0, %0" : : "r" (x));
}

static inline void 
w_pmpcfg0(uint64_t x)
{
  asm volatile("csrw pmpcfg0, %0" : : "r" (x));
}


static inline void 
w_satp(uint64_t x)
{
  asm volatile("csrw satp, %0" : : "r" (x));
}

static inline void 
w_sscratch(uint64_t x){
  asm volatile("csrw sscratch, %0" : : "r" (x));
}


static inline uint64_t
r_sscratch(void){
  uint64_t x;
  asm volatile("csrr %0, sscratch" : "=r" (x) );
  return x;
}



#define SIE_SEIE (1L << 9) // external
#define SIE_STIE (1L << 5) // timer
#define SIE_SSIE (1L << 1) // software

static inline uint64_t
r_sie(void){
  uint64_t x;
  asm volatile("csrr %0, sie" : "=r" (x) );
  return x;
}

static inline void 
w_sie(uint64_t x)
{
  asm volatile("csrw sie, %0" : : "r" (x));
}



static inline void 
w_mtvec(uint64_t x)
{
  asm volatile("csrw mtvec, %0" : : "r" (x));
}


static inline void 
w_mscratch(uint64_t x)
{
  asm volatile("csrw mscratch, %0" : : "r" (x));
}

static inline void 
w_tp(uint64_t x)
{
  asm volatile("mv tp, %0" : : "r" (x));
}

static inline void 
w_sip(uint64_t x)
{
  asm volatile("csrw sip, %0" : : "r" (x));
}

static inline uint64_t
r_sip(void)
{
  uint64_t x;
  asm volatile("csrr %0, sip" : "=r" (x) );
  return x;
}

static inline uint64_t
r_scause(void)
{
  uint64_t x;
  asm volatile("csrr %0, scause" : "=r" (x) );
  return x;
}


#define SATP_PPN_MASK ((1ull << 44) - 1)

static inline uint64_t
r_satp(void)
{
  uint64_t x;
  asm volatile("csrr %0, satp" : "=r" (x) );
  return x;
}

static inline uint64_t
r_stval(void)
{
  uint64_t x;
  asm volatile("csrr %0, stval" : "=r" (x) );
  return x;
}

static inline void
intr_off(void){
  w_sstatus(r_sstatus() & ~2); 
}

static inline void
intr_on(void){
  w_sstatus(r_sstatus() | 2); 
}


#define KSTVEC_MODE (8ull << 60) //Sv39
#define KSTVEC_ASID (0ull << 44)

#define STVEC_PPN_MASK ((1 << 44) - 1)

static inline void 
w_stvec(uint64_t x){
  asm volatile("csrw stvec, %0" : : "r" (x));
}

static inline uint64_t
r_stvec(void){
  uint64_t x;
  asm volatile("csrr %0, stvec" : "=r" (x) );
  return x;
}

#endif 