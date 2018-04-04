#include <stdio.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

void (*current_scene_proc)(void);

void init_game() {
  init_splash();
  current_scene_proc = splash_proc;
}

void main_proc() {
  current_scene_proc();
  char fps_buf[32];
  sprintf(fps_buf, "FPS: %d", gGetFPS());
  gDrawString(20, 20, fps_buf, G_WHITE);
}

