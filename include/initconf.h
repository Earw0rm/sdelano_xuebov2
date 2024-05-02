#ifndef _INITCONF_H
#define _INITCONF_H




/**
 * level | Encoding   | Name |
 * -------------------------
 * 0     | User       | U
 * 1     | Supervisor | S
 * 2     | Res        |
 * 3     | Machine    | M
*/

/*////////////////////////////////////
**************mstatus*****************
*/////////////////////////////////////

//previous interrupt enable
#define MPIE (1 << 7);
// interrupt enable
// set 0 when intrr happens
#define MIE (0 << 3);
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

#define INIT_MSTATUS_VAL (MPIE | MPP | MIE | MXR | MPRV | SUM | TVM | TW | TSR)

/*////////////////////////////////////
**************mtvec*****************
*The mtvec register is an MXLEN-bit WARL read/write register that holds trap vector configura-
*tion, consisting of a vector base address (BASE) and a vector mode (MODE).
*/////////////////////////////////////



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



#define INIT_MEDELEG_VAL
#define INIT_MIDELEG_VAL 

/**
 * 
 * ////////////////////////////////////////// 
 * *************mie mip**************а
 * ////////////////////////////////////////// 
 * The mip register is an MXLEN-bit read/write register containing information on pending interrupts,
 * while mie is the corresponding MXLEN-bit read/write register containing interrupt enable bits.
 * 
 * 
 * 
 * 
*/



#endif