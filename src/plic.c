#include "plicth1520.h"


#define PLICC906 ((struct plicmap *) C906_PLIC_BASE)
#define PLICC910 ((struct plicmap *) C910_PLIC_BASE)

void plicinit(void){
    // set desired IRQ priorities non-zero (otherwise disabled).
    PLICC910->priority[INT_AOSYSUART] = 1;
    PLICC910->priority[INT_UART0] = 1;
}

void plicinithart(uint8_t mhartid){
    PLICC910->enables_array[mhartid][3] = 1 << 14; // AOSYSUART
    PLICC910->enables_array[mhartid][1] = 1 << 4; // AOSYSUART
    PLICC910->context_controls[mhartid].priority_threshold = 0;
}

// return 0 if there are no interrupt pending
uint32_t plic_claim(uint8_t mhartid){
    return PLICC910->context_controls[mhartid].claim_complete;
}

void plic_complete(uint8_t mhartid, uint32_t irq){
     PLICC910->context_controls[mhartid].claim_complete = irq;
}