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
  switch (id_dest->width) {
    case 1: 
      id_dest->val = (int8_t)id_dest->val;
      break;
    case 2:
      id_dest->val = (int16_t)id_dest->val;
      break;
    case 4:
      id_dest->val = (int32_t)id_dest->val;
      break;
    default:
      assert(0);
  }
  rtl_sar(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(rol) {
  //TODO();
  //i didn't find it, may it be left out
  // ohhhhhhhhhhh, i forget the width
  t2 = 31;
  switch (id_dest->width) {
      case 1: 
        t2 = 7;
      case 2:
        t2 = 15;
      case 4:
        t2 = 31;
  }
  at = 1;
  t0 = id_dest->val;
  while (id_src->val--) {
    t1 = (t0 >> t2) & 1;
    rtl_shl(&t0, &t0, &at);
    rtl_or(&t0, &t0, &t1);
  }
  cpu.eflags.CF = t1;
  if((t0 & 1) != t1) {
      cpu.eflags.OF = 1;
  } else {
      cpu.eflags.OF = 0;
  }
  switch (id_dest->width) {
    case 1:
      id_dest->val = (t0 & 0xff) | ((id_dest->val >> 8) << 8);
    case 2:
      id_dest->val = (t0 & 0xffff) | ((id_dest->val >> 16) << 16);
    case 4:
      id_dest->val = t0;
    default:
      id_dest->val = t0;
  }
  operand_write(id_dest, &id_dest->val);
  
  print_asm_template2(rol);
}

make_EHelper(setcc) {
  //printf("before ecx is 0x%x\nid_dest->val is 0x%x\n", cpu.ecx, id_dest->val);
  uint32_t cc = decoding.opcode & 0xf;
  //printf("decoding.opcode 0x%x\n", decoding.opcode);
  rtl_setcc(&t2, cc);
  //printf("t2 is %u\n", t2);
  //rtl_mv(&at, &reg_l(id_dest->reg));
  //printf("after ecx is 0x%x\nid_dest->val is 0x%x\n", cpu.ecx, id_dest->val);
  //id_dest->val = (id_dest->val >> 8) << 8;
  //rtl_or(&id_dest->val, &id_dest->val, &t2);
  operand_write(id_dest, &t2);
  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  //TODO();
  rtl_not(&id_dest->val, &id_dest->val);
  operand_write(id_dest, &id_dest->val);
  print_asm_template1(not);
}
