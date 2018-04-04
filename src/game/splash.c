#include <stdio.h>
#include <time.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

#define SPLASH_NOT_START  0
#define SPLASH_RUNNING    1
#define SPLASH_HOLD       2
#define SPLASH_END        3

static int status = SPLASH_NOT_START;

static clock_t c_start;

void init_splash() {
  status = SPLASH_RUNNING;
  c_start = clock();
}

void splash_proc() {
  clock_t c_now = clock();
  uint32_t alpha = (c_now - c_start) * 256;
  alpha >>= 12;
  if (alpha > 0xff) alpha = 0xff;
  gDrawImageAA(220, 100, &img_ProjectN_big, alpha);
  gDrawStringA(205, 330, "Product of ProjectN Gaming", G_WHITE, alpha);
}

