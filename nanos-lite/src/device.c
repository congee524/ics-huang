#include "common.h"
#include <amdev.h>
#include "klib.h"

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i = 0; i < len; i++){
    _putc(((char*)buf)[i]);
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
  return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  assert(0);
  memcpy(buf, (void *)dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int w = screen_width();
  int h = screen_height();
  int x = (offset / 4) % w;
  int y = (offset / 4) / w;
  Log("w %d h % d coordinate %d %d", w, h, x, y);
  draw_rect((uint32_t *)buf, x, y, w, h); 
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo, "WIDTH:%dHEIGHT:%d", screen_width(), screen_height);
  Log("the content of dispinfo: %s", dispinfo);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
