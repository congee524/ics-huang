#include "cpu/exec.h"
// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);

// control.c
make_EHelper(call);
make_EHelper(ret);

// prefix.c
make_EHelper(operand_size);

// special.c
make_EHelper(inv);
make_EHelper(nemu_trap);

// logic.c
make_EHelper(xor);

// arith.c
make_EHelper(sub);
