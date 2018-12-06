#include "common.h"
#include "syscall.h"

int sys_yield();
int sys_write(int fd, void *buf, size_t count);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  printf("a[0]: %p\n", a[0]);
  printf("a[1]: %p\n", a[1]);
  printf("a[2]: %p\n", a[2]);
  printf("a[3]: %p\n", a[3]);

  switch (a[0]) {
    // SPEC.md say code should be 0, since return value is put in eax, we may use a[1]
    // _yield() return nothing, therefore, complete a funciton sys_yield
    case SYS_exit: 
      _halt(a[1]); 
      break;
    case SYS_yield:{ 
                     c->GPRx = sys_yield(); 
                     break;
                   }
    case SYS_open: assert(0); break;
    case SYS_read: assert(0); break;
    case SYS_write: {
                      assert(0);
                      //c->GPRx = sys_write((int)a[1], (void *)a[2], (size_t)a[3]);
                      break;
                    }
    case SYS_kill: assert(0); break;
    case SYS_getpid: assert(0); break;
    case SYS_close: assert(0); break;
    case SYS_lseek: assert(0); break;
    case SYS_brk: assert(0); break;
    case SYS_fstat: assert(0); break;
    case SYS_time: assert(0); break;
    case SYS_signal: assert(0); break;
    case SYS_execve: assert(0); break;
    case SYS_fork: assert(0); break;
    case SYS_link: assert(0); break;
    case SYS_unlink: assert(0); break;
    case SYS_wait: assert(0); break;
    case SYS_times: assert(0); break;
    case SYS_gettimeofday: assert(0); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return c;
}

int sys_yield() {
  _yield();
  return 0;
}

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
