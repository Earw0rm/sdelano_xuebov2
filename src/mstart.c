#include "uart.h"


void mstart(uint8_t mhartid){
    if(mhartid != 0){
        while(1);
    }
    
    init_uart();


    uart_send_unsafe("Hello world!\r\n");
    while(1);
}