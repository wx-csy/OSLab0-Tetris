#include <stdio.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

void screen_update() {
  char buf[32];
  sprintf(buf, "FPS: %d", gGetFPS());
  gDrawImageA(50, 50, &img_ProjectN_big);
  gDrawString(20, 20, buf, G_WHITE);
}

