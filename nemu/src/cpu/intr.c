#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //TODO();

  //printf("NO is 0x%x, ret_addr is 0x%x\n", NO, ret_addr);
  // push eflags
  rtl_push(&cpu.eflags.val);

  // push CS
  rtl_push(&cpu.cs);

  // push eip
  rtl_push(&ret_addr);

  cpu.eflags.IF = cpu.eflags.TF = 0;

  
  assert(0);
}

void dev_raise_intr() {
}
