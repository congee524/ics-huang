#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void context_kload(PCB *pcb, void *entry);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
  naive_uload(NULL, "/bin/init");
  //context_kload(&pcb[0], (void *)hello_fun);
  switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
  // return the context of the following process
  Log("schedule");
  current->cp = prev;
  Log("before, eip 0x%x", current->cp->eip);
  Log("before, cs 0x%x", current->cp->cs);
  Log("before, 0x%x", current->cp->eip);
  Log("before, 0x%x", current->cp->eip);
  current = &pcb[0];
  Log("after, eip 0x%x", current->cp->eip);
  Log("after, cs 0x%x", current->cp->cs);
  Log("after, 0x%x", current->cp->eip);
  Log("after, 0x%x", current->cp->eip);
  Log("after, 0x%x", current->cp->eip);
  return current->cp;
  // return NULL;
}
