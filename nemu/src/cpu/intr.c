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
  //cpu.eflags.IF = cpu.eflags.TF = 0;

  union {
    GateDesc gate;
    uint32_t val[2];
  } GD;

  //Log("0x%x", cpu.IDTR.base);
  //Log("%d", NO);

  GD.val[0] = vaddr_read(cpu.IDTR.base + NO * 8, 4);
  GD.val[1] = vaddr_read(cpu.IDTR.base + NO * 8 + 4, 4);
  
  //Log("15_0 = 0x%x", GD.gate.offset_15_0);
  //Log("31_16 = 0x%x", GD.gate.offset_31_16);
  //Log("eip = 0x%x", cpu.eip);

  decoding.seq_eip = GD.gate.offset_15_0 + (GD.gate.offset_31_16 << 16);
  //cpu.eip = GD.gate.offset_15_0;

  //Log("after eip = 0x%x", cpu.eip);
  //assert(0);
}

void dev_raise_intr() {
}
