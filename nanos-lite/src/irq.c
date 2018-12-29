#include "common.h"

extern _Context* do_syscall(_Context *c);
extern _Context* schedule(_Context *c);

static _Context* do_event(_Event e, _Context* c) {
  _Context *ret = NULL;
  switch (e.event) {
    case _EVENT_YIELD:      //Log("into EVENT_YIELD"); 
                            ret = schedule(c);
                            break;
    case _EVENT_SYSCALL:    do_syscall(c); 
                            break;
    case _EVENT_IRQ_TIMER:  Log("TIME INTR!!");
                            _yield();
                            break;
    default:                panic("Unhandled event ID = %d", e.event);
  }

  return ret;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
