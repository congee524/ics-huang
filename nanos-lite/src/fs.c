#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  ReadFn read;
  WriteFn write;
  off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

// we ignore flags and mode
int fs_open(const char *pathname, int flags, int mode){
  //return open(pathname, flags, mode);
  Log("fs_open %s", pathname);
  for (int i = 0; i < NR_FILES; i++) {
    Log("pre file name: %s", file_table[i].name);
    if (strcmp(pathname, file_table[i].name) == 0) {
      file_table[i].open_offset = 0;
      return i;
    }
  }
  Log("file doesn't exist!!!!!!!");
  assert(0);
  return 0;
}

ssize_t fs_read(int fd, void *buf, size_t len){
  Finfo fo = file_table[fd];
  if(fo.open_offset >= fo.size){
    printf("out of file_size!\n");
    return 0;
  }
  if(fo.open_offset + len >= fo.size){
    file_table[fd].open_offset = fo.size;
    return ramdisk_read(buf, fo.disk_offset+fo.open_offset, fo.size-fo.open_offset);
  } else {
    file_table[fd].open_offset += len;
    return ramdisk_read(buf, fo.disk_offset + fo.open_offset, len);
  }
}

// ssize_t fs_write(int fd, const void *buf, size_t len);

off_t fs_lseek(int fd, off_t offset, int whence) {
  off_t tmp;
  Finfo fo = file_table[fd];
  switch (whence) {
    case SEEK_SET: tmp = offset; 
                   break;
    case SEEK_CUR: tmp = fo.open_offset + offset; 
                   break;
    case SEEK_END: tmp = fo.size + offset;
                   break;
    default:       printf("wrong whence type!\n");
                   return -1;
  }
  if(tmp >= 0 && tmp < fo.size){
    file_table[fd].open_offset = tmp;
    return tmp;
  } else {
    printf("out of file bound!\n");
    return -1;
  }
}

int fs_close(int fd) {
  printf("%s closed!\n", file_table[fd].name);
  return 0;
}

size_t fs_filesz(int fd){
  return file_table[fd].size;
}
