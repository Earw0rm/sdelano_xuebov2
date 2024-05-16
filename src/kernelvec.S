#include "memlayout.h"

.section .trampoline
.globl kernelvec
kernelvec: # traframe 
    addi sp, sp, -256
    sd ra, 0(sp)
    sd sp, 8(sp)
    sd gp, 16(sp)
    sd tp, 24(sp) #last time we in tp
    sd t0, 32(sp)
    sd t1, 40(sp)
    sd t2, 48(sp)
    sd s0, 56(sp)
    sd s1, 64(sp)
    sd a0, 72(sp)
    sd a1, 80(sp)
    sd a2, 88(sp)
    sd a3, 96(sp)
    sd a4, 104(sp)
    sd a5, 112(sp)
    sd a6, 120(sp)
    sd a7, 128(sp)
    sd s2, 136(sp)
    sd s3, 144(sp)
    sd s4, 152(sp)
    sd s5, 160(sp)
    sd s6, 168(sp)
    sd s7, 176(sp)
    sd s8, 184(sp)
    sd s9, 192(sp)
    sd s10, 200(sp)
    sd s11, 208(sp)
    sd t3, 216(sp)
    sd t4, 224(sp)
    sd t5, 232(sp)
    sd t6, 240(sp)

    csrr t0, sepc 
    sd t0, 248(sp)

    csrr t0, sstatus
    sd t0, 256(sp)

    csrr t0, satp 
    mv   t1, sp 

    # context saved
    # change to kernel pgtbl
    li t2, MYCPU
    ld t2, 40(t2)
    csrw satp, t2
    sfence.vma x0, x0 

    li t2, MYCPU 
    ld sp, 48(t2)

    #save uset stack and pagetable
    sd t1, 0(t2)
    sd t0, 8(t2)
    
    ld t0, 56(t2)
    jalr ra, t0, 0

.globl kernelvecret
kernelvecret: # traframe нам надо взять наш стэк с начала и нашу таблицу страниц 
    
    li t0, MYCPU 
    ld sp, 0(t0)
    ld t1, 8(t0)

    csrw satp, t1 
    sfence.vma x0, x0 

    ld ra, 0(sp)
    ld gp, 16(sp)
    # not tp (contains hartid), in case we moved CPUs
    # ld t0, 32(sp)
    ld t1, 40(sp)
    ld t2, 48(sp)
    ld s0, 56(sp)
    ld s1, 64(sp)
    ld a0, 72(sp)
    ld a1, 80(sp)
    ld a2, 88(sp)
    ld a3, 96(sp)
    ld a4, 104(sp)
    ld a5, 112(sp)
    ld a6, 120(sp)
    ld a7, 128(sp)
    ld s2, 136(sp)
    ld s3, 144(sp)
    ld s4, 152(sp)
    ld s5, 160(sp)
    ld s6, 168(sp)
    ld s7, 176(sp)
    ld s8, 184(sp)
    ld s9, 192(sp)
    ld s10, 200(sp)
    ld s11, 208(sp)
    ld t3, 216(sp)
    ld t4, 224(sp)
    ld t5, 232(sp)
    ld t6, 240(sp)

    ld t0, 248(sp)
    csrw sepc, t0 

    ld t0, 256(sp)
    csrw sstatus, t0 

    ld t0, 32(sp)

    addi sp, sp, 256
    sret    
