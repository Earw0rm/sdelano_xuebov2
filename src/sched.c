#include "speenlock.h"
#include "task.h"
#include "utils.h"
#include "param.h"
#include "paalloc.h"

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

void switch_to(struct task * new_task){ 
    struct cpu * my_cpu = (struct cpu *)r_sscratch();
    struct task old_task = my_cpu->current_task;
    my_cpu->current_task = *new_task;


    if(old_task.pure){//sinse first task is not pure...
        acquire(&tasks_lock);
            task_buffer[++tasks_buffer_count] = old_task;
        release(&tasks_lock);        
    }

}

struct task user_task_create(uint8_t (*main)(void)){
    struct task task = {0};

    pagetable_t upgtbl = (pagetable_t) allocpage();
    memset((void *) upgtbl, 0, 0x1000);
    task.pgtbl = (uint64_t) upgtbl;

    uint64_t trapframe = allocpage();
    memset((void *) trapframe, 0, 0x1000);
    task.trapframe = (struct trapframe *) trapframe;

    uint64_t ustack = allocpage();
    memset((void *) ustack, 0, 0x1000);
    task.trapframe->sp = ustack;

    // task.trapframe->kpgtbl = 

    // 1) set kpgtbl and kernel vec
    // 2) подумать о том как хранить адреса таблицы страниц. Сдвинутые, не сдвинутые, и тд. 


    // for(char * pointer = &_kernel_start; pointer < &_kernel_end; pointer += 0x1000){
    //     int8_t res = mapva((uint64_t) pointer, (uint64_t) pointer, pgtbl, PTE_XWRDA, true);
    //     if(res < 0) return -1;
    // }

}
