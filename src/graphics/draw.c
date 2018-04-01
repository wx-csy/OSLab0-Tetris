#include <string.h>
#include <graphics.h>

void gClear() {
  memset(_gBuf, 0, 4 * gVideoInfo->width * gVideoInfo->height);
}

void gSetPixel(int x, int y, gRGB_t color) {
  _gPixel(x, y) = color;
}

void gFillRect(int x, int y, int width, int height, gRGB_t color) {
  for (int j = y; j < y + height; j++) {
    for (int i = x; i < x + width; i++) {
      _gPixel(i, j) = color;
    }
  } 
}

