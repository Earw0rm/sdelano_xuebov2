#include "common.h"
#include "riscv.h"
#include "uart.h"
#include "printf.h"
#include "memlayout.h"
#include "vm.h"
#include "task.h"
#include "sched.h"

#define SUPERVISOR_SOFTWARE_INTERRUPT (0x8000000000000000L | 1)
#define SUPERVISOR_EXTERNAL_INTERRUPT (0x8000000000000000L | 9)

#define CAUSE_INTR_MASK (1ull << 63)



uint8_t askintr(void){
    uint64_t scause = r_scause();
    switch (scause){
        case SUPERVISOR_SOFTWARE_INTERRUPT:
            w_sip(r_sip() & ~2); // asc
            return 1;
            break;
        case SUPERVISOR_EXTERNAL_INTERRUPT:
            return 2;
        default:
            return 0;
            break;
    }
}

void kerneltrap(void){

    uint64_t sstatus = r_sstatus(); 
    uint64_t scause = r_scause();
    uint64_t stval  = r_stval();

    if((sstatus & SPP_MASK) != 0){ // mean that pp is supervisor

        


        if((scause & CAUSE_INTR_MASK) == 0){ //this is exception

            if(((scause & 0xff) == 0xd) && (stval < PHYMEMEND ) && (stval > PHYMEMSTART)){
 
               printf("Exception \r\n");
                uint64_t satp = r_satp();
                w_satp(0x0);
                asm volatile("sfence.vma x0, x0");
                
                uint64_t pgtbl = (satp & SATP_PPN_MASK) << 12;
                int8_t map_res = mapva(stval, stval, (pagetable_t) pgtbl, PTE_XWR, true);
                
                w_satp(satp);
                asm volatile("sfence.vma x0, x0");
            }



        }else if((scause & 1 ) != 0){ //this is software interrupt
            printf("Software interrupt. \r\n");
            schedule();
            w_sip(r_sip() & ~2); // asc
        }

    }else{ // user mode 

        printf("Kernel trap, previous mode is usermode. \r\n");

        
    }
}

