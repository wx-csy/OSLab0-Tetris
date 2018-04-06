#include <graphics.h>
#include <time.h>
#include <am.h>
#include <amdev.h>

_Device *dev_video = NULL;
_Device *dev_input = NULL;
static struct gVideoInfo_t video_info;
const struct gVideoInfo_t * const gVideoInfo = &video_info;

#define MAX_BUF_SIZE (4 * 1024 * 1024)
uint32_t _gBuf[MAX_BUF_SIZE];

int gInit() {
  for (int n = 1; ; n++) {
    _Device *dev = _device(n);
    if (!dev) break;
    if (dev->id == _DEV_VIDEO) {
      _VideoInfoReg info;
      dev_video = dev;
      dev->read(_DEVREG_VIDEO_INFO, &info, sizeof info);
      video_info.width = info.width;
      video_info.height = info.height;
      if (video_info.width <= 0 || video_info.height <= 0 || 
          video_info.width * video_info.height > MAX_BUF_SIZE)
        return -1;
    }
    if (dev->id == _DEV_INPUT) {
      dev_input = dev;
    }
  }
  if (dev_video == NULL || dev_input == NULL) return -1;
  _gInitAlphaTable();
  return 0;
}

static clock_t last_fps_clock;
static clock_t last_clock, frame_time;
static int last_fps, fps;

void gRender() {
  _FBCtlReg ctl = {
    .x = 0, .y = 0,
    .w = video_info.width, .h = video_info.height,
    .sync = 1,
    .pixels = _gBuf
  };
  dev_video->write(_DEVREG_VIDEO_FBCTL, &ctl, sizeof ctl);

  fps++;
  clock_t new_clock = clock();
  if (new_clock - last_fps_clock > 1000) {
    last_fps_clock = new_clock;
    last_fps = fps;
    fps = 0;
  }
  frame_time = new_clock - last_clock;
  last_clock = new_clock;
}

void gCreateScreenshot(int x, int y, int width, int height, uint8_t *buf) {
  for (int j = y; j < y + height; j++) {
    for (int i = x; i < x + width; i++) {
      uint32_t pixel = gGetPixel(i, j);
      *(buf++) = gRGB_red(pixel);
      *(buf++) = gRGB_green(pixel);
      *(buf++) = gRGB_blue(pixel);
      *(buf++) = 0xff;
    }
  }
}

int gGetFPS() {
  return last_fps;
}

uint32_t gGetFrameTime() {
  return (uint32_t)frame_time * 1000 / CLOCKS_PER_SEC;
}


