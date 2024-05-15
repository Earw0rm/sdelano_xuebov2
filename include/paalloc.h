#ifndef _PAALLOC_H
#define _PAALLOC_H
#include "common.h"

#define ALLOC_NUM 10

struct run{
    struct run* next;
};

uint64_t allocpage(void);
#endif 