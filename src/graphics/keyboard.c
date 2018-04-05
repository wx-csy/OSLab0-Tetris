#include <graphics.h>
#include <am.h>
#include <amdev.h>

static int key_status[256];
static int last_key_status[256];

void gUpdateKeyboard(void) {
  _KbdReg reg;
  do {
    dev_input->read(_DEVREG_INPUT_KBD, &reg, sizeof reg);
    key_status[reg.keycode] = reg.keydown;
  } while (reg.keycode != _KEY_NONE);
}

int gIsKeyPressed(int keycode) {
  return key_status[keycode];
}

int gIsKeyDown(int keycode) {
  int res = (key_status[keycode] && !last_key_status[keycode]);
  last_key_status[keycode] = key_status[keycode];
  return res;
}

