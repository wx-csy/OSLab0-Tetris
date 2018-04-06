#include <stdint.h>
#include "graphics.h"

extern void (*current_scene_proc)(void);

extern uint8_t screenshot[640 * 480 * 4];

void init_game(void);

void main_proc(void);

void splash_proc(void);

void tetris_init(void);

void tetris_proc(void);

void gameover_proc(void);

