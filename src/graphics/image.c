#include <graphics.h>

static inline uint8_t channel_blend(uint8_t fore, uint8_t back, 
    uint8_t alpha) {
  return (fore * alpha + back * (255 - alpha)) / 255;
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
            channel_blend(fb, gRGB_blue(pixel), fa),
          ));
    }
  }
  return 0;
}

