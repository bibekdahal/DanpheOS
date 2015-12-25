typedef struct
{
    uint prev;
    uint size;
    uint free;
} mm_header;


typedef struct
{
    mm_header * start;
    uint size;
} heap;
