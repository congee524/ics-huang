#include "common.h"
#include "syscall.h"
#include "proc.h"

int sys_yield();
int sys_write(int fd, void *buf, size_t count);
int sys_brk(intptr_t newbrk);
void naive_uload(PCB *pcb, const char *filename);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    // SPEC.md say code should be 0, since return value is put in eax, we may use a[1]
    // _yield() return nothing, therefore, complete a funciton sys_yield
    case SYS_exit:          //naive_uload(NULL, "/bin/init");
                            _halt(a[1]); 
                            break;
    case SYS_yield:         c->GPRx=sys_yield(); 
                            break;
    case SYS_brk:           c->GPRx=sys_brk((intptr_t)a[1]);
                            break;
    case SYS_write:         c->GPRx=fs_write((int)a[1],(const void *)a[2],(size_t)a[3]);
                            break;
    case SYS_open:          c->GPRx=fs_open((const char *)a[1],(int)a[2],(int)a[3]);
                            break;
    case SYS_read:          c->GPRx=fs_read((int)a[1],(void *)a[2],(size_t)a[3]);
                            break;
    case SYS_close:         c->GPRx=fs_close((int)a[1]);
                            break;
    case SYS_lseek:         c->GPRx=fs_lseek((int)a[1],(off_t)a[2],(int)a[3]);
                            break;
    case SYS_execve:        naive_uload(NULL, (char *)a[1]);
                            c->GPRx=0;
                            break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  return c;
}

int sys_yield() {
  _yield();
  return 0;
}

int sys_brk(intptr_t newbrk){
  _heap.end = (void *)newbrk;
  return 0;
}

/*
   int sys_write(int fd, void *buf, size_t count){
   if (fd == 1 || fd == 2) {
   for (int i = 0; i < count; i++) {
   _putc(((char*)buf)[i]);
   }
   } else {
   printf("fd: %d\n", fd);
   assert(0);
   }
   return count;
   }
   */


