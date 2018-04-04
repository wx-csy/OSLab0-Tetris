#include <graphics.h>

static inline uint8_t channel_blend(uint8_t fore, uint8_t back, 
    uint8_t alpha) {
  uint32_t a = alpha;
  if (a == 0xff) a++;
  return (fore * a + back * (256 - a)) >> 8;
}

int gDrawImageA(int x, int y, const gImage_t *image) {
  const uint8_t *data = image->pixel_data;
  if (image->bytes_per_pixel != 4) return -1;
  for (int j = y; j < y + image->height; j++) {
    for  (int i = x; i < x + image->width; i++) {
      gRGB_t pixel = gGetPixel(i, j);
      if (pixel == 0xffffffff) continue;
      uint8_t fr = *(data++), fg = *(data++), 
              fb = *(data++), fa = *(data++);
      gSetPixel(i, j, gRGB(
            channel_blend(fr, gRGB_red(pixel), fa),
            channel_blend(fg, gRGB_green(pixel), fa),
            channel_blend(fb, gRGB_blue(pixel), fa)
          ));
    }
  }
  return 0;
}

int gDrawImageAA(int x, int y, const gImage_t *image, uint8_t alpha) {
  const uint8_t *data = image->pixel_data;
  if (image->bytes_per_pixel != 4) return -1;
  for (int j = y; j < y + image->height; j++) {
    for  (int i = x; i < x + image->width; i++) {
      gRGB_t pixel = gGetPixel(i, j);
      if (pixel == 0xffffffff) continue;
      uint8_t fr = *(data++), fg = *(data++), 
              fb = *(data++), fa = *(data++);
      fa = channel_blend(fa, 0x00, alpha);
      gSetPixel(i, j, gRGB(
            channel_blend(fr, gRGB_red(pixel), fa),
            channel_blend(fg, gRGB_green(pixel), fa),
            channel_blend(fb, gRGB_blue(pixel), fa)
          ));
    }
  }
  return 0;
}

