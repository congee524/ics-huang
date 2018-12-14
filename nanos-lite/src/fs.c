#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_DISPINFO, FD_EVENTS};

size_t serial_write(const void *buf, size_t offset, size_t len);

size_t dispinfo_read(void *buf, size_t offset, size_t len);

size_t fb_write(const void *buf, size_t offset, size_t len);

size_t events_read(void *buf, size_t offset, size_t len);

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
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"/proc/dispinfo", 128, 0, 0, dispinfo_read, invalid_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
  {"/dev/tty", 0, 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_width() * screen_height() * 4;
}

int fs_open(const char *pathname, int flags, int mode){
  // we ignore flags and mode
  // return open(pathname, flags, mode);
  // Log("%s", pathname);
  for (int i = 0; i < NR_FILES; i++) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("no such file %s!", pathname);
  return 0;
}

ssize_t fs_read(int fd, void *buf, size_t len){
  Finfo fo = file_table[fd];
  if(fo.open_offset + len > fo.size){
      len = fo.size - fo.open_offset;
  }
  int ret = 0;
  if(file_table[fd].read == NULL){
    ret = ramdisk_read(buf, fo.disk_offset + fo.open_offset, len);
  } else {
    // events_read | dispinfo_read 
    // events_read return strlen(buf) instead of len
    // so we cannot use open_offset += len
    ret = file_table[fd].read(buf, fo.open_offset, len);
  }
  file_table[fd].open_offset += ret;
  return ret;
}

ssize_t fs_write(int fd, const void *buf, size_t len){
  Finfo fo = file_table[fd];
   if(file_table[fd].write == NULL) {
    if(fo.open_offset + len >= fo.size){
      len = fo.size - fo.open_offset;
    }
    file_table[fd].open_offset += len;
    return ramdisk_write(buf, fo.disk_offset + fo.open_offset, len);
  } else {
    // serial_write | fb_write
    file_table[fd].open_offset += len;
    return file_table[fd].write(buf, fo.disk_offset + fo.open_offset, len);
  }
}

off_t fs_lseek(int fd, off_t offset, int whence) {
  // Log("fd is %d", fd);
  off_t tmp;
  Finfo fo = file_table[fd];
  switch (whence) {
    case SEEK_SET:  tmp = offset; 
                    break;
    case SEEK_CUR:  tmp = fo.open_offset + offset; 
                    break;
    case SEEK_END:  tmp = fo.size + offset;
                    break;
    default:        Log("wrong whence type!");
                    return -1;
  }
  if(tmp > fo.size){
    file_table[fd].open_offset = fo.size;
  } else {
    file_table[fd].open_offset = tmp;
  }
  return file_table[fd].open_offset;
}

int fs_close(int fd) {
  return 0;
}

size_t fs_filesz(int fd){
  return file_table[fd].size;
}
