#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <am.h>
#include "test.h"
#include "game.h"
#include "graphics.h"

int main() {
  if (_ioe_init() != 0) _Exit(0); 
  puts("IOE initialized!"); 
  
  test();
  time_t cur_time = time(NULL);
  printf("Current calendar time is: %s\n", ctime(&cur_time));
  
  for (volatile int i = 0; i < 100000000; i++);
  
  srand(time(NULL));
  gInit();

  if (gVideoInfo->width < 640 || gVideoInfo->height < 480) {
    printf("This screen does not support 640*480 resolution!\n");
    _Exit(0);
  }
  
  init_game();

  while (1) {
    gClear();
    gUpdateKeyboard();
    main_proc();
    gRender();
  }

  return 0;
}

