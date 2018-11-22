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
  Log("1");
  // push CS
  rtl_push(&cpu.cs);
  Log("2");
  // push eip
  rtl_push(&ret_addr);
  Log("3");
  cpu.eflags.IF = cpu.eflags.TF = 0;
  Log("4");
  union {
    GateDesc gate;
    uint32_t val[2];
  } GD;
  Log("5");
  GD.val[0] = vaddr_read(cpu.IDTR.base + NO, 4);
  Log("6");
  GD.val[1] = vaddr_read(cpu.IDTR.base + NO + 4, 4);
  Log("15_0 = 0x%x\n31_16 = 0x%x\neip = 0x%x", GD.gate.offset_15_0, GD.gate.offset_31_16, cpu.eip);
  cpu.eip = GD.gate.offset_15_0 + (GD.gate.offset_31_16 << 16);
  Log("after eip = 0x%x", cpu.eip);
  assert(0);
}

void dev_raise_intr() {
}
