#ifndef _UTILS_H
#define _UTILS_H
#include "common.h"


// str pointer to the block of memory to fill.
// c value to be set. 
// n number of bytes to be set to the value.
void * memset(void *str, int c, size_t n);

//copies n characters from memory area src to memory area dest.
void * memcpy(void * dest, const void * src, size_t n);

#endif 