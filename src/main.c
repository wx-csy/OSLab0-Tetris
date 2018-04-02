#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <am.h>
#include "test.h"
#include "graphics.h"

int main() {
  puts("Booting ...");
  if (_ioe_init() != 0) {
    puts("IOE initialization failed!");
    _Exit(0); 
  } else {
    puts("IOE initialized!"); 
  };
  test();
  time_t cur_time = time(NULL);
  printf("Current calender time is: %s\n", ctime(&cur_time));
  
  for (volatile int i = 0; i < 200000000; i++);
  
  srand(time(NULL));
  gInit();
  while (1) {
    gClear();
    gUpdateKeyboard();
    if (gIsKeyPressed(G_KEY_SPACE)) {
      gFillRect(0, 0, gVideoInfo->width, gVideoInfo->height, G_BLUE);
    }
    gRender();
  }

  return 0;
}

