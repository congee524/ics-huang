#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void context_kload(PCB *pcb, void *entry);
void context_uload(PCB *pcb, const char *filename);

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
  //naive_uload(NULL, "/bin/init");
  //context_kload(&pcb[0], (void *)hello_fun);
  //context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[0], "/bin/pal");
  switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
  // return the context of the following process
  current->cp = prev;
  /*
     Log("eflags 0x %x", current->cp->eflags);
     Log("cs 0x%x", current->cp->cs);
     Log("eip 0x%x", current->cp->eip);
     Log("err 0x%x", current->cp->err);
     Log("irq 0x%x", current->cp->irq);
     Log("eax 0x%x", current->cp->eax);
     Log("ebx 0x%x", current->cp->ebx);
     Log("ecx 0x%x", current->cp->ecx);
     Log("edx 0x%x", current->cp->edx);
     Log("edi 0x%x", current->cp->edi);
     Log("esi 0x%x", current->cp->esi);
     Log("ebp 0x%x", current->cp->ebp);

     Log("change!!!!\n");
     */
  //current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  current = &pcb[0];
  /*
     Log("eflags 0x %x", current->cp->eflags);
     Log("cs 0x%x", current->cp->cs);
     Log("eip 0x%x", current->cp->eip);
     Log("err 0x%x", current->cp->err);
     Log("irq 0x%x", current->cp->irq);
     Log("eax 0x%x", current->cp->eax);
     Log("ebx 0x%x", current->cp->ebx);
     Log("ecx 0x%x", current->cp->ecx);
     Log("edx 0x%x", current->cp->edx);
     Log("edi 0x%x", current->cp->edi);
     Log("esi 0x%x", current->cp->esi);
     Log("ebp 0x%x", current->cp->ebp);
     */
  return current->cp;
  // return NULL;
}
