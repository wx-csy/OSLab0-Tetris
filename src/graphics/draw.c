#include <string.h>
#include <graphics.h>

void gClear() {
  memset(_gBuf, 0, 4 * gVideoInfo->width * gVideoInfo->height);
}

void gDrawRect(int x1, int y1, int x2, int y2, gRGB_t color) {
  for (int i = x1; i <= x2; i++) {
    gSetPixel(i, y1, color);
    gSetPixel(i, y2, color);
  }
  for (int j = y1; j <= y2; j++) {
    gSetPixel(x1, j, color);
    gSetPixel(x2, j, color);
  }
}

void gFillRect(int x, int y, int width, int height, gRGB_t color) {
  for (int j = y; j < y + height; j++) {
    for (int i = x; i < x + width; i++) {
      gSetPixel(i, j, color);
    }
  } 
}

