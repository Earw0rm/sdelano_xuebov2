#ifndef _UART_TH1520_H
#define _UART_TH1520_H

#include "common.h"




struct ns16550a{
    // &uart_sclk {
    // 	clock-frequency = <100000000>;
    // };
    // divisor = DLL (DLAB = 1)
    // output baud rate = (serial clock frequency)/(16*divisor).
    //
    // RBR data valid only if DR bit in LSR is set
    // 
    // THR Data should only be written to the THR when the THR Empty (THRE) bit (LSR[5]) is set.
    volatile uint32_t rbr_thr_dll;  /* 0x00: reseiver buffer register (read), transmit holding register (write), devisor line low (DLAB=1) */
    volatile uint32_t ier_dlh;      /* 0x04: interrupt enable register (DLAB=0), devisor line hight (DLAB=1) */

    // IIR [3:0]:
    // ◼ 0x0 (MODEM_STATUS): Modem status
    // ◼ 0x1 (NO_INTERRUPT_PENDING): No interrupt 
    // ◼ 0x2 (THR_EMPTY): THR empty
    // ◼ 0x4 (RECEIVED_DATA_AVAILABLE): Received data available
    // ◼ 0x6 (RECEIVER_LINE_STATUS): Receiver line status
    // ◼ 0x7 (BUSY_DETECT): Busy detect
    // ◼ 0xc (CHARACTER_TIMEOUT): Character timeout
    volatile uint32_t iir_fcr;      /* 0x08:  Interrupt Identification Register (read), fifo control register (write) */

    // LCR : parity, stop&start bit, data length select. 
    volatile uint32_t lcr;          /* 0x0c: line control register */
    volatile uint32_t mcr;          /* 0x10: modem control register */

    // Read Only. LSR[6] Transmitter Empty bit, LSR[5] Transmit Holding Register Empty bit
    volatile uint32_t lsr;          /* 0x14: line status register */

    // Read only.
    volatile uint32_t msr;          /* 0x18: modem status register */
    volatile uint32_t scr;          /* 0x1c: scrachpad register */



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

///////////////////////
// default uart0 cfg //
///////////////////////


// E - Bps/Par/Bits : 115200 8N1.
// Devisor latch low.                            |
#define DLL 54

//LCR. Default - one stop bit, 8 data bit, no parity
#define DLAB_EN  (1 << 7)
#define PEN_OFF  (0 << 3)
#define STOP_N   (0 << 2) 
#define DLS_N    (0x3)


//IER. 
//This is applicable only for Overrun Error, Parity Error, Framing Error, and Break Interrupt status bits.
#define ELCOLR (1 << 4)
//modem interrupt. Not relevant for keyboard and logs
#define EDSSI  (0 << 3)
// Different errors (OE, PE, FE, BI, FIFO)
#define ELSI   (1 << 2)
//Transmitter holding register empty interrupt
#define ETVEI  (1 << 1)
//Received data available interrupt
#define ERBFI  (1)


//FCR.
//the trigger level in the receiver FIFO buffer
#define RT (0x0 << 6)
//the trigger level in the transmit FIFO buffer. Set to 1/2 of buffer
#define TET (0x3 << 4)
// bit for clearing fifo buffers. This 2 register set need to be using in error handling
#define XFIFOR (1 << 2)
#define RFIFOR (1 << 1)
//fifo enable
#define FIFOE  (1)


//LSR

#endif 