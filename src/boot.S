.section .data.boot
_bss_is_clear:
    .quad 0x0
.section .text.boot

.global _start
_start:
    csrr t0, mhartid
    addi t0, t0, 1
    slli t0, t0, 12 
    la   sp, stack0
    add  sp, sp, t0 

    la gp, _global_pointer

    csrr a0, mhartid
    bnez a0, _wait_zero_bss
    la t1, _bss_start
    la t2, _bss_end
_zero_bss:
    sub t3, t2, t1
    beqz t3, _break_zero_bss
    sd x0, (t1)
    addi t1, t1, 8
    j _zero_bss
_break_zero_bss:
    li t1, 1
    la t2, _bss_is_clear
    sd t1, (t2)
    csrr a0, mhartid
    j mstart

    j _proc_hand
_wait_zero_bss:
    la t2, _bss_is_clear
    ld t1, (t2)
    beqz t1, _wait_zero_bss
    csrr a0, mhartid
    j mstart
_proc_hand:
    wfi
    j _proc_hand

