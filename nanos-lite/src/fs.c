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

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_DISPINFO};

size_t serial_write(const void *buf, size_t offset, size_t len);

size_t dispinfo_read(void *buf, size_t offset, size_t len);

size_t fb_write(const void *buf, size_t offset, size_t len);

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
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
  {"/deb/fb", 0, 0, invalid_read, fb_write},
  {"/proc/dispinfo", 128, 0, dispinfo_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  //Log("tmp: %d", screen_width());
  //assert(0);
  Log("w %d h %d", screen_width(), screen_height());
  file_table[FD_FB].size = screen_width() * screen_height() * 4;
  Log("size 0x%x", file_table[FD_FB].size);
}

// we ignore flags and mode
int fs_open(const char *pathname, int flags, int mode){
  //return open(pathname, flags, mode);
  Log("fs_open %s", pathname);
  Log("nr_files %d", NR_FILES);
  for (int i = 0; i < NR_FILES; i++) {
    Log("pre file name: %s", file_table[i].name);
    if (strcmp(pathname, file_table[i].name) == 0) {
      Log("%d", i);
      file_table[i].open_offset = 0;
      return i;
    }
  }
  Log("file doesn't exist!!!!!!!");
  assert(0);
  return 0;
}

ssize_t fs_read(int fd, void *buf, size_t len){
  // Log("fd is %d", fd);
  Finfo fo = file_table[fd];
  if(file_table[fd].read == NULL){
    // Log("0x%x 0x%x %d", fo.open_offset, fo.size, len);
    //if(fo.open_offset >= fo.size){
    //  printf("out of file_size!\n");
    //  return 0;
    //}
    Log("%d", fd);
    if(fo.open_offset + len > fo.size){
      len = fo.size - fo.open_offset;
    }
    file_table[fd].open_offset += len;
    return ramdisk_read(buf, fo.disk_offset + fo.open_offset, len);
  } else {
    //return file_table[fd].read(buf, fo.disk_offset + fo.open_offset, len);
    Log("read: %d", fd);
    file_table[fd].open_offset += len;
    return file_table[fd].read(buf, fo.open_offset, len);
  }

}

ssize_t fs_write(int fd, const void *buf, size_t len){
  // Log("fd is %d", fd);
  Finfo fo = file_table[fd];
  if(file_table[fd].write == NULL) {
    // Log("0x%x 0x%x %d", fo.open_offset, fo.size, len);
    if (fo.open_offset > fo.size){
      printf("out of file bound!\n");
      return 0;
    }
    if(fo.open_offset + len >= fo.size){
      len = fo.size - fo.open_offset;
    }
    file_table[fd].open_offset += len;
    return ramdisk_write(buf, fo.disk_offset + fo.open_offset, len);
  } else {
    return file_table[fd].write(buf, fo.disk_offset + fo.open_offset, len);
  }
}

off_t fs_lseek(int fd, off_t offset, int whence) {
  // Log("fd is %d", fd);
  off_t tmp;
  Finfo fo = file_table[fd];
  switch (whence) {
    case SEEK_SET: // Log("SEEK_SET");
      tmp = offset; 
      break;
    case SEEK_CUR: // Log("SEEK_CUR");
      tmp = fo.open_offset + offset; 
      break;
    case SEEK_END: // Log("SEEK_END");
      tmp = fo.size + offset;
      break;
    default:       printf("wrong whence type!\n");
                   return -1;
  }
  // Log("lseek 0x%x 0x%x 0x%x", tmp, fo.size, offset);
  if(tmp > fo.size){
    file_table[fd].open_offset = fo.size;
    return fo.size;
  } else {
    file_table[fd].open_offset = tmp;
    return tmp;
  }
}

int fs_close(int fd) {
  // Log("%d %s closed!\n", fd, file_table[fd].name);
  return 0;
}

size_t fs_filesz(int fd){
  return file_table[fd].size;
}
