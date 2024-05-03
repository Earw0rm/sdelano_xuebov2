#include "uart.h"
#include "riscv.h"



void sstart(void){
    uart_send_unsafe("Hello world!\r\n");
    while(1);
}



void mstart(uint8_t mhartid){
    if(mhartid == 0){
        init_uart();
    }else{
        while(1);
    }


    w_mstatus(INIT_MSTATUS_VAL);

    w_mie(0xffff);
    w_mie(0x0);


    w_mepc((uint64_t) sstart);

    w_satp(0);

    w_medeleg(0xffff);
    w_mideleg(0xffff);


    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);


    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(0xf);




    asm volatile("mret");



}