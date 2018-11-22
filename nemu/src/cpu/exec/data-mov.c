#include "cpu/exec.h"
#include "cpu/rtl.h"

make_EHelper(mov) {
  //printf("mov: id_src->val 0x%08x\n", id_src->val);
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val); 
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  operand_write(id_dest, &id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  //TODO();
  t2 = vaddr_read(cpu.esp, 4);
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&t2);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);
  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  rtl_mv(&reg_l(4), &reg_l(5));
  rtl_pop(&t0);
  rtl_mv(&reg_l(5), &t0);
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    //TODO();
      if ((cpu.eax >> 15) & 1) {
          cpu.edx = 0xFFFF;
      } else {
          cpu.edx = 0;
      }
  }
  else {
    //TODO();
      if ((cpu.eax >> 31) & 1) {
          cpu.edx = 0xFFFFFFFF;
      } else {
          cpu.edx = 0;
      }
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    rtl_sext(&at, &reg_l(0), 2);
    //to ensure that the high bit keep still
    cpu.eax = (at & 0xffff) | cpu.eax;
  }
  else {
    rtl_sext(&reg_l(0), &reg_l(0), 4);
  }
  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  //printf("dest width is %d, src width is %d\n", id_dest->width, id_src->width);
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
