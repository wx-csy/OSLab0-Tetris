#include <graphics.h>
#include <am.h>
#include <amdev.h>

static _Device *dev_video = NULL;
static _VideoInfoReg video_info;

#define MAX_BUF_SIZE (4 * 1024 * 1024)
uint32_t _gBuf[MAX_BUF_SIZE];

int gInit() {
  for (int n = 1; ; n++) {
    _Device *dev = _device(n);
    if (!dev) break;
    if (dev->id == _DEV_VIDEO) {
      _VideoInfoReg video_info
      dev_video = dev;
      dev->read(_DEVREG_VIDEO_INFO, &video_info, sizeof video_info);
      if (video_info.width <= 0 || video_info.height <= 0 || 
          video_info.width * video_info.height > MAX_BUF_SIZE)
        return -1;
      else
        return 0;
    }
  }
  return -1;
}

void gGetVideoInfo(gVideoInfo *info) {
  info->width = video_info.width;
  info->height = video_info.height;
}

void gRender() {
  _FBCtlReg ctl = {
    .x = 0, .y = 0,
    .w = video_info.width, .h = video_info.height,
    .sync = 1,
    .pixel = &_gBuf;
  };
  dev->write(_DEVREG_VIDEO_FBCTL, &ctl, sizeof ctl);
}

