#include "memory.h"
#include "proc.h"

static void *pf = NULL;
extern PCB *current;

void* new_page(size_t nr_page) {
  void *p = pf;
  // Log("pre %p", pf);
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
  Log("mm_brk");
  current->cur_brk = new_brk;
  if (new_brk > current->max_brk) {
    int nr_page = current->max_brk / PGSIZE + 1;
    void *va = (void *)(nr_page * PGSIZE);
    int diff_size = new_brk - (int)va - 1;
    void *pa = NULL;
    while (diff_size > 0) {
      pa = new_page(1);
      _map(&(current->as), va, pa, 1);
      diff_size -= PGSIZE;
      va += PGSIZE;
    }
    current->max_brk = new_brk;
  }
  return 0;
}

void init_mm() {
  // the inital position of page at the start of heap
  // return the physical address
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
