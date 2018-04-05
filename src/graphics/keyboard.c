#include <graphics.h>
#include <am.h>
#include <amdev.h>

static int key_status[256];

void gUpdateKeyboard(void) {
  _KbdReg reg;
  do {
    dev_input->read(_DEVREG_INPUT_KBD, &reg, sizeof reg);
    if (reg.keydown) 
      key_status[reg.keycode]++;
    else 
      key_status[reg.keycode] = 0;
  } while (reg.keycode != _KEY_NONE);
}

int gIsKeyPressed(int keycode) {
  return key_status[keycode] > 0;
}

int gIsKeyDown(int keycode) {
  if (key_status[keycode] > 0) {
    key_status[kecode]--;
    return 1;
  } else {
    return 0;
  }
}

