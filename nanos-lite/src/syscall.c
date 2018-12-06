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
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
