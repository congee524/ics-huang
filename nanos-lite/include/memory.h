#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#ifndef PGSIZE
#define PGSIZE 4096 // 4kb
#endif

#define PG_ALIGN __attribute((aligned(PGSIZE)))

#define PGMASK          (PGSIZE - 1)    // Mask for bit ops
#define PGROUNDUP(sz)   (((sz)+PGSIZE-1) & ~PGMASK) // (~PGMASK) set low 12 bit as 0
#define PGROUNDDOWN(a)  (((a)) & ~PGMASK)

void* new_page(size_t);

#endif
