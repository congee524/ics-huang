#include "cpu/exec.h"
// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);

// control.c
make_EHelper(call);
make_EHelper(ret);

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

// arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
