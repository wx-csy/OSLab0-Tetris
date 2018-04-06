#include "graphics.h"

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
            gChannelBlend(fr, gRGB_red(pixel), fa),
            gChannelBlend(fg, gRGB_green(pixel), fa),
            gChannelBlend(fb, gRGB_blue(pixel), fa)
          ));
    }
  }
  return 0;
}

int gDrawImageA2(int x, int y, const gImage_t *image, uint8_t alpha) {
  const uint8_t *data = image->pixel_data;
  if (image->bytes_per_pixel != 4) return -1;
  for (int j = y; j < y + image->height; j++) {
    for  (int i = x; i < x + image->width; i++) {
      gRGB_t pixel = gGetPixel(i, j);
      if (pixel == 0xffffffff) continue;
      uint8_t fr = *(data++), fg = *(data++), fb = *(data++);
      data++;
      gSetPixel(i, j, gRGB(
            gChannelBlend(fr, gRGB_red(pixel), alpha),
            gChannelBlend(fg, gRGB_green(pixel), alpha),
            gChannelBlend(fb, gRGB_blue(pixel), alpha)
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
      fa = gChannelBlend(fa, 0x00, alpha);
      gSetPixel(i, j, gRGB(
            gChannelBlend(fr, gRGB_red(pixel), fa),
            gChannelBlend(fg, gRGB_green(pixel), fa),
            gChannelBlend(fb, gRGB_blue(pixel), fa)
          ));
    }
  }
  return 0;
}

int gDrawImageM(int x, int y, const gImage_t *image, 
    const gColorMatrix matrix) {
  const uint8_t *data = image->pixel_data;
  gRGB_t last_pixel = 0, transformed_color = 0;
  if (image->bytes_per_pixel != 4) return -1;
  for (int j = y; j < y + image->height; j++) {
    for  (int i = x; i < x + image->width; i++) {
      uint8_t fr = *(data++), fg = *(data++), fb = *(data++);
      data++; 
      gRGB_t pixel = gRGB(fr, fg, fb);
      if (pixel != last_pixel) {
        transformed_color = gColorMatrixTransform(pixel, matrix);
        last_pixel = pixel;
      }
      gSetPixel(i, j, transformed_color);
    }
  }
  return 0;
}

