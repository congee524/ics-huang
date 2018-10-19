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

// control.c
make_EHelper(call);// maybe exist effect on eflags
make_EHelper(ret);
make_EHelper(jcc);

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
