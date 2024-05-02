.section .data.boot.mstack
.align 12
mstack: .space 0x4000

.section .data.boot
.section .text.boot

.global _start
_start:
    csrr t0, mhartid
    addi t0, t0, 1
    slli t0, t0, 12 
    la   sp, _mstack_bot
    add  sp, sp, t0 

    la gp, _global_pointer
    csrr a0, mhartid
    j mstart
    j _proc_hand
    wfi


_proc_hand:
    wfi
    j _proc_hand

