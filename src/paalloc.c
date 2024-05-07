#include "paalloc.h"
#include "memlayout.h"
#include "common.h"
#include "speenlock.h"

struct run* freemem = (struct run*) 0x0;
uint64_t pa_alloc_pointer = (uint64_t)(&_kernel_end + 0x1000);

struct speenlock freememlock;


//lock must be held 
void add_pages(void){
    //if lock.cpu_id != my_cpu_id then panic
    for(uint8_t i = 0; i < 10; ++i){
        //if pa_alloc_pointer >= PHYSTOP then panic
        struct run * new_page = (struct run *) pa_alloc_pointer;
        pa_alloc_pointer += 0x1000; 
        new_page->next = freemem;
        freemem = new_page;
    }
}

uint64_t allocpage(void){
    acquire(&freememlock);


        struct run * page = freemem;
        if(page == (struct run *)0x0){
            add_pages();
            page = freemem;
        }



        freemem = page->next;
    release(&freememlock);
    return (uint64_t) page;
}
