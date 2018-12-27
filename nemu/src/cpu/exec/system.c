#include "cpu/exec.h"
#include "device/port-io.h"

void difftest_skip_ref();
void difftest_skip_dut();

void raise_intr();

make_EHelper(lidt) {
  //TODO();
  assert(id_dest->width == 2 || id_dest->width == 4);
  cpu.IDTR.limit = vaddr_read(id_dest->addr, 2);
  if (id_dest->width == 2) {
    cpu.IDTR.base = vaddr_read(id_dest->addr + 2, 4) & 0x00ffffff;
    // about this, i think if we need 16 bit address, then we need data[1];
    assert(0);
  } else if (id_dest->width == 4) {
    cpu.IDTR.base = vaddr_read(id_dest->addr + 2, 4);
  }
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  //TODO();
  switch(id_dest->reg) {
    case 0:
      cpu.cr0.val = id_src->val;
      break;
    case 3:
      cpu.cr3.val = id_src->val;
      break;
    default:
      Log("wrong reg type %d in mov_r2cr", id_dest->reg);
  }
  printf("cpu.cr0: 0x%x\ncpu.cr3: 0x%x\n", cpu.cr0.val, cpu.cr3.val);
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  //TODO();
  switch(id_src->reg) {
    case 0:
      operand_write(id_dest, &cpu.cr0.val);
      break;
    case 3:
      operand_write(id_dest, &cpu.cr3.val);
      break;
    default:
      Log("wrong reg type %d in mov_cr2r", id_src->reg);
  }
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  //TODO();
  raise_intr(id_dest->val, decoding.seq_eip);
  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  //TODO();
  //rtl_pop(&decoding.jmp_eip);
  rtl_pop(&decoding.seq_eip);
  rtl_pop(&cpu.cs);
  rtl_pop(&cpu.eflags.val);
  //rtl_j(decoding.jmp_eip);
  print_asm("iret");
}

make_EHelper(in) {
  //TODO();
  switch(id_dest->width) {
    case 4: id_dest->val = pio_read_l(id_src->val); break;
    case 2: id_dest->val = pio_read_w(id_src->val); break;
    case 1: id_dest->val = pio_read_b(id_src->val); break;
  }
  operand_write(id_dest, &id_dest->val);
  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  //TODO();
  switch(id_dest->width) {
    case 4: pio_write_l(id_dest->val, id_src->val); break;
    case 2: pio_write_w(id_dest->val, id_src->val); break;
    case 1: pio_write_b(id_dest->val, id_src->val); break;
  }
  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
