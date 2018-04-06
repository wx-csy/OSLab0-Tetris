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

static const gImage_t img_screenshot = {640, 480, 4, screenshot};

static const gColorMatrix mat_bw = 
  {{86, 86, 86}, {86, 86, 86}, {86, 86, 86}};

void gameover_proc() {
  uint32_t alpha;
  if (status == GAMEOVER_NOT_START) {
      c_start = clock();
      status = GAMEOVER_RUNNING;
  } 
  clock_t c_now = clock();
  alpha = (uint32_t)(c_now - c_start) * 256;
  alpha = alpha / CLOCKS_PER_SEC / 4;
  if (status == GAMEOVER_RUNNING && alpha > 0xff)
    status = GAMEOVER_HOLD;
  switch (status) {
    case GAMEOVER_RUNNING:
      gDrawImageA2(0, 0, &img_screenshot, 0xff - alpha);
      gDrawImageAA(80, 171, &img_gameover, alpha);
      return;
    case GAMEOVER_HOLD:
      gDrawImageM(0, 0, &img_screenshot, mat_bw);    
      gDrawImageA(80, 171, &img_gameover);
      return;
    default:  assert(0);                       
  }
}

