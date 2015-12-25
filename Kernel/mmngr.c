#include "common.h"
#include "mmngr.h"

void mm_createheap(uint start_addr, uint size, heap* hp)
{
    uint i;

    size += size%0x1000; // 4K aligned size
    for (i=start_addr;i<=start_addr+size;i+=4096)
        vm_alloc_page(vm_getpage(i));

    mm_header * hd = (mm_header*)start_addr;
    hd->prev = 0;
    hd->size = size;
    hd->free = 1;

    mm_header * hdend = (mm_header*) (start_addr+size - sizeof(mm_header));
    hdend->prev = (uint)hd;
    hdend->size = 0;
    hdend->free = 0;

    hp->start = hd;
    hp->size = size;
}

mm_header* mm_insert(uint size, heap * hp)
{
    mm_header* hd = hp->start;
    while (hd->size!=0)
    {
        if (hd->size > size && hd->free)
        {
            mm_header* newhd = (mm_header*) ((uint)hd + size);
            newhd->prev = (uint)hd;
            newhd->size = hd->size - size;

            mm_header* nexthd = (mm_header*) ((uint)hd + hd->size);
            nexthd->prev = (uint)newhd;
            newhd->free = 1;
            hd->size = size;
            hd->free = 0;
            return hd;
        }
        hd = (mm_header*) ((uint)hd + hd->size);
    }
    return 0;
}



void* mm_alloc(uint size, heap* hp)
{
    mm_header* hd = mm_insert(size+sizeof(mm_header), hp);
    if (hd)
    {
        return (void*) ((uint)hd + sizeof(mm_header));
    }
}

void displayHeap(heap* hp)
{
    mm_header* hd = hp->start;
    while (hd->size!=0)
    {
        puts("0x"); puthex(hd->prev); putch(' '); putdec(hd->size); putch(' '); putdec(hd->free); putch('\n');
        hd = (mm_header*) ((uint)hd + hd->size);
    }
}








