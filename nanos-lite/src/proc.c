#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

int fg_pcb = 1;
int time_pcb = 0;

void context_kload(PCB *pcb, void *entry);
void context_uload(PCB *pcb, const char *filename);
size_t events_read(void *buf, size_t offset, size_t len);

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
  context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[1], "/bin/events");
  //context_uload(&pcb[1], "/bin/pal");
  context_uload(&pcb[2], "/bin/pal");
  context_uload(&pcb[3], "/bin/pal");
  switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
  // return the context of the following process
  current->cp = prev;
 /* 
  char buf[128];
  events_read(buf, 0, 128);
  */
  /*
  if (strcmp(buf, "kd F1\n") == 0) {
    fg_pcb = 1;
  } else if (strcmp(buf, "kd F2\n") == 0) {
    fg_pcb = 2;
  } else if (strcmp(buf, "kd F3\n") == 0) {
    fg_pcb = 3;
  }
  */ 

  if (time_pcb > 200) {
    current = &pcb[0];
    time_pcb = 0;
  } else {
    current = &pcb[fg_pcb];
    time_pcb++;
  }
  
  //current = (current == &pcb[0] ? &pcb[fg_pcb] : &pcb[0]);
  //current = &pcb[fg_pcb];
  return current->cp;
  // return NULL;
}
