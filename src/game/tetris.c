#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"
#include "game.h"

typedef enum TETRO_TYPE {
  TETRO_NONE,
  TETRO_I, TETRO_J, TETRO_L, TETRO_O, TETRO_S, TETRO_T, TETRO_Z
} TETRO_TYPE;

static gRGB_t tetro_color[8] = {
  [TETRO_NONE] = G_BLACK,
  [TETRO_I] = G_CYAN,
  [TETRO_J] = G_BLUE,
  [TETRO_L] = G_MAGENTA,
  [TETRO_O] = G_GRAY,
  [TETRO_S] = G_GREEN,
  [TETRO_T] = G_YELLOW,
  [TETRO_Z] = G_RED,
};

//  This game uses the so-called "super rotation system" 
// without wall kicks.
uint8_t tetris_shape[8][4][4][4] = {
  [TETRO_I] = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
    {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}},
    {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
    {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
  },
  [TETRO_J] = {
    {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},
    {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
    {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}
  },
  [TETRO_L] = {
    {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},
    {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},
    {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}
  },
  [TETRO_O] = {
    {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}
  },
  [TETRO_S] = {
    {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},
    {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
    {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
  },
  [TETRO_T] = {
    {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
    {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},
    {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
  },
  [TETRO_Z] = {
    {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
    {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
    {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}
  }
};

#define NUM_ROWS  20
#define NUM_COLS  10

static inline int is_in_playground(int x, int y) {
  return x >= 0 && x < NUM_ROWS && y >= 0 && y < NUM_COLS;
}

static TETRO_TYPE grid[NUM_ROWS][NUM_COLS];

static struct {
  TETRO_TYPE type;
  int row, col;
  unsigned rot; 
} current;

static inline void draw_square(int offx, int offy, int row, int col,
    TETRO_TYPE type) {
  gFillRect(offx + col * 24 + 2, offy + row * 24 + 2, 20, 20, 
      tetro_color[type]);
}

static void draw_current_tetro(int offx, int offy) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris_shape[current.type][current.rot][i][j] == 0) continue;
      if (is_in_playground(current.row + i, current.col + j))
        draw_square(offx, offy, current.row + i, current.col + j,
            current.type);
    }
  }
}

static void draw_grid(int offx, int offy) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (grid[i][j] == TETRO_NONE) continue;
      draw_square(offx, offy, i, j, grid[i][j]);
    }
  }
}

static int is_valid_pos() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris_shape[current.type][current.rot][i][j] == 0) continue;
      int row = current.row + i, col = current.col + j;
      if (row >= NUM_ROWS || col < 0 || col >= NUM_COLS) return 0;
      if (grid[row][col] != TETRO_NONE) return 0;
    }
  }
  return 1;
}

static void generate_new_tetro() {
  current.type = rand() % 7 + 1;
  current.row = -1;
  current.col = 3;
  current.rot = 0;
  if (!is_valid_pos())
    current.row--;
}

static void game_over() {
  printf("Game Over!\n");
  _Exit(0);
}

static void fix_current_to_grid() {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_ROWS; j++) {
      if (tetris_shape[current.type][current.rot][i][j] == 0) continue;
      if (current.row + i < 0) game_over();
      grid[current.row + i][current.col + j] = current.type;
    }
  }
}

static void current_down() {
  current.row++;
  if (!is_valid_pos()) {
    fix_current_to_grid();
    generate_new_tetro();
  }
}

static void tetris_key_proc() {
#define DEBUG
#ifdef DEBUG
  // These keys are for debug only.
  if (gIsKeyDown(G_KEY_Q)) {
    current.type++;
    current.type %= sizeof(tetro_color) / sizeof(gRGB_t);
  }
  if (gIsKeyDown(G_KEY_UP)) {
    current.row--;
  }
#endif
  if (gIsKeyDown(G_KEY_LEFT)) {
    current.col--;
    if (!is_valid_pos()) {
      current.col++;
    }
  }
  if (gIsKeyDown(G_KEY_RIGHT)) {
    current.col++;
    if (!is_valid_pos()) {
      current.col--;
    }
  }
  if (gIsKeyDown(G_KEY_DOWN)) { 
    current.row++;
  }
  if (gIsKeyDown(G_KEY_RSHIFT)) {
    current.rot++;
    current.rot &= 3;
    if (!is_valid_pos()) {
      current.rot--;
      current.rot &= 3;
    }
  }
}

uint32_t res_time;

void tetris_init() {
  generate_new_tetro();
  srand(time(NULL));
  res_time = 0;
}

void tetris_proc() {
  tetris_key_proc();
  res_time += gGetFrameTime();
  if (res_time > 1000) {
    res_time %= 1000;
    current_down();
  }
  gDrawRect(200, 0, 439, 479, G_WHITE);
  draw_grid(200, 0);
  draw_current_tetro(200, 0);
}

