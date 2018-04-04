#include <stdio.h>
#include <game.h>
#include <graphics.h>

void screen_update() {
  char buf[32];
  sprintf(buf, "FPS: %d", gGetFPS());
  gDrawString(20, 20, buf, G_WHITE);
}

