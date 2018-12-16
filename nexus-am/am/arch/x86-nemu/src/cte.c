#include <am.h>
#include <x86.h>
#include <klib.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();
void vecsys();

_Context* irq_handle(_Context *tf) {
  _Context *next = tf;
  //printf("tf->irq = 0x%x\n", tf->irq);
  
  if (user_handler) {
    _Event ev = {0};
    switch (tf->irq) {
      case 0x80:  ev.event = _EVENT_SYSCALL;  break;
      case 0x81:  ev.event = _EVENT_YIELD;    break;
      default:    ev.event = _EVENT_ERROR;    break;
    }
    next = user_handler(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }

  return next;
}

static GateDesc idt[NR_IRQ]; // NR_IRQ = 256

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_KERN);
  // 0x81 field function enter

  set_idt(idt, sizeof(idt)); // set the first address and lenght of idt

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  // printf("entry is 0x%x\n", entry);
  _Context *c = (_Context*)stack.end - 1;
  memset(c, 0, sizeof(_Context));
  c->eip = (uintptr_t)entry;
  c->cs = 0x8;
  c->eflags = 0x2;
  //c->ebp = (int)stack.end;
  return c;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
