#ifndef _GPIOTH1520_H
#define _GPIOTH1520_H


//4KB long
#define GPIO0_START (0xffec005000)
#define GPIO0_END (0xffec005fff)

#define GPIO1_START (0xffec006000)
#define GPIO1_END (0xffec006fff)

//16KB long 
#define GPIO2_START (0xffe7f34000)
#define GPIO2_END (0xffe7f37fff)

#define GPIO3_START (0xffe7f38000)
#define GPIO3_END (0xffe7f3bfff)


/**
 * GPIO memory map and description
 * todo:beta in terms of offset
*/


//port A data register
#define GPIO_SWPORTA_DR (0x0)

//port A data direction register
#define GPIO_SWPORTA_DDR (0x4)

//interrupt enable register
#define GPIO_INTEN (0x30)


//interrupt mask register
#define GPIO_INTMASK (0x34)

// Interrupt level. Controls the type of interrupt that can
// occur on Port A. Whenever a 0 is written to a bit of this
// register, it configures the interrupt type to be levelsensitive; otherwise, it is edge-sensitive.
#define GPIO_INTTYPE_LEVEL (0x38)

// Interrupt level Interrupt polarity. Controls the polarity
// of edge or level sensitivity that can occur on input of
// Port A. Whenever a 0 is written to a bit of this register,
// it configures the interrupt type to falling-edge or
// active-low sensitive; otherwise, it is rising-edge or
// active-high sensitive.
#define GPIO_INT_POLARITY (0x3c)

//interrupt status
#define GPIO_INTSTATUS (0x40) 

//raw interrupt status
#define GPIO_RAW_INTSTATUS (0x44)

//port A clear interrupt register
#define GPIO_PORTA_EOI (0x4c)

//external port A register
#define GPIO_EXT_PORTA (0x50)

//Synchronization level
// Writing a 1 to this register results in all level-sensitive
// interrupts being synchronized to pclk_intr.
#define GPIO_LS_SYNC (0x60)

//gpio id code
#define GPIO_ID_CODE (0x64)

//gpio component version
#define GPIO_VER_ID_CODE (0x6c)


//gpio configuration register 2
#define GPIO_CONFIG_REG2 (0x70)

//gpio configuration register 1
#define GPIO_CONFIG_REG1 (0x74)


#endif 