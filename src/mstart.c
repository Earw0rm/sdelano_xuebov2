#include "uart.h"
#include "riscv.h"
#include "memlayout.h"
#include "param.h"


// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// a scratch area per CPU for machine-mode timer interrupts.
uint64_t timer_scratch[NCPU][5];

// assembly code in kernelvec.S for machine-mode timer interrupt.
extern void timervec(void);
extern void kernelvec(void);

void sstart(void){
    // uart_send("Hello world!\r\n");
    while(1);
}



void
timerinit(){
    // each CPU has a separate source of timer interrupts.
    int id = r_mhartid();



    *(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + TIMERINTERVAL;

    // timer interrupt
    w_mie(1 << 7); 

    uint64_t *scratch = &timer_scratch[id][0];    
    scratch[3] = CLINT_MTIMECMP(id);
    scratch[4] =  TIMERINTERVAL;
    w_mscratch((uint64_t) scratch);

    w_mtvec((uint64_t) timervec);
    // enable global intr
    w_mstatus(r_mstatus() | (1 << 3));
}
void shittrap(void){
    uart_send("shittrap\r\n");
}

void mstart(uint8_t mhartid){
    if(mhartid == 0){
        init_uart();
    }

    w_tp(mhartid);
    w_mstatus(INIT_MSTATUS_VAL);

    w_mie(0xffff);
    w_mip(0x0);


    w_mepc((uint64_t) sstart);

    w_satp(0);
    w_medeleg(0xffff);
    w_mideleg(0xffff);


    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(0xf);




    timerinit();

    w_stvec((uint64_t) kernelvec);
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);


    asm volatile("mret");
}