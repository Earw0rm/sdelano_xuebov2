#include "speenlock.h"
#include "riscv.h"
#include "memlayout.h"
#include "common.h"
#include "utils.h"
/**
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
 * 
 * 
 * ldra - load acquire 
 * stlr - store release 
 * 
*/


/**
 * 1) we must remember interrupt status before acquire call
 * 2) release return interrupt status in state before acquire call 
 * 3) we need to held counter. Acquire increment this countr, where release dicrement. 
 *    If counter == 0 return interrupt status
 * 4) cpus struct it is nice place to hold noff(number of off interrupt) and intena (interrupt enable)
 * 5) intr of and intr on manipulate this variables
 * 6) push_off and push_on manipulate lock and intr_on inter_off
 * 
*/

void acquire(struct speenlock * lock){
    
    push_off();
    while(__atomic_test_and_set(&(lock->locked), __ATOMIC_ACQ_REL) != 0){
        asm volatile("nop");
    }

    lock->cpu = mycpu;
}



/**
 * __sync_synchronize()
 * __sync_lock_release(&value)
 * turn_on interruptions
*/
void release(struct speenlock *lock){

    if(!holding(lock)){
        // panic
    }
    lock->cpu = (struct cpu *) 0x0;
    __atomic_clear(&(lock->locked), __ATOMIC_RELEASE);
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    push_on();
}


void push_off(){

    if(mycpu->noff == 0){
        bool is_enabled =  is_intr_enabled();
        mycpu->intena = is_enabled;
        if(is_enabled == true) intr_off();
    }
    ++(mycpu->noff);
}

void push_on(){
    if((--(mycpu->noff)) == 0 && mycpu->intena == true) intr_on();
}


bool holding(struct speenlock *lock){
    return (lock->locked && lock->cpu == mycpu);
}