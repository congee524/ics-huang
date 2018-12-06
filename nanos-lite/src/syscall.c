#include "common.h"
#include "syscall.h"

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
    case SYS_exit: _halt(a[0]); break;
    case SYS_yield: _yield(); break;
    case SYS_open: assert(0); break;
    case SYS_read: assert(0); break;
    case SYS_write: assert(0); break;
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
