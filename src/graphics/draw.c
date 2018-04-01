#include <string.h>
#include <graphics.h>

void gClear() {
  memset(_gBuf, 0, 4 * gVideoInfo.width * gVideoInfo.height);
}

void gSetPixel(int x, int y, gRGB_t color) {
  _gPixel(x, y) = color;
}

