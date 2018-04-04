#include <stdio.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

void init_game() {
  init_splash();
}

void screen_update() {
  char buf[32];
  sprintf(buf, "FPS: %d", gGetFPS());
  draw_splash();
  gDrawString(20, 20, buf, G_WHITE);
}

