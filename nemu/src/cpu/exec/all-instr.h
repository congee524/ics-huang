#include "cpu/exec.h"
// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);

// control.c
make_EHelper(call);
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

// arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
make_EHelper(cmp);
