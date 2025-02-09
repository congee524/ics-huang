#include "common.h"
#include <amdev.h>
#include "klib.h"

extern int fg_pcb;

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //_yield();
  char *tmp = (char*) buf;
  for(int i = 0; i < len; i++){
    _putc(tmp[i]);
  }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  //_yield();
  int scan_code = read_key() & 0xffff;
  if (scan_code) {
    if (scan_code & 0x8000) {
      sprintf(buf, "kd %s\n", keyname[scan_code & 0xfff]);
    } else {
      sprintf(buf, "ku %s\n", keyname[scan_code]);
    }
  } else {
    sprintf(buf, "t %d\n", uptime());
  }

  if (strcmp(buf, "kd F1\n") == 0) fg_pcb = 1;
  if (strcmp(buf, "kd F2\n") == 0) fg_pcb = 2;
  if (strcmp(buf, "kd F3\n") == 0) fg_pcb = 3;

  return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //assert(0);
  memcpy(buf, dispinfo, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  //_yield();
  int w = screen_width();
  int bt = len / 4;
  int x = (offset / 4) % w;
  int y = (offset / 4) / w;

  // maybe input multi lines
  int flag = 1, have_in = 0, to_in = 0;
  while(flag){
    to_in = w - x;
    if (to_in > bt - have_in) {
      to_in = bt - have_in;
      flag = 0;
    }
    draw_rect((uint32_t *)buf, x, y, to_in, 1);
    x = 0;
    y++;
    have_in += to_in;
  }
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d", screen_width(), screen_height());
  // Log("the content of dispinfo: %s", dispinfo);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
