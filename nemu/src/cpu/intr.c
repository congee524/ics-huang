#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //TODO();

  printf("NO is 0x%x, ret_addr is 0x%x\n", NO, ret_addr);
  // push eflags
  rtl_push(&cpu.eflags.val);

  // push CS
  cpu.esp -= 2; // because CS is 16 bit, i cannot use rtl_push
  vaddr_write(cpu.esp, cpu.CS, 2);

  // push eip
  rtl_push(&cpu.eip);

  cpu.eflags.IF = cpu.eflags.TF = 0;

  assert(0);
}

void dev_raise_intr() {
}
