#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

#define GAMEOVER_NOT_START  0
#define GAMEOVER_RUNNING    1
#define GAMEOVER_HOLD       2
#define GAMEOVER_END        3

static int status = GAMEOVER_NOT_START;

static clock_t c_start;

void gameover_proc() {
  clock_t c_now = clock();
  uint32_t alpha;
  switch (status) {
    case GAMEOVER_NOT_START: 
      c_start = clock();
      status = GAMEOVER_RUNNING;
      return;
    case GAMEOVER_RUNNING:
      alpha = (uint32_t)(c_now - c_start) * 256;
      alpha = alpha / CLOCKS_PER_SEC / 4;
      if (alpha > 0xff) status = GAMEOVER_HOLD;
      gDrawImageAA(120, 20, &img_gameover, alpha);
      return;
    case GAMEOVER_HOLD:
      gDrawImageA(120, 20, &img_gameover);
      return;
    default:  assert(0);                       
  }
}

