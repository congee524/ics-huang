#include "cpu/exec.h"
// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);
make_EHelper(pusha);
make_EHelper(cltd);
make_EHelper(movsx);
make_EHelper(cwtl);

// control.c
make_EHelper(call);// maybe exist effect on eflags
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(call_rm);
make_EHelper(jmp_rm);

// prefix.c
make_EHelper(operand_size);

// special.c
make_EHelper(inv);
make_EHelper(nemu_trap);
make_EHelper(nop);

// logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(or);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(not); 
make_EHelper(shr);
make_EHelper(rol);

// arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
make_EHelper(cmp);
make_EHelper(dec);
make_EHelper(inc);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(imul3);
make_EHelper(idiv);
make_EHelper(sbb);
make_EHelper(div);
make_EHelper(mul);
make_EHelper(neg);

// system.c
make_EHelper(in);
make_EHelper(out);
