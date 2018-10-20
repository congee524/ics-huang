#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = decoding.opcode & 0xf;
  //printf("j 0x%x cc %u\n", decoding.opcode, cc);
  //printf("j ZF is %u SF is %u OF is %u\n", cpu.eflags.ZF, cpu.eflags. SF, cpu.eflags.OF);
  rtl_setcc(&t0, cc);
  //printf("ori t0 is %u\n", t0);
  rtl_li(&t1, 0);
  //printf("t0 is %u\n", t0);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);

  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  rtl_push(&decoding.seq_eip);
  rtl_j(decoding.jmp_eip);
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  // rtl_pop(&decoding.seq_eip);
  // some strange thing happened! if i use rtl_pop(&decoding.seq_eip)
  // in the nemu, the "ret" will be printf incorrectly.
  // but when i use the following two lines code, the "ret" will be printf correctly!
  rtl_pop(&decoding.jmp_eip);
  rtl_j(decoding.jmp_eip);
  print_asm("ret");
}

make_EHelper(call_rm) {
  //TODO();
  //printf("id_dest val is 0x%x seq.eip is 0x%x\n", id_dest->val, decoding.seq_eip);
  rtl_push(&decoding.seq_eip);
  rtl_j(id_dest->val);
  print_asm("call *%s", id_dest->str);
}
