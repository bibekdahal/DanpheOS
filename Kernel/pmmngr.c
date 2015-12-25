#include "common.h"

// Each bit in "frames" represents a 4K frame
uint* mmap;
uint memsize;
uint maxblocks;
uint usedblocks;

static void pm_set(int bit)
{
    mmap[bit / 32] |= (1 << (bit % 32));
}
static void pm_unset(uint bit)
{
     mmap[bit / 32] &= ~ (1 << (bit % 32));
}
static int pm_test(uint bit)
{
    return mmap[bit / 32] &  (1 << (bit % 32));
}
static int pm_firstfree()
{
    uint i;
    int j;
    for (i = 0; i < maxblocks; i++)
        if (mmap[i] != 0xFFFFFFFF)
            for (j = 0; j < 32; j++)
            {
                int bit = 1 << j;
                if ( !(mmap[i]&bit) )
                    return i*4*8+j;
            }

    return -1;
}

static int pm_firstfrees(uint size)
{
    uint i,count,free;
    int j, startingBit, bit;
	if (size==0)    return -1;
	if (size==1)	return pm_firstfree();

	for (i=0; i<maxblocks; i++)
	{

		if (mmap[i] != 0xFFFFFFF)
        {
            for (j=0; j<32; j++)
            {
				bit = 1<<j;
				if (! (mmap[i] & bit) )
				{
					startingBit = i*32;
					startingBit+=bit;
					free=0;
					for (count=0; count<=size;count++)
					{
						if (! pm_test (startingBit+count) )
							free++;
						if (free==size)
							return i*4*8+j;
					}
				}
			}
        }
	}



	return -1;
}



void pm_init(uint MemSize, uint bitmapAddr)
{
    memsize = MemSize;
    mmap = (uint*) bitmapAddr;
    maxblocks = (MemSize*1024)/4096;        // Each block is 4K (4096)
    usedblocks = maxblocks;
    memset(mmap, 0xF, maxblocks/8);
}

void pm_init_region(uint base, uint size)
{
	int align = base / 4096;
	int blocks = size / 4096;
	for (; blocks>0; blocks--) {
		pm_unset(align++);
		usedblocks--;
	}
	pm_set (0);	//first block is always set. This insures allocs cant be 0
}

void pm_deinit_region(uint base, uint size)
{
	int align = base / 4096;
	int blocks = size / 4096;
	for (; blocks>0; blocks--) {
		pm_set (align++);
		usedblocks++;
	}
	pm_set (0);
}

void* pm_alloc()
{
	if (usedblocks >= maxblocks)
		return 0;
	int frame = pm_firstfree();
	if (frame == -1)
		return 0;
	pm_set(frame);
	uint addr = frame * 4096;
	usedblocks++;
	return (void*)addr;
}

void pm_free(void* p)
{

	uint addr = (uint)p;
	int frame = addr / 4096;
	pm_unset (frame);
	usedblocks--;
}

void* pm_allocs(uint size)
{
	if (usedblocks >= maxblocks)
		return 0;
	int frame = pm_firstfrees(size);
	if (frame == -1)
		return 0;
    uint i;
	for (i=0; i<size; i++)
		pm_set(frame+i);
	uint addr = frame * 4096;
	usedblocks+=size;
	return (void*)addr;
}

void pm_frees(void* p, uint size)
{
	uint addr = (uint)p;
	int frame = addr / 4096;
	uint i;
	for (i=0; i<size; i++)
		pm_unset(frame+i);
	usedblocks-=size;
}
