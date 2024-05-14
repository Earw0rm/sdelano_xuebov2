#include "common.h"
#include "param.h"
#include "task.h"
#include "riscv.h"
#include "memlayout.h"





void * memset(void *str, int c, size_t n){
    void * str_cpy = str;
    for(size_t i = 0; i < n; ++i){
        *((char *)str_cpy) = (char) c;
        str_cpy = (void *)(((char *)str_cpy) + 1);
    }
    return str;
}

//copies n characters from memory area src to memory area dest.
void * memcpy(void * dest, const void * src, size_t n){
    char * dest_cpy = (char *) dest;
    char * src_cpy  = (char *) src;
    for(size_t i = 0; i < n; ++i, ++dest_cpy, ++src_cpy){
        *dest_cpy = *src_cpy;   
    }
    return dest;
}
