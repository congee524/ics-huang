#include <am.h>
#include <x86.h>
#include <amdev.h>
#define I8042_DATA_PORT 0x60
#define KEYDOWN_MASK 0x8000

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      kbd->keycode = inl(I8042_DATA_PORT);
      if (kbd->keycode & KEYDOWN_MASK) {
          kbd->keydown = 1;
      } else {
          kbd->keydown = _KEY_NONE;
      }
      //kbd->keycode = _KEY_NONE;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
