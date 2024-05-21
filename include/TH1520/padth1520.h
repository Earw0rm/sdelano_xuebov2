#ifndef _PAD1520_H
#define _PAD1520_H

#include "common.h"

// ***************** Physical Access Device *****************
// All the PADs on the chip are divided into three groups. MUX and registers of each group are independent.
// The base Addresses of related registers in each PAD group are also different.
// Most PADs correspond to two registers: PADCFG register and PADMUX register.
//
// The PADMUX register is used to select the PAD working mode.
// The PADCFG register can mainly be used to configure the pull-up/pull-down state of the
// internal resistor of the PAD, the slew rate control of the PAD output signal, the Schmitt control of the
// PAD input signal, and the PAD driver strength.
// 
// Nice table of all PAD with their mux => 3.3.2 Find Register by PAD Name (System user manual page 56)
// **********************************************************


//address map for c906 and c910 
#define G1_BASE_ADDR 0xFFFFF4A000
#define G2_BASE_ADDR 0xFFE7F3C000
#define G3_BASE_ADDR 0xFFEC007000


struct g1_pad_array{
    uint32_t g1_padcfg[24]; 
    char __padding[0x400];
    uint32_t g1_muxcfg[6];  
};

struct g2_pad_array{
    uint32_t g2_padcfg[32];
    char __padding[0x400];
    uint32_t g2_muxcfg[8];  
};

struct g3_pad_array{
    uint32_t g3_padcfg[28];  
    char __padding[0x400];
    uint32_t g3_muxcfg[7];  
};


// #define G1_CFG ((struct g1_pad_array *)G1_BASE_ADDR)
// #define G2_CFG ((struct g2_pad_array *)G2_BASE_ADDR)
// #define G3_CFG ((struct g3_pad_array *)G3_BASE_ADDR)


void padinit(void);


#endif 