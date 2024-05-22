#ifndef _UART_TH1520_H
#define _UART_TH1520_H

#include "common.h"

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
 * 
 * ...
 * 
 * 
 * basic registers for standart mode without dma
 * 
 * --------------------------------------------------------------------------------------------------------------------------
*/

struct ns16550a{
    volatile uint32_t rbr_thr_dll;  /* 0x00: RBR (read), THR (write), DLL (DLAB=1) */
    volatile uint32_t ier_dlm;      /* 0x04: IER (DLAB=0), DLM (DLAB=1) */
    volatile uint32_t iir_fcr;      /* 0x08: IIR (read), FCR (write) */
    volatile uint32_t lcr;          /* 0x0c: LCR */
    volatile uint32_t mcr;          /* 0x10: MCR */
    volatile uint32_t lsr;          /* 0x14: LSR */
    volatile uint32_t msr;          /* 0x18: MSR */
    volatile uint32_t scr;          /* 0x1c: SCR */

    volatile uint32_t lpdll;        /* 0x20: low power devicor latch low register*/
    volatile uint32_t lpdlh;        /* 0x24: low power devicor latch high register*/
    volatile uint32_t __pad[2];

    volatile uint32_t srbr_sthr[16]; /* 0x30 + n*0x4: Shadow ((read) Receive Buffer/(write)Transmit Holding) Register*/
    volatile uint32_t far;           /* 0x70: fifo access register */
    volatile uint32_t tfr;           /* 0x74: transmit fifo read   */
    volatile uint32_t rfw;           /* 0x78: receive  fifo write  */
    volatile uint32_t usr;           /* 0x7c: uart status register */
    volatile uint32_t tfl;           /* 0x80: transmit fifo level  */
    volatile uint32_t rfl;           /* 0x84: receive fifo level   */
    volatile uint32_t srr;           /* 0x88: software reset register  */

    volatile uint32_t srts;          /* 0x8c: shadow request to send  */
    volatile uint32_t sbcr;          /* 0x90: shadow break control register  */
    volatile uint32_t cdmam;         /* 0x94: shadow dma mode register  */
    volatile uint32_t sfe;           /* 0x98: shadow fifo enable register  */
    volatile uint32_t srt;           /* 0x9c: shadow rcvr trigger register  */
    volatile uint32_t stet;          /* 0xa0: shadow tx empty trigger register  */

    volatile uint32_t htx;          /* 0xa4: halt tx  */
    volatile uint32_t dmasa;        /* 0xa8: dma software acknowledge register  */
    volatile uint32_t tcr;          /* 0xac: transceiver control register  */
    volatile uint32_t de_en;        /* 0xb0: driver output enable register  */
    volatile uint32_t re_en;        /* 0xb4: receiver output enable register  */

    volatile uint32_t det;          /* 0xb8: driver output enable timing register (DET)  */
    volatile uint32_t tat;          /* 0xbc: TurnAround Timing Register  */

    volatile uint32_t dlf;          /* 0xc0: Divisor latch fraction register  */
    volatile uint32_t rar;          /* 0xc4: Receive Address Register  */
    volatile uint32_t tar;          /* 0xc8: Transmit Address Register  */
    
    volatile uint32_t lcr_ext;      /* 0xcc: Line extended control register  */
    volatile uint32_t cpr;          /* 0xf4: Component parameter register  */

    volatile uint32_t ucv;          /* 0xf8: uart component version  */
    volatile uint32_t ctr;          /* 0xfc: Component type register  */
};


#endif 