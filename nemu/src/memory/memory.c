#include "nemu.h"
#include "device/mmio.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

// Page directory and page table constants   from x86.h
#define PTXSHFT   12      // offset of PTX in a linear address
#define PDXSHFT   22      // offset of PDX in a linear address

#define PDX(va)     (((uint32_t)(va) >> PDXSHFT) & 0x3ff)
#define PTX(va)     (((uint32_t)(va) >> PTXSHFT) & 0x3ff)
#define OFF(va)     ((uint32_t)(va) & 0xfff) 

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((uint32_t)((d) << PDXSHFT | (t) << PTXSHFT | (o)))
#define PTE_ADDR(pte)   ((uint32_t)(pte) & ~0xfff)

uint8_t pmem[PMEM_SIZE];

paddr_t page_translate(vaddr_t addr) {
  //Log("addr: 0x%x", addr);
  //assert(0);
  PDE pde;
  PTE pte;
  paddr_t paddr;
  int off  = OFF(addr);
  int pgtx = PTX(addr);
  int pgdx = PDX(addr);
  
  paddr_t pgdir_addr = cpu.cr3.PDBR << 12;
  pde.val = paddr_read(pgdir_addr + (pgdx << 2), 4);
  if (pde.present == 0) {
    panic("addr 0x%x -- no such pde", addr);
  }
  
  paddr_t pgtable_addr = pde.page_frame << 12;
  pte.val = paddr_read(pgtable_addr + (pgtx << 2), 4);
  if (pte.present == 0) {
    panic("addr 0x%x, table addr 0x%x -- no such pte", addr, pde.page_frame);
  }
  
  paddr = (pte.page_frame << 12) + off;
  return paddr;
}

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  uint32_t map_NO = is_mmio(addr);
  if (map_NO == -1) {
    return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
  } else {
    return mmio_read(addr, len, map_NO) & (~0u >> ((4 - len) << 3));
  }
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
  uint32_t map_NO = is_mmio(addr);
  if (map_NO == -1) {
    memcpy(guest_to_host(addr), &data, len);
  } else {
    mmio_write(addr, len, data, map_NO);
  }
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  paddr_t paddr = addr;
  //Log("cpu.cr0 0x%x", cpu.cr0.val);
  if (cpu.cr0.pg == 1) {
    //assert(0);
    if (((addr & 0xfff) + len) > 0x1000) {
      panic("data cross the page boundaty");
    } else {
      paddr = page_translate(addr);
    }
  }
  return paddr_read(paddr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_t paddr = addr;
  //Log("cpu.cr0 0x%x", cpu.cr0.val);
  if (cpu.cr0.pg == 1) {
    //assert(0);
    if (((addr & 0xfff) + len) > 0x1000) {
      Log("data cross the page boundary");
      assert(0);
    } else {
      paddr = page_translate(addr);
    }
  }
  paddr_write(paddr, data, len);
}
