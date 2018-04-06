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

static const gColorMatrix mat_identity = 
  {{0xff, 0, 0}, {0, 0xff, 0}, {0, 0, 0xff}};

static const gColorMatrix mat_gray = 
  {{84, 84, 84}, {150, 150, 150}, {28, 28, 28}};

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
  gColorMatrix mat;
  switch (status) {
    case GAMEOVER_RUNNING:
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          mat[i][j] = 
            gChannelBlend(mat_gray[i][j], mat_identity[i][j], alpha);
        }
      }
      gDrawImageM(0, 0, &img_screenshot, mat);
      gDrawImageAA(80, 171, &img_gameover, alpha);
      return;
    case GAMEOVER_HOLD:
      gDrawImageM(0, 0, &img_screenshot, mat_gray);
      gDrawImageA(80, 171, &img_gameover);
      return;
    default:  assert(0);                       
  }
}

