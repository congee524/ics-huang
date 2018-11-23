#ifndef __ARCH_H__
#define __ARCH_H__

#include <am.h>

#define PMEM_SIZE (128 * 1024 * 1024)
#define PGSIZE    4096    // Bytes mapped by a page

typedef uint32_t size_t;

struct _Context {
  // original:
  // uintptr_t esi, ebx, eax, eip, edx, err, eflags, ecx, cs, esp, edi, ebp;

  /*  
  uintptr_t eflags, cs, eip;
  int irq;
  uintptr_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
  struct _Protect *prot;
  */

   
  struct _Protect *prot;
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  int irq;
  uintptr_t eip, cs, eflags;
  
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
