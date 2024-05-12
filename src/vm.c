#include "vm.h"
#include "paalloc.h"
#include "memlayout.h"
#include "riscv.h"
#include "utils.h"
#include "printf.h"
#include "param.h"
#include "task.h"

__attribute__ ((aligned (0x1000))) char kpgtbl[0x1000 * NCPU] = {0};

//mstart.c
extern struct cpu cpus[NCPU];

pte_t * walk(uint64_t va, pagetable_t pgtbl,  uint16_t flags, bool alloc){
    flags = flags & (0xf0); 

    for(uint8_t level = 2; level != 0; --level){
        pte_t * pte = &pgtbl[VAPDIND(va, level)];
        
        if(((*pte) & PTE_V) == 1){
            pgtbl = (pagetable_t)(PTE2PA(*pte));
        }else if(alloc == true){
            uint64_t page = allocpage();
            memset((void *) page, 0, 0x1000);
            *pte = (PA2PTE(page) | PTE_V | flags);
        }else{
            //panic
            return (pte_t *) 0x0;
        }

        pgtbl = (pagetable_t)PTE2PA(*pte);
    }

    return &pgtbl[VAPDIND(va, 0)];
}


int8_t mapva(uint64_t va, uint64_t pa, pagetable_t pgtbl,  uint16_t flags, bool alloc){
        va = ADDRROUNDDOWN(va);
        pa = ADDRROUNDDOWN(pa);

        pte_t * pte = walk(va, pgtbl, flags, alloc);
        if(pte == (pte_t*)0x0){
            //panic
            return -1;
        }
        uint64_t pte_str = PA2PTE(pa) | PTE_V | flags;
        *pte = pte_str;
        return 0;
}

uint64_t kpgtbl_init(void){

    uint8_t id = r_mhartid();
    pagetable_t pgtbl = (pagetable_t)&(kpgtbl[id << 12]);
    int8_t map_res;

    for(char * pointer = &_kernel_start; pointer < &_kernel_end; pointer += 0x1000){
        map_res = mapva((uint64_t) pointer, (uint64_t) pointer, pgtbl, PTE_XWRDA, true);
        if(map_res < 0) return -1;
    }

    map_res = mapva(TRAMPOLINE, (uint64_t) kernelvec, pgtbl, PTE_XWRDA, true);
    map_res = mapva(MYCPU, (uint64_t) &cpus[id], pgtbl, PTE_XWRDA, true);
    map_res = mapva(UART, UART, pgtbl, PTE_XWRDA, true);


    return (uint64_t) pgtbl;
}


void pagetable_debug(pagetable_t pgtbl, uint64_t level){
    for(uint16_t i = 0; i <= 511; ++i){
        uint64_t val = pgtbl[i];
        if(val & PTE_V){
            for(uint8_t j = 2; j > level; --j){
                printf("\t");
            }

            printf("ind:%x, val:%x, pte2pa:%X \r\n", i, val, PTE2PA(val));


            if(level != 0) {
                pagetable_debug((pagetable_t) PTE2PA(val), level - 1);
            }
        }
    }
}
void ppgtbl(pagetable_t pgtbl){
    printf("###################pagetable:%x###################\r\n", (uint64_t)pgtbl);
    pagetable_debug(pgtbl, 2);
}