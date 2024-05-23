#ifndef _UART_TH1520_H
#define _UART_TH1520_H

#include "common.h"

#define UART0BASE 0xFFE7014000
#define UART1BASE 0xFFE7F00000
#define UART2BASE 0xFFEC010000
#define UART3BASE 0xFFE7F04000
#define UART4BASE 0xFFF7F08000
#define UART5BASE 0xFFF7F0C000


struct uartmap{
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

    //This indicates the number of data entries in the transmit FIFO.
    volatile uint32_t tfl;           /* 0x80: transmit fifo level  */
    //This indicates the number of data entries in the receive FIFO.
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


//LSR masks
// To clear interrupt (error) we need to read LSR.
// This is used to indicate if there is at least one parity error, framing error, or break indication in the FIFO.
// This bit is cleared when the LSR is read and the character with the error is at the top of the receiver FIFO
// and there are no subsequent errors in the FIFO.
#define RECEIVE_FIFO_ERROR (1 << 7)
//Transmitter Shift Register and the FIFO are both empty. 
#define TRANSMITTER_EMPTY  (1 << 6)
// If THRE_MODE_USER = Disabled or THRE mode is
// disabled (IER[7] set to zero) and regardless of FIFO's
// being implemented/enabled or not, this bit indicates
// that the THR or TX FIFO is empty.
#define TRANSMIT_HOLGING_EMPTY (1 << 5)
// In the FIFO mode, the character associated with the
// break condition is carried through the FIFO and is
// revealed when the character is at the top of the FIFO.
// Note: If a FIFO is full when a break condition is received, a FIFO overrun occurs. 
// The break condition and all the information associated with itparity and framing errors-is discarded; 
// any information that a break character was received is lost.
#define BREAK_INTERRUPT (1 << 4)
// This is used to indicate the occurrence of a framing
// error in the receiver. A framing error occurs when the
// receiver does not detect a valid STOP bit in the received data.
// It should be noted that the Framing Error (FE) bit (LSR[3]) will be set if a break interrupt has occurred, as
// indicated by Break Interrupt (BI) bit (LSR[4]). This happens because the break character implicitly
// generates a framing error by holding the sin input to logic 0 for longer than the duration of a character.
#define FRAMING_ERROR (1 << 3)
// This is used to indicate the occurrence of a parity error
// in the receiver if the Parity Enable (PEN) bit (LCR[3]) is set.
// It should be noted that the Parity Error (PE) bit (LSR[2]) will be set if a break interrupt has occurred, as
// indicated by Break Interrupt (BI) bit (LSR[4]).
#define PARITY_ERROR (1 << 2)

// This is used to indicate the occurrence of an overrun
// error. This occurs if a new data character was received
// before the previous data was read.
#define OVERRUN_ERROR (1 << 1)

// This is used to indicate that the receiver contains at
// least one character in the RBR or the receiver FIFO. This
// bit is cleared when the RBR is read in the non-FIFO
// mode, or when the receiver FIFO is empty, in the FIFO mode.
#define DATA_READY_BIT (1)

void uartinit(void);

typedef enum {UART_READ = 0, UART_WRITE = 1} UART_MODE;
typedef enum {
        ERROR_AT_TOP = -5,
        FIFO_IS_CLEAR = -4,
        NON_IMPLEMENTED = -3,
        THRNONEMPTY = -2,
        UNRESOLVED_CONDITION = -1,
        OPSUCCESS = 0
    } UART_OPERATION_STATUS;
UART_OPERATION_STATUS uartrw(char * rw, UART_MODE mode);

#endif 