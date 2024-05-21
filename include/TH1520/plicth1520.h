#ifndef _PLIC1520_H
#define _PLIC1520_H

#include "common.h"

#define C906_PLIC_BASE 0xFFD0000000
#define C910_PLIC_BASE 0xFFD8000000

struct plicmap{

    volatile uint32_t priority[1024]; // 0x000000 - 0x000FFC: Interrupt source priorities

    volatile uint32_t pending_array[32];// 0x001000 - 0x00107C: Interrupt pending bits
    volatile uint32_t _reserved1[992];  // 0x001080 - 0x001FFC: Reserved

    volatile uint32_t enables_array[15872][32]; // 0x002000 - 0x1F1FFC: Enable bits for contexts 0-15871 and sources 0-1023
    volatile uint32_t _reserved2[209664]; // 0x1F2000 - 0x1FFFFF: Reserved

    struct {
        volatile uint32_t priority_threshold; // Priority threshold for context
        volatile uint32_t claim_complete; // Claim/complete for context
        volatile uint32_t reserved[1022]; // Reserved space
    } context_controls[15872]; // 0x200000 - 0x3FFFFFC: Controls for contexts 0-15871

};

// 0-15 interrupts of E902/C906/C910 are reserved for internal use, so the actual interrupt number is the interrupt
// number plus 16 in the following tables.

#define INTRNUMBASE 16
#define INT_AOSYSUART (INTRNUMBASE + 62)
#define INT_UART0     (INTRNUMBASE + 20)


void plicinit(void);
void plicinithart(uint8_t mhartid);
uint32_t plic_claim(uint8_t mhartid);
void plic_complete(uint8_t mhartid, uint32_t irq);



#endif 