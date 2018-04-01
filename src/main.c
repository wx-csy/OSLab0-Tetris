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
  srand(time(NULL));
  
  for (volatile int i = 0; i < 500000000; i++);

  gInit();
  while (1) {
    gClear();
    gSetPixel(rand()%100, rand()%200, gRGB(rand()&0xff, rand()&0xff, rand()&0xff));
    gRender();
  }

  return 0;
}

