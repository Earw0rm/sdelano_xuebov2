#include "printf.h"
#include "riscv.h"
#include "memlayout.h"
#include "param.h"
#include "vm.h"
#include "task.h"
#include "sched.h"


// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[0x1000 * NCPU];


__attribute__ ((aligned (16))) char handler_stack[0x1000 * NCPU];


// a scratch area per CPU for machine-mode timer interrupts.
uint64_t timer_scratch[NCPU][5];


__attribute__ ((aligned (0x1000))) struct cpu cpus[NCPU] = {0};

// assembly code in kernelvec.S for machine-mode timer interrupt.
extern void timervec(void);

//trap.c
extern void kerneltrap(void);

__attribute__ ((aligned (0x1000)))
uint8_t ustart(void){
    while(1);
    return 0;
}

void kstart(void){
    fork(ustart);

}

void _kstart(void){
    kstart();

    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE); // ok
    intr_on();
    while(1);
}



void timer_init(void){
    // each CPU has a separate source of timer interrupts.
    int id = r_mhartid();


    *(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + TIMERINTERVAL;

    // timer interrupt
    w_mie(1 << 7); 

    uint64_t *scratch = &timer_scratch[id][0];    
    scratch[3] = CLINT_MTIMECMP(id);
    scratch[4] = TIMERINTERVAL;
    w_mscratch((uint64_t) scratch);

    w_mtvec((uint64_t) timervec);
    // enable global intr
    w_mstatus(r_mstatus() | (1 << 3));
}

void supervisor_init(void){
    uint64_t mhartid = r_mhartid();

    struct task dirt_task = {0};
    cpus[mhartid].current_task = dirt_task;
    cpus[mhartid].kstack = (uint64_t) &handler_stack[(mhartid + 1) << 12];
    cpus[mhartid].traphandler = (uint64_t) &kerneltrap;
    cpus[mhartid].id = mhartid;
    cpus[mhartid].ksatp = 0x0; 
    w_sscratch((uint64_t) &cpus[mhartid]); //ok


    uint64_t default_sstatus = r_sstatus() | (1 << 18 )| (1 << 8) | (1 << 5) | (1 << 1);
    w_sstatus(default_sstatus); // maybe ok
    
    uint64_t pgtbl = kpgtbl_init(); 
    uint64_t ksatp = KSTVEC_MODE | KSTVEC_ASID | (pgtbl >> 12);
   
    cpus[mhartid].ksatp = ksatp;

    w_satp(ksatp); //ok 
    asm volatile("sfence.vma x0, x0");

    //supervisor
    w_stvec(TRAMPOLINE); // ok

    // if(mhartid == 0){
    //     ppgtbl((pagetable_t) pgtbl);
    // }

}

void mstart(uint8_t mhartid){
    if(mhartid == 0){
        init_printf();
    }

    w_tp(mhartid);
    w_mstatus(INIT_MSTATUS_VAL);

    w_mie(0xffff);
    w_mip(0x0);


    w_mepc((uint64_t) _kstart);

    w_medeleg(0xffff);
    w_mideleg(0xffff);

    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(0xf);

    supervisor_init();
    timer_init();

    asm volatile("mret");
}