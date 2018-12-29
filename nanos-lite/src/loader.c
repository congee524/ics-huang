#include "proc.h"

#define DEFAULT_ENTRY 0x8048000

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO(); PA3.2: ramdisk_read((void*)DEFAULT_ENTRY, 0, get_ramdisk_size());
  int fd = fs_open(filename, 0, 0);
  //fs_read(fd, (void*)DEFAULT_ENTRY, fs_filesz(fd));
  uint32_t fsize = fs_filesz(fd);

  // int _map(_Protect *p, void *va, void *pa, int prot) {
  // void* new_page(size_t nr_page) 
  // pgalloc_usr ???
  void *va = (void *)DEFAULT_ENTRY;
  void *pa = NULL;
  while(fsize > 0) {
    // nr_page indicate the num of allocated free pages
    // but if there exist no such many pages, maybe wrong?
    // so i allocate one page each time
    pa = new_page(1);
    _map(&(pcb->as), va, pa, 1);
    fs_read(fd, pa, PGSIZE);
    fsize -= PGSIZE;
    va += PGSIZE;
  }
  fs_close(fd);
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  _protect(&(pcb->as));
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
