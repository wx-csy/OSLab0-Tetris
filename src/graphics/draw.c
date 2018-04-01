#include <graphics.h>

void gSetPixel(int x, int y, gRGB_t color) {
  _gPixel(x, y) = color;
}

