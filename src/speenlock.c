#include "speenlock.h"
#include "riscv.h"
/**
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
 * 
 * 
 * ldra - load acquire 
 * stlr - store release 
 * 
*/


void acquire(struct speenlock * lock){
    

//disable interrupts
    // intr_off();
    while(__atomic_test_and_set(&(lock->locked), __ATOMIC_ACQ_REL) != 0){
        asm volatile("nop");
    }

}



/**
 * __sync_synchronize()
 * __sync_lock_release(&value)
 * turn_on interruptions
*/
void release(struct speenlock *lock){

    lock->name = "unlocked";
    __atomic_clear(&(lock->locked), __ATOMIC_RELEASE);
    // intr_on();
}