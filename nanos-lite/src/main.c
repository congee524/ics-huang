#include "common.h"

void init_mm(void);
void init_ramdisk(void);
void init_device(void);
void init_irq(void);
void init_fs(void);
void init_proc(void);

int main() {
#ifdef HAS_VME
  init_mm();
#endif

  Log("'Hello World!' from Nanos-lite");
  Log("Build time: %s, %s", __TIME__, __DATE__);

  init_ramdisk();

  init_device();

#ifdef HAS_CTE
  init_irq();
#endif

  init_fs();
  
  init_proc();

#ifdef HAS_CTE
  _yield();
#endif

  panic("Should not reach here");
}
