#include "uart.h"
#include "common.h"
#include "speenlock.h"


struct speenlock uart_lock;

#define UART ((struct ns16550a *)(0x10000000))

void init_uart(void){
    //8 data bits, one stop bit, no parity, DLAB=1
    uint8_t lcrval = (0x1 << 7 | 0x0 << 6 | 0x0 << 3 | 0x0 << 2 | 0x3);
    UART->lcr = lcrval;

    // Speed (bps) = 115200
    UART->rbr_thr_dll = 0x01;
    UART->ier_dlm = 0x00;

    //DLAB=0
    UART->lcr &= ~(0x1 << 7);
    
    //enable fifo buffer and clear
    UART->iir_fcr = (0x3 << 6 | 0x1 << 1 | 0x1 << 2 | 0x1);

}

void uart_send_unsafe(char * str){
    while(*str != '\0'){
        uint8_t line_is_ready = UART->lsr & (0x1 << 5);
        if(line_is_ready != 0){
            UART->rbr_thr_dll = *str;
            ++str;
        }
    }
}

void uart_send(char * str){
    acquire(&uart_lock);
        uart_send_unsafe(str);
    release(&uart_lock);
}

void putc(void* p, char c){
    char* str;
    *(str) = c;
    uart_send_unsafe(str);
}