#include "vm.h"
#include "paalloc.h"
#include "memlayout.h"
#include "riscv.h"
#include "utils.h"
#include "printf.h"
__attribute__ ((aligned (0x1000))) char kpgtbl[0x4000] = {0};


pte_t * walk(uint64_t va, pagetable_t pgtbl, bool alloc){

    for(uint8_t level = 2; level != 0; --level){
        pte_t * pte = &pgtbl[VAPDIND(va, level)];
        
        if(((*pte) & PTE_V) == 1){
            pgtbl = (pagetable_t)(PTE2PA(*pte));
        }else if(alloc == true){
            uint64_t page = allocpage();
            memset((void *) page, 0, 0x1000);
            *pte = (PA2PTE(page) | PTE_V);
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

        pte_t * pte = walk(va, pgtbl, alloc);
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




    for(char * pointer = &_kernel_start; pointer < &_kernel_end; pointer += 0x1000){
        int8_t res = mapva((uint64_t) pointer, (uint64_t) pointer, pgtbl, PTE_XWRDA, true);
        if(res < 0) return -1;
    }

    return (uint64_t) pgtbl;
}


void pagetable_debug(pagetable_t pgtbl, uint64_t level){
    for(uint16_t i = 0; i <= 511; ++i){
        uint64_t val = pgtbl[i];
        if(val & PTE_V){
            for(uint8_t j = 2; j > level; --j){
                printf("\t");
            }
            
            printf("ind:%x, val:%x, pte2pa:%x \r\n", i, val, PTE2PA(val));


            if(level != 0) {
                pagetable_debug((pagetable_t) PTE2PA(val), level - 1);
            }
        }
    }
}
void ppgtbl(pagetable_t pgtbl){
    pagetable_debug(pgtbl, 2);
}