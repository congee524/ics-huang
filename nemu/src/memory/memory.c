#include "nemu.h"
#include "device/mmio.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

paddr_t page_translate(vaddr_t addr) {
  Log("addr: 0x%x", addr);
  assert(0);
  return 0;
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
  if (cpu.cr0.pg == 1) {
    assert(0);
    if (((addr & 0xfff) + len) > 0x1000) {
      Log("data cross the page boundaty");
      assert(0);
    } else {
      paddr = page_translate(addr);
    }
  }
  return paddr_read(paddr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_t paddr = addr;
  if (cpu.cr0.pg == 1) {
    assert(0);
    if (((addr & 0xfff) + len) > 0x1000) {
      Log("data cross the page boundary");
      assert(0);
    } else {
      paddr = page_translate(addr);
    }
  }
  paddr_write(paddr, data, len);
}
