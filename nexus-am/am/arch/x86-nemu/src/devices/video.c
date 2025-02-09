#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

#define min(a, b) (a < b ? a : b)
#define W 400
#define H 300

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;
//static uint32_t fb[W * H];

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = W;
      info->height = H;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
      int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
      uint32_t *pixels = ctl->pixels;
      int cp_bytes = sizeof(uint32_t) * min(w, W - x);
      for (int j = 0; j < h && y + j < H; j++) {
          //printf("pixels is 0x%08x\n", *pixels);
          //fb[(y + j) * W + x] = *pixels;
          memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
          //printf("the position is %d\n", (y + j) * W + x);
          //printf("fb is 0x%08x\n", fb[(y + j) * W + x]);
          pixels += w;
      }
      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
    memset(fb, 0, W * H * sizeof(uint32_t));
}

