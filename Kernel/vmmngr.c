#include "common.h"

// virtual address has format
// AAAAAAAAAA         BBBBBBBBBB        CCCCCCCCCCCC
// directory index    page table index  offset into page
// use this format to obtain required addresses
#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
// Get frame address from table/page entry
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

typedef uint page;
typedef uint pdentry;
typedef struct page_table_t
{
    page pages[1024];
} ptable;

typedef struct pdirectory_t
{
    pdentry tables[1024];
} pdirectory;

pdirectory* curdir;   // current directory
uint        curpdbr;  // page directory base register : physical address : cr3 register

int vm_alloc_page (page* e)
{
    if ((*e & 1) == 1) return;
    void* p = (void*)pm_alloc();
    if (!p)
        return 0;
    *e = (uint)p | 3;
    return 1;
}
void vm_free_page(page* e)
{
    void* p = (void*)(uint)e;
    if (p)  pm_free(p);
    *e &= ~1;   // not present
}

void map_page (void* phys, void* virt)
{
    pdirectory* pageDirectory = curdir;

    pdentry* e = &pageDirectory->tables [PAGE_DIRECTORY_INDEX((uint)virt)];
    if ((*e & 1) != 1)    // if table not "present"
    {
        ptable* table = (ptable*) pm_alloc ();
        if (!table)
            return;
        memset (table, 0, sizeof(ptable));
        *e = (uint)table | 3;
    }

    ptable* table = (ptable*)PAGE_GET_PHYSICAL_ADDRESS(e);
    table->pages [PAGE_TABLE_INDEX ((uint)virt)] = (uint)phys | 3;
}

extern void enablepaging(uint ENABLE /* 1 Or 0*/);
void loadpdbr(uint pdbr)
{
    asm volatile("mov %0, %%cr3":: "b"(pdbr));
}
void vm_switchdir(pdirectory* dir)
{
    if (!dir) return;
    curdir = dir;
    curpdbr = (uint)&dir->tables;
    loadpdbr(curpdbr);
}


void vm_initialize()
{
    pdirectory* dir = (pdirectory*) pm_alloc();
    if (!dir)   return;
    memset(dir,0,sizeof(pdirectory));

    int i;
    ptable* table = (ptable*) pm_alloc();
    if (!table) return;
    memset (table, 0, sizeof(ptable));

    int frame=0x0;
    for (i=0; i<1024; i++)
    {
        table->pages[i] = frame | 3;
        frame+=4096;
    }
    dir->tables[0] = (uint)table | 3;

    ptable* table2 = (ptable*) pm_alloc();
    if (!table2)    return;
    memset (table2, 0, sizeof(ptable));

    frame=0x100000;
    for (i=0; i<1024; i++)
    {
        table2->pages[i] = frame | 3;
        frame+=4096;
    }
    dir->tables[768] = (uint)table2 | 3;

    vm_switchdir(dir);

}

/*Make sure page table exists / create new if not*/
page* vm_getpage(uint addr)
{
    pdentry* e = &curdir->tables [PAGE_DIRECTORY_INDEX((uint)addr)];

    if ((*e & 1) != 1)    // if table not "present"
    {
        uint i;

        ptable* table = (ptable*) pm_alloc ();
        if (!table)
            return 0;
        memset (table, 0, sizeof(ptable));
        *e = (uint)table | 3;
    }

    ptable* table = (ptable*)PAGE_GET_PHYSICAL_ADDRESS(e);
    return & table->pages [PAGE_TABLE_INDEX ((uint)addr)];
}

