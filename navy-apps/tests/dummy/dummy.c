#include <stdint.h>

#define SYS_yield 1
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  //return _syscall_(SYS_yield, 0, 0, 0);
  return _syscall_(SYS_yield, 2, 0, 0);
}
