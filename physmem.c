#include "system.h"

uint32_t nframes;
uint32_t usedframes;
uint32_t * frames;

static inline void set_frame(uint32_t bit)
{
	frames[bit/32] |= (1 << (bit%32));
}

static inline void unset_frame(uint32_t bit)
{
	frames[bit/32] &= ~(1 << (bit%32));
}

static inline int test_frame(uint32_t bit)
{
	return frames[bit/32] & (1 << (bit%32));
}

static int first_free_frame()
{
	uint32_t i, j;
	for (i=0; i<nframes/32; ++i)
		if (frames[i] != 0xFFFFFFFF)
			for (j=0; j<32; ++j)
				if (!(frames[i] & (1 << j)))
					return i*32+j;
	return -1;
}


///////////////////////////////////////////////////


void pm_init(uint32_t memsizeInKB, uint32_t * bitmapaddr)
{
	usedframes = nframes = memsizeInKB / 4;	// each frame is 4KB
	frames = bitmapaddr;
	memset((char*)frames, 0xF, sizeof(uint32_t) * nframes / 8);	// marks as all used
}

void pm_init_region(uint32_t baseaddr, uint32_t size)
{
	unsigned a, b;
	for (a=baseaddr/4096,b=size/4096; b>0; --b)
	{
		unset_frame(a++);
		usedframes++;
	}
	set_frame(0); 													// 0x0 address is always invalid
}

void pm_deinit_region(uint32_t baseaddr, uint32_t size)
{
	unsigned a, b;
	for (a=baseaddr/4096,b=size/4096; b>0; --b)
	{
		set_frame(a++);
		usedframes--;
	}
}

void* pm_alloc_frame()
{
	if (usedframes == nframes) return 0;
	int frame = first_free_frame();
	if (frame == -1) return 0;
	set_frame(frame);
	usedframes++;

	return (void*)(frame*4096);
}

void pm_free_frame(void* addr)
{
	uint32_t frame = (uint32_t)addr / 4096;
	if (!test_frame(frame)) return;
	unset_frame(frame);
	usedframes--;
}