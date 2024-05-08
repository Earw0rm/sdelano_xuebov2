#include "memlayout.h"


.section .trampoline
.globl uservec
.align 4
uservec:
    addi sp, sp, -8
    sd t0, 0(sp)
    
    la t0, TRAPFRAME

    sd ra, 0(t0)
    # sd sp, 8(t0)
    sd gp, 16(t0)
    sd tp, 24(t0) #last time we in tp
    # sd t0, 32(a0)
    sd t1, 40(t0)
    sd t2, 48(t0)
    sd s0, 56(t0)
    sd s1, 64(t0)
    sd a0, 72(t0)
    sd a1, 80(t0)
    sd a2, 88(t0)
    sd a3, 96(t0)
    sd a4, 104(t0)
    sd a5, 112(t0)
    sd a6, 120(t0)
    sd a7, 128(t0)
    sd s2, 136(t0)
    sd s3, 144(t0)
    sd s4, 152(t0)
    sd s5, 160(t0)
    sd s6, 168(t0)
    sd s7, 176(t0)
    sd s8, 184(t0)
    sd s9, 192(t0)
    sd s10, 200(t0)
    sd s11, 208(t0)
    sd t3, 216(t0)
    sd t4, 224(t0)
    sd t5, 232(t0)
    sd t6, 240(t0)

    mv t1, t0 
    ld t0, 0(sp)
    addi sp, sp, 8
    sd t0, 32(t1) 
    sd sp, 8(t1)

    csrr t0, sepc 
    sd t0, 248(t1)

    # now all context are saved    
    # load kernel stack, kernel pagetable and kernel trapvector

    ld sp, 264(t1) 

    ld t0, 272(t1)
    csrw t0, stvec 

    ld t2, 256(t1) 

    srli t2, t2, 12
    csrw t2, satp

    sfense.vma x0, x0

    call usertrap    #trap.c

.globl uservec
uservecret:
    csrr t0, sscratch
    ld t0, 0(t0)
    
    #store kernel stack pointer before return to user mode
    sd sp, 264(t0) 

    ld ra, 0(t0)
    ld sp, 8(t0)
    ld gp, 16(t0)
    # not tp (contains hartid), in case we moved CPUs
    # we need to set tp in that task before we call this function 
    # ld t0, 32(t0)
    # ld t1, 40(t0)
    ld t2, 48(t0)
    ld s0, 56(t0)
    ld s1, 64(t0)
    ld a0, 72(t0)
    ld a1, 80(t0)
    ld a2, 88(t0)
    ld a3, 96(t0)
    ld a4, 104(t0)
    ld a5, 112(t0)
    ld a6, 120(t0)
    ld a7, 128(t0)
    ld s2, 136(t0)
    ld s3, 144(t0)
    ld s4, 152(t0)
    ld s5, 160(t0)
    ld s6, 168(t0)
    ld s7, 176(t0)
    ld s8, 184(t0)
    ld s9, 192(t0)
    ld s10, 200(t0)
    ld s11, 208(t0)
    ld t3, 216(t0)
    ld t4, 224(t0)
    ld t5, 232(t0)
    ld t6, 240(t0)

    ld t1, 248(t0) # sepc 
    csrw t1, sepc

    ## set vector and user pagetable
    la t1, uservec
    csrw t1, stvec 

    csrr t0, sscratch
    ld t1, 8(t0)
    srli t1, t1, 12
    csrw t1, satp 
    
    sfense.vma x0, x0

    la t0, TRAPFRAME 
    ld t1, 40(t0)
    ld t0, 32(t0)
    sret    