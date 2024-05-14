#ifndef _TASK_H
#define _TASK_H

#include "common.h"

typedef enum {
    RUNABLE,
    RUNNING,
    SLEEP,
} task_state;

struct trapframe{
    uint64_t ra; // 0(sp)
    uint64_t sp; // 8(sp)
    uint64_t gp; // 16(sp)
    uint64_t tp; // 24(sp)
    uint64_t t0; // 32(sp)
    uint64_t t1; // 40(sp)
    uint64_t t2; // 48(sp)
    uint64_t s0; // 56(sp)
    uint64_t s1; // 64(sp)
    uint64_t a0; // 72(sp)
    uint64_t a1; // 80(sp)
    uint64_t a2; // 88(sp)
    uint64_t a3; // 96(sp)
    uint64_t a4; //104(sp)
    uint64_t a5; //112(sp)
    uint64_t a6; //120(sp)
    uint64_t a7; //128(sp)
    uint64_t s2; //136(sp)
    uint64_t s3; //144(sp)
    uint64_t s4; //152(sp)
    uint64_t s5; //160(sp)
    uint64_t s6; //168(sp)
    uint64_t s7; //176(sp)
    uint64_t s8; //184(sp)
    uint64_t s9; //192(sp)
    uint64_t s10; //200(sp)
    uint64_t s11; //208(sp)
    uint64_t t3; //216(sp)
    uint64_t t4; //224(sp)
    uint64_t t5; //232(sp)
    uint64_t t6; //240(sp)

    uint64_t sepc;    // 248(sp)
    uint64_t sstatus; // 256
};

struct task{
    uint64_t stack; // 0
    uint64_t satp; //  8
    uint64_t ret0; // 16

    bool pure;
    uint8_t padding[8];
};

struct cpu{
    struct task current_task; 
    uint64_t ksatp;  // 40 (cpu)
    uint64_t kstack; // 48
    uint64_t traphandler; // 56
    uint8_t id;

    uint8_t noff; // hom much time we off interrupt
    uint8_t intena; // previous interrupt enable masks
};




#endif 