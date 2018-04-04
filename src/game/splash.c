#include <stdio.h>
#include <time.h>
#include "graphics.h"
#include "game.h"
#include "resource.h"

static int status = 0;

static clock_t c_start;

void init_splash() {
  status = 1;
  c_start = clock();
}

void draw_splash() {
  clock_t c_now = clock();
  uint32_t alpha = (c_now - c_start) * 256;
  alpha >>= 12;
  if (alpha > 0xff) alpha = 0xff;
  gDrawImageAA(220, 100, &img_ProjectN_big, alpha);
  gDrawStringA(205, 330, "Product of ProjectN Gaming", G_WHITE, alpha);
}

