#include "graphics.h"

uint8_t _gAlphaTable[256][256][256];

void _gInitAlphaTable() {
  for (uint32_t alpha = 0; alpha < 256; alpha++) {
    for (uint32_t fore = 0; fore < 256; fore++) {
      for (uint32_t back = 0; back < 256; back++) {
        _gAlphaTable[alpha][fore][back] = 
          (fore * alpha + back * (0xff - alpha)) / 0xff;
      }
    }
  }
}

