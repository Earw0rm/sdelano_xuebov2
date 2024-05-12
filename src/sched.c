#include "speenlock.h"
#include "task.h"
#include "utils.h"
#include "param.h"
#include "paalloc.h"
#include "vm.h"
#include "memlayout.h"
#include "printf.h"
#include "riscv.h"

struct speenlock tasks_lock = {
    .cpu_num = -1,
    .locked = 0,
    .name = "clear"
};
//acquire lock for changes this 
static int64_t tasks_count   = -1 ;
static struct task tasks[NPROC] = {0};

//completed tasks
static int64_t tasks_buffer_count   = -1 ;
static struct task task_buffer[NPROC]  = {0};



struct task get_last(void){
    struct task ret = {0};
    acquire(&tasks_lock);
        if(tasks_count >= 0){
            ret = tasks[tasks_count];
            --tasks_count;
        }
    release(&tasks_lock);
    return ret;
}

void switch_to(struct task * new_task){ 
    struct cpu * my_cpu = (struct cpu *)MYCPU;
    struct task old_task = my_cpu->current_task;
    my_cpu->current_task = *new_task;


    if(old_task.pure){//sinse first task is not pure...
        acquire(&tasks_lock);
            task_buffer[++tasks_buffer_count] = old_task;
        release(&tasks_lock);        
    }
    printf("################################################ \r\n");
    ppgtbl(SATP2PPN(new_task->satp));
}

void schedule(void){
    struct task task = get_last();
    if(!task.pure){
        acquire(&tasks_lock);   
            if(tasks_buffer_count >= 0){
                    for(;tasks_buffer_count >= 0; --tasks_buffer_count){
                    tasks[++tasks_count] = task_buffer[tasks_buffer_count]; 
                }
            }
        release(&tasks_lock); 
        return;
    }
    switch_to(&task); 
}






struct task task_create(uint64_t start_addr, uint64_t pgtbl, uint64_t stack, uint64_t sstatus){
    struct task task = {0};
    struct trapframe tf = {0};

    tf.sepc = start_addr;
    tf.sstatus = sstatus;
    tf.sp = (uint64_t)(((char *)USTACK) + sizeof(struct trapframe));


    *((struct trapframe *) stack) = tf;

    task.satp = (8ull << 60) | (pgtbl >> 12);
    task.stack = tf.sp;
    task.pure = 1;

    return task;
}

struct task user_task_create(uint8_t (*main)(void)){
    int8_t map_res; 

    uint64_t pgtbl = allocpage();
    uint64_t stack = allocpage();
    uint64_t sstatus = r_sstatus();
    sstatus &= ~(1 << 8);// set spp = 0
    sstatus |= (1 << 5);

    memset((void *) pgtbl, 0, 0x1000);
    memset((void *) stack, 0, 0x1000);

    
    if((&_trampoline_end - &_trampoline_start) > 0x1000){
        printf("PANIC! user_task_create => trampoline is more than one page \r\n");
        while(1);
    }


    map_res = mapva((uint64_t) USTACK, stack, (pagetable_t) pgtbl,  PTE_W | PTE_R | PTE_U, true);    
    map_res = mapva((uint64_t) 0x0, (uint64_t) main, (pagetable_t)  pgtbl, PTE_XWRDA | PTE_U, true);    
    map_res = mapva(TRAMPOLINE, (uint64_t) kernelvec, (pagetable_t) pgtbl, PTE_XWRDA, true);



    struct task task = task_create(0x0, pgtbl, stack, sstatus);

    return task;
}

uint8_t fork(uint8_t (*main)(void)){
    struct task task = user_task_create(main);



    acquire(&tasks_lock);
        uint8_t task_id = ++tasks_buffer_count;
        task_buffer[task_id] = task;
    release(&tasks_lock);
    return task_id;
}