#include <x86.h>
#include "klib.h"

#define PG_ALIGN __attribute((aligned(PGSIZE)))

typedef uint32_t paddr_t;

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  // printf("1\n");
  pgalloc_usr = pgalloc_f;
  // printf("2\n");
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);

  return 0;
}

int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}

void _unprotect(_Protect *p) {
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
  set_cr3(c->prot->ptr);
  cur_as = c->prot;
}

int _map(_Protect *p, void *va, void *pa, int mode) {
  PDE pde_val;
  // PTE pte_val;
  uint32_t pgdx = PDX(va);
  uint32_t pgtx = PTX(va);
  // uint32_t off  = OFF(va);

  // base addr(p->addr) | pagedirectory index
  paddr_t pde_addr = PTE_ADDR(p->ptr) | (pgdx << 2);
  pde_val = *((PDE *)pde_addr);
  if ((pde_val & PTE_P) == 0) {
    // if the address is not present, allocate a new page
    paddr_t pte_base_addr = (paddr_t)pgalloc_usr(1);
    pde_val = pte_base_addr | PTE_P;
    *((PDE *)pde_addr) = pde_val;
  }

  if ((mode & PTE_P) == 0) {
    // if prot.present == 0, the mapping is valid
    assert(0);
    return 0;
  }

  paddr_t pte_addr = PTE_ADDR(pde_val) | (pgtx << 2);
  *((PTE *)pte_addr) = PTE_ADDR((paddr_t)pa) | PTE_P;
  return 0;
}

_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
  // native/src/vme.c
  // main(argc, argv, env)  three parameters, the address of pointer
  void *stack_args = ustack.end - 3 * sizeof(void *);
  memset(stack_args, 0, 3 * sizeof(void *));
  // ustack.end -= 1 * sizeof(uintptr_t);
  // uintptr_t *ret = stack_args - sizeof(void *);
  // *(uintptr_t *)ret = 0;

  _Context *c = (_Context*)stack_args - 1;
  memset(c, 0, sizeof(_Context));
  c->cs = 0x8;
  c->eflags = 0x2;
  c->eip = (uintptr_t)entry;
  c->esp = c->ebp = (uintptr_t)ustack.end;
  // c->irq = 0x81;
  c->prot = p;

  /*
     c->uc.uc_mcontext.gregs[REG_EDI] = 0;
     c->uc.uc_mcontext.gregs[REG_ESI] = ret; // ???
     c->uc.uc_mcontext.gregs[REG_EDX] = ret; // ??? native use the same value
     */
  return c;
  // return NULL;
}
