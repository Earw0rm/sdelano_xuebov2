#ifndef _UART_H
#define _UART_H

#include "common.h"
/**
 * ns16550a qemu https://www.lammertbies.nl/comm/info/serial-uart
*/


/**
 * I/O Port | Read(DLAB=0)                 | Write(DLAB=0)           | Read(DLAB=1)                 | Write(DLAB=1)         |
 * -------------------------------------------------------------------------------------------------------------------------|
 * base     | RBR receiver buffer          | THR transmitter holding | DLL divisor latch LSB        | DLL divisor latch LSB |
 * base+1	| IER interrupt enable         | IER interrupt enable	 | DLM divisor latch MSB        | DLM divisor latch MSB |
 * base+2	| IIR interrupt identification | FCR FIFO control	     | IIR interrupt identification | FCR FIFO control      |
 * base+3	| LCR line control	           | LCR line control	     | LCR line control	            | LCR line control      |
 * base+4	| MCR modem control	           | MCR modem control	     | MCR modem control	        | MCR modem control     |
 * base+5	| LSR line status	           | factory test	         | LSR line status	            | factory test          |
 * base+6	| MSR modem status	           | not used	             | MSR modem status	            | not used              |
 * base+7	| SCR scratch	               | SCR scratch	         | SCR scratch	                | SCR scratch           |
 * --------------------------------------------------------------------------------------------------------------------------
*/



struct ns16550a{
    volatile uint8_t rbr_thr_dll;  /* 0x00: RBR (read), THR (write), DLL (DLAB=1) */
    volatile uint8_t ier_dlm;      /* 0x01: IER (DLAB=0), DLM (DLAB=1) */
    volatile uint8_t iir_fcr;      /* 0x02: IIR (read), FCR (write) */
    volatile uint8_t lcr;          /* 0x03: LCR */
    volatile uint8_t mcr;          /* 0x04: MCR */
    volatile uint8_t lsr;          /* 0x05: LSR */
    volatile uint8_t msr;          /* 0x06: MSR */
    volatile uint8_t scr;          /* 0x07: SCR */
};

void init_uart(void);
void uart_send_unsafe(char * str);
void uart_send(char * str);
void putc(void* p, char c);

#endif 