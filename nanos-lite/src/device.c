#include "common.h"
#include <amdev.h>

int screen_width();
int screen_height();
void draw_rect(uint32_t*,int,int,int,int);

static int width;
static int height;

size_t serial_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

extern int read_key();
extern uint32_t uptime();

size_t events_read(void *buf, size_t offset, size_t len) {
    char mybuf[128];
    int key = read_key();
    size_t ret = 0;
    
    if (key){
        if (key & 0x8000) ret = sprintf(mybuf,"kd %s\n",keyname[key^0x8000]);
        else ret = sprintf(mybuf,"ku %s\n",keyname[key]);
    }else{
        ret = sprintf(mybuf,"t %d\n",uptime());
    }
    
    if (ret>len)
      ret = len;
    
    char* ch = (char*) buf;
    for (int i=0;i<ret;i++)
	ch[i] = mybuf[i];
    return ret;
}

#define DISPINFO_SIZE 128
static char dispinfo[DISPINFO_SIZE] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  size_t real_len = DISPINFO_SIZE-offset;
  if (real_len>len)
	  real_len = len;
  char* ch = (char*)buf;
  for (int i=0;i<real_len;i++) ch[i] = dispinfo[i];

  return real_len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    int nowy = (offset/4)/width;
    int nowx = (offset/4)%width;
    int num = 0;
    
    int flag = 1;

    while (flag){
        int rem = width-nowx;
        if (rem > len/4-num){
            rem = len/4-num;
            flag = 0;
        }
        draw_rect((uint32_t*)buf,nowx,nowy,rem,1);
        nowx = 0;  nowy++;
        num += rem;
        offset += 4*rem;
    }
    return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d",width=screen_width(),height=screen_height());
  //FINISHED();
}
