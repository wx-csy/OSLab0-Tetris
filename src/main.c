#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <am.h>
#include "test.h"

int main() {
  puts("Booting ...");
  if (_ioe_init() != 0) {
    puts("IOE initialization failed!");
    _Exit(0); 
  };
  time_t cur_time = time(NULL);
  printf("Current calender time is: %s\n", ctime(&cur_time));
  return 0;
}

