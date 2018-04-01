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
  
  for (volatile int i = 0; i < 300000000; i++);
  
  srand(time(NULL));
  gInit();
  while (1) {
    gClear();
    gFillRect(rand()%200, rand()%200, rand()%200, rand()%200, 0x0000ff);
    gRender();
  }

  return 0;
}

