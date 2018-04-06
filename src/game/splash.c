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
  alpha = (uint32_t)(c_now - c_start) * 256;
  alpha = alpha / CLOCKS_PER_SEC / 3;
  if (status == SPLASH_RUNNING && 
       (alpha > 0xff || gIsKeyPressed(G_KEY_RETURN)))
    status = SPLASH_HOLD;
  switch (status) {
    case SPLASH_NOT_START: 
      c_start = clock();
      status = SPLASH_RUNNING;
      return;
    case SPLASH_RUNNING:
      gDrawImageAA(120, 20, &img_logo, alpha);
      gDrawImageAA(245, 165, &img_ProjectN, alpha);
      gDrawStringA(210, 350, "Product of ProjectN Gaming", 
          G_WHITE, alpha);
      return;
    case SPLASH_HOLD:
      gDrawImageA(120, 20, &img_logo);
      gDrawImageA(245, 165, &img_ProjectN);
      gDrawString(210, 350, "Product of ProjectN Gaming", G_WHITE);
      gDrawString(245, 375, "Press 'C' to Start", G_WHITE); 
      if (gIsKeyDown(G_KEY_C)) {
        status = SPLASH_END;
        tetris_init();
        current_scene_proc = tetris_proc;
      }
      return;
    default:  assert(0);                       
  }
}

