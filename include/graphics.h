#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdint.h>
#include <time.h>
#include <am.h>
#include <amdev.h>

extern uint32_t _gBuf[];
extern _Device *dev_video, *dev_input;

extern struct gVideoInfo_t {
  uint32_t width, height;
} const * const gVideoInfo;

#define _gPixel(x, y) (_gBuf[(y)*(gVideoInfo->width) + x])

// On success, 0 is returned; otherwise -1 is returned.
int gInit(void);

void gRender(void);
void gCreateScreenshot(int x, int y, int width, int height, uint8_t *buf);
int gGetFPS(void);
uint32_t gGetFrameTime(void);

typedef uint32_t gRGB_t;

static inline gRGB_t gRGB(uint8_t r, uint8_t g, uint8_t b) {
  return (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b;
}

static inline uint8_t gRGB_red(gRGB_t color) {
  return 0xff & (color >> 16);
}

static inline uint8_t gRGB_green(gRGB_t color) {
  return 0xff & (color >> 8);
}

static inline uint8_t gRGB_blue(gRGB_t color) {
  return 0xff & color;
}

static inline uint8_t gChannelBlend(uint8_t fore, uint8_t back,
    uint8_t alpha) {
  uint32_t a = alpha;
  if (a == 0xff) a++;
  return (fore * a + back * (256 - a)) >> 8;
}

static inline gRGB_t gColorBlend(gRGB_t fore, gRGB_t back, 
    uint8_t alpha) {
  return gRGB(
      gChannelBlend(gRGB_red(fore), gRGB_red(back), alpha),
      gChannelBlend(gRGB_green(fore), gRGB_green(back), alpha),
      gChannelBlend(gRGB_blue(fore), gRGB_blue(back), alpha));
}

#define G_BLACK     0x000000
#define G_WHITE     0xffffff
#define G_RED       0xff0000
#define G_GREEN     0x00ff00
#define G_BLUE      0x0000ff
#define G_YELLOW    0xffff00
#define G_MAGENTA   0xff00ff
#define G_CYAN      0x00ffff
#define G_GRAY      0x7f7f7f

static inline void gSetPixel(int x, int y, gRGB_t color) {
  if (x < gVideoInfo->width && y <= gVideoInfo->height &&
      x >= 0 && y >= 0)
    _gPixel(x, y) = color;
}

static inline gRGB_t gGetPixel(int x, int y) {
  if (x < gVideoInfo->width && y <= gVideoInfo->height &&
      x >= 0 && y >= 0)
    return _gPixel(x, y);
  else
    return 0xffffffff;
}

void gClear(void);
void gDrawRect(int x1, int y1, int x2, int y2, gRGB_t color);
void gFillRect(int x, int y, int width, int height, gRGB_t color);
void gDrawString(int x, int y, const char *str, gRGB_t color);
void gDrawStringA(int x, int y, const char *str, gRGB_t color,
    uint8_t alpha);

typedef struct gImage_t {
  int width, height;
  int bytes_per_pixel;
  const uint8_t *pixel_data;
} gImage_t;

int gDrawImageA(int x, int y, const gImage_t *image);
int gDrawImageAA(int x, int y, const gImage_t *image, uint8_t alpha);

#define _G_KEY_NAME(k) G_KEY_##k,
enum gKeyCode {
  G_KEY_NONE = 0,
  _KEYS(_G_KEY_NAME)
};
#undef _G_KEY_NAME

void gUpdateKeyboard(void);
int gIsKeyPressed(int keycode);
int gIsKeyDown(int keycode);

#endif

