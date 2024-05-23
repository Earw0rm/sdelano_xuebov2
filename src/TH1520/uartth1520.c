#include "uartth1520.h"


#define UART0 ((struct uartmap *) UART0BASE) 

void uartinit(void){
    UART0->lcr = DLAB_EN | PEN_OFF | STOP_N | DLS_N;
    UART0->rbr_thr_dll = DLL;
    UART0->lcr &= ~DLAB_EN;
    UART0->ier_dlh = ELCOLR | EDSSI | ELSI | ERBFI;
    UART0->iir_fcr = RT | TET | XFIFOR | RFIFOR | FIFOE;

    //clear interrupts
    volatile uint32_t interrupt_clear_ = UART0->lsr;
}




UART_OPERATION_STATUS uartrw(char * rw, UART_MODE mode){
    uint32_t line_status = UART0->lsr;
    if(mode == UART_WRITE){
        if((line_status & TRANSMIT_HOLGING_EMPTY) == 0) return THRNONEMPTY;

        UART0->rbr_thr_dll= (uint32_t) *rw;
        return OPSUCCESS;
    }else if(mode == UART_READ){
        if((line_status & DATA_READY_BIT == 0)) return FIFO_IS_CLEAR;
        if((line_status & (BREAK_INTERRUPT | FRAMING_ERROR | PARITY_ERROR | OVERRUN_ERROR)) != 0) return ERROR_AT_TOP;
        

        *rw = UART0->rbr_thr_dll;
        return OPSUCCESS;
    }

    return UNRESOLVED_CONDITION;
}