#ifndef _MEMLAYOUT_H
#define _MEMLAYOUT_H


// Physical memory layout

// qemu -machine virt is set up like this,
// based on qemu's hw/riscv/virt.c:
//
// 00001000 -- boot ROM, provided by qemu
// 02000000 -- CLINT
// 0C000000 -- PLIC
// 10000000 -- uart0 
// 10001000 -- virtio disk 
// 80000000 -- boot ROM jumps here in machine mode
//             -kernel loads the kernel here
// unused RAM after 80000000.

// the kernel uses physical memory thus:
// 80000000 -- entry.S, then kernel text and data
// end -- start of kernel page allocation area
// PHYSTOP -- end RAM used by the kernel




// core local interruptor (CLINT), which contains the timer.
#define CLINT 0x2000000L
#define CLINT_MTIMECMP(hartid) (CLINT + 0x4000 + 8*(hartid))
#define CLINT_MTIME (CLINT + 0xBFF8) 


#define MYCPU       0x180000000
#define PHYMEMSTART 0x80000000
#define PHYMEMEND   0x87ffffff
#define UART        0x10000000


#ifndef __ASSEMBLER__
extern char _kernel_start;
extern char _kernel_end;

extern char _trampoline_start;
extern char _trampoline_end;
extern char _text_end;


extern char kernelvec[];
#endif 



#define UMEMSTART  0x0
#define UMEMEND    0x10000000



#define TRAMPOLINE 0x280000000ull
#define USTACK     0x10001ff0
#define USTACKTOP  0x10001000

#endif 