#include "paalloc.h"
#include "memlayout.h"
#include "common.h"
#include "speenlock.h"
#include "vm.h"
#include "riscv.h"
#include "utils.h"


struct run* freemem = (struct run*) 0x0;
uint64_t pa_alloc_pointer = (uint64_t)(&_kernel_end + 0x1000);
uint64_t freememcount = 0;//in pages

struct speenlock freememlock;

//lock must be held 
void add_pages(void){
    struct cpu* cpu = mycpu();
    uint64_t pgtbl = (cpu->ksatp & SATP_PPN_MASK) << 12;

    //if lock.cpu_id != my_cpu_id then panic
    for(uint8_t i = 0; i < ALLOC_NUM; ++i){
        //if pa_alloc_pointer >= PHYSTOP then panic
        struct run * new_page = (struct run *) pa_alloc_pointer;
        pa_alloc_pointer += 0x1000;         
        new_page->next = freemem;
        freemem = new_page;
    }

    freememcount += ALLOC_NUM;
}



uint64_t allocpage(void){
    acquire(&freememlock);

        struct run * page;

        if(freememcount <= 4) add_pages();
        
        page = freemem;

        if(page == (struct run *)0x0){            
            // panic
        }

        freemem = page->next;
        --freememcount;
    release(&freememlock);
    
    return (uint64_t) page;
}

