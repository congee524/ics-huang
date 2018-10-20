#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  //TODO();
  rtl_and(&t2, &id_dest->val, &id_src->val);

  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(&t2, id_dest->width);
  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
  rtl_and(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(&t2, id_dest->width);
  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(&t2, id_dest->width);
  print_asm_template2(xor);
}

make_EHelper(or) {
  //TODO();
  rtl_or(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(&t2, id_dest->width);
  print_asm_template2(or);
}

make_EHelper(sar) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  while(id_src->val--) {
      id_dest->val >>= 1;
  }
  operand_write(id_dest, &id_dest->val);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  while(id_src->val--) {
      id_dest->val <<= 1;
  }
  operand_write(id_dest, &id_dest->val);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  while(id_src->val--) {
      id_dest->val >>= 1;
  }
  operand_write(id_dest, &id_dest->val);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;
  //printf("decoding.opcode 0x%x\n", decoding.opcode);
  rtl_setcc(&t2, cc);
  //printf("t2 is %u\n", t2);
  id_dest->val = (id_dest->val >> 8) << 8;
  rtl_or(&id_dest->val, &id_dest->val, &t2);
  operand_write(id_dest, &id_dest->val);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  //TODO();
  rtl_not(&id_dest->val, &id_dest->val);
  operand_write(id_dest, &id_dest->val);
  print_asm_template1(not);
}
