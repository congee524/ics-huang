#ifndef __PROC_H__
#define __PROC_H__

#include "common.h"
#include "memory.h"

#define STACK_SIZE (8 * PGSIZE)

typedef size_t ssize_t;
typedef size_t off_t;
typedef union {
  uint8_t stack[STACK_SIZE] PG_ALIGN;
  struct {
    _Context *tf;
    _Protect as;
    uintptr_t cur_brk;
    // we do not free memory, so use `max_brk' to determine when to call _map()
    uintptr_t max_brk;
  };
} PCB;
extern PCB *current;

// loader.c
void naive_uload(PCB *pcb, const char *filename);

// ramdisk.c
size_t ramdisk_read(void *but, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

// fs.c
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
size_t fs_filesz(int fd);
#endif
