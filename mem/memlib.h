#include <lib.h>
#include <spinlock.h>

struct spinlock mem_lock;

#define MAX_HEAP (8 * (1 << 20))

void init_mem(void);
void mem_deinit(void);
void* mem_sbrk(size_t incr);
void mem_reset_brk(void);
void* mem_heap_lo(void);
void* mem_heap_hi(void);
size_t mem_heapsize(void);
size_t mem_pagesize(void);

