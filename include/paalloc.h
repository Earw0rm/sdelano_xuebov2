#ifndef _PAALLOC_H
#define _PAALLOC_H
#include "common.h"

struct run{
    struct run* next;
};

uint64_t allocpage(void);
#endif 