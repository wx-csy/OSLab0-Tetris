#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

#define SPLASH_NOT_START  0
#define SPLASH_RUNNING    1
#define SPLASH_HOLD       2
#define SPLASH_END        3

static int status = SPLASH_NOT_START;

static clock_t c_start;

void splash_proc() {
  clock_t c_now = clock();
  uint32_t alpha;
  switch (status) {
    case SPLASH_NOT_START: 
      c_start = clock();
      status = SPLASH_RUNNING;
      return;
    case SPLASH_RUNNING:
      alpha = (uint32_t)(c_now - c_start) * 256;
      alpha = alpha / CLOCKS_PER_SEC / 4;
      if (alpha > 0xff) status = SPLASH_HOLD;
      gDrawImageAA(220, 20, &img_logo, alpha);
      gDrawImageAA(220, 100, &img_ProjectN_big, alpha);
      gDrawStringA(205, 330, "Product of ProjectN Gaming", 
          G_WHITE, alpha);
      return;
    case SPLASH_HOLD:
      gDrawImageA(220, 20, &img_logo, alpha);
      gDrawImageA(220, 100, &img_ProjectN_big);
      gDrawString(205, 330, "Product of ProjectN Gaming", G_WHITE);
      gDrawString(235, 355, "Press 'C' to Start", G_WHITE); 
      if (gIsKeyDown(G_KEY_C)) {
        status = SPLASH_END;
        tetris_init();
        current_scene_proc = tetris_proc;
      }
      return;
    default:  assert(0);                       
  }
}

