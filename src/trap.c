#include "common.h"
#include "riscv.h"
#include "uart.h"
#include "printf.h"

#define SUPERVISOR_SOFTWARE_INTERRUPT (0x8000000000000000L | 1)
#define SUPERVISOR_EXTERNAL_INTERRUPT (0x8000000000000000L | 9)




uint8_t askintr(void){
    uint64_t scause = r_scause();
    switch (scause){
        case SUPERVISOR_SOFTWARE_INTERRUPT:
            /* code */

            w_sip(r_sip() & ~2); // asc
            return 1;
            break;
        case SUPERVISOR_EXTERNAL_INTERRUPT:
            return 2;
        default:
            return 0;
            break;
    }
}

void kerneltrap(void){
    printf("Hello kerneltrap \r\n");
    uint8_t askres = askintr();
}

void usertrap(void){
    printf("Hello usertrap \r\n");
}