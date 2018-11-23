#include <am.h>
#include <x86.h>
#include <klib.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();

_Context* irq_handle(_Context *tf) {
  _Context *next = tf;
  
  printf("tf 0x%x\n", tf);
  printf("tf->prot 0x %x", *(tf->prot));
  printf("tf->irq = %d\n", tf->irq);
  printf("tf->eax = 0x%x\n", tf->eax);
  printf("tf->ecx = 0x%x\n", tf->ecx);
  printf("tf->edx = 0x%x\n", tf->edx);
  printf("tf->ebx = 0x%x\n", tf->ebx);
  printf("tf->esp = 0x%x\n", tf->esp);
  printf("tf->ebp = 0x%x\n", tf->ebp);
  printf("tf->esi = 0x%x\n", tf->esi);
  printf("tf->edi = 0x%x\n", tf->edi);
  printf("tf->eip = 0x%x\n", tf->eip);
  if (user_handler) {
    _Event ev;
    //printf("1");
    switch (tf->irq) {
      case 0: ev.event = _EVENT_NULL; break;
      case 1: ev.event = _EVENT_ERROR; break;
      case 2: ev.event = _EVENT_IRQ_TIMER; break;
      case 3: ev.event = _EVENT_IRQ_IODEV; break;
      case 4: ev.event = _EVENT_PAGEFAULT; break;
      case 5: ev.event = _EVENT_YIELD; break;
      case 6: ev.event = _EVENT_SYSCALL; break;
      default: ev.event = _EVENT_ERROR; break;
    }
    //printf("2");
    next = user_handler(ev, tf);
    //printf("3");
    if (next == NULL) {
      next = tf;
    }
  }

  printf("tf 0x%x\n", tf);

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
  // 0x81 field function enter

  set_idt(idt, sizeof(idt)); // set the first address and lenght of idt

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
