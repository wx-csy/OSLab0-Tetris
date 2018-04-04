#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <am.h>
#include "test.h"
#include "graphics.h"

int main() {
  if (_ioe_init() != 0) _Exit(0); 
  puts("IOE initialized!"); 
  
  test();
  time_t cur_time = time(NULL);
  printf("Current calender time is: %s\n", ctime(&cur_time));
  
  for (volatile int i = 0; i < 200000000; i++);
  
  srand(time(NULL));
  gInit();

  if (gVideoInfo->width < 640 || gVideoInfo->height < 480) {
    printf("This screen does not support 640*480 resolution!\n");
    _Exit(0);
  }

  while (1) {
    gClear();
    gUpdateKeyboard();
    if (gIsKeyPressed(G_KEY_SPACE)) {
      gFillRect(0, 0, gVideoInfo->width, gVideoInfo->height, G_BLUE);
    }
    char buf[20];
    sprintf(buf, "%d FPS", gGetFPS());
    gDrawString(20, 20, buf, G_WHITE); 
    sprintf(buf, "frame time: %d ms", gGetFrameTime() * 1000 / CLOCKS_PER_SEC);
    gDrawString(20, 40, buf, G_WHITE);
    gRender();
  }

  return 0;
}

