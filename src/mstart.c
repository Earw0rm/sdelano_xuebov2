#include "uart.h"


void mstart(void){
    init_uart();


    uart_send_unsafe("Hello world!\r\n");
    while(1);
}