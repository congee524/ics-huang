#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset; // FINISHED as nju-ics.gitbook.io reuqires
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_DISPINFO};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}


size_t std_write(const void* buf,size_t offset,size_t len){
	char* ch = (char*) buf;
	for (int i=0;i<len;i++) _putc(ch[i]);
	return len;
}

/* This is the information about all files in disk. */

#define DISPINFO_SIZE 128
char dispinfo_buf[DISPINFO_SIZE];
extern size_t dispinfo_read(void*,size_t,size_t);

#define DEFAULT_DEVFB_SIZE 0*0
extern size_t fb_write(const void*,size_t,size_t);

extern size_t events_read(void*,size_t,size_t);

static Finfo file_table[] __attribute__((used)) = {
  {"stdin", -1, 0, invalid_read, invalid_write}, // SIZE = -1 (i.e., maxlongint) 
  {"stdout", -1, 0, invalid_read, std_write},
  {"stderr", -1, 0, invalid_read, std_write},
  {"/dev/fb", DEFAULT_DEVFB_SIZE, 0, invalid_read, fb_write},
  {"/proc/dispinfo", DISPINFO_SIZE, 0, dispinfo_read, invalid_write},
  {"/dev/events", -1, 0, events_read, invalid_write},
  {"/dev/tty", -1, 0, invalid_read, std_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

extern int screen_width();
extern int screen_height();

void init_fs() {
  // TODO: initialize the size of /dev/fb

  file_table[FD_FB].size = screen_height()*screen_width()*sizeof(uint32_t);

  //FINISHED
}


int fs_open(const char *pathname,int flags,int mode){
    for (int i=0;i<NR_FILES;i++)
        if (strcmp(pathname,file_table[i].name)==0){
		file_table[i].open_offset = 0;
		return i;
	}
    panic("No such file: %s",pathname);
    return 0;
}

int fs_close(int fd){
  return 0;
}

extern size_t ramdisk_read(void*,size_t,size_t);
extern size_t ramdisk_write(const void*,size_t,size_t);

size_t fs_read(int fd, void* buf, size_t len){
  ReadFn file_read = file_table[fd].read;
  if (file_read == NULL)
	file_read = ramdisk_read;  

  if (len==0) return 0;
  if (file_table[fd].open_offset >= file_table[fd].size)
	  return -1; //error
  
  size_t real_len = file_table[fd].size-file_table[fd].open_offset;
  if(real_len>len)
    real_len = len;

  size_t ret = file_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,real_len);

  file_table[fd].open_offset += ret;
  
  //printf("read success len=%d\n",real_len);
  return ret;
}

size_t fs_filesz(int fd){
	return file_table[fd].size;
}


extern void _putc(char);

size_t fs_write(int fd, const void* buf, size_t len){
  WriteFn file_write = file_table[fd].write;
  if (file_write==NULL)
	  file_write = ramdisk_write;
	
  if (len==0) return 0;
  if (file_table[fd].open_offset >= file_table[fd].size)
	  return -1; //error
  
  size_t real_len = file_table[fd].size-file_table[fd].open_offset;
  if(real_len>len)
    real_len = len;

  size_t ret = file_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,real_len);

  file_table[fd].open_offset += ret;

  return ret;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  size_t newoff;

  switch(whence){
      case SEEK_SET: newoff = offset; break;
      case SEEK_CUR: newoff = file_table[fd].open_offset + offset; break;
      case SEEK_END: newoff = file_table[fd].size + offset; break;
      default: return -1;
  }

  if (newoff>file_table[fd].size)
	  return -1;
  file_table[fd].open_offset = newoff;
  return newoff;
}

