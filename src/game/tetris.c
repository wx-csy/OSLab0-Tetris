#include <stdlib.h>
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
  return x >= 0 && x < NUM_ROWS && y >= 0 && y<= NUM_COLS;
}

static TETRO_TYPE grid[NUM_ROWS][NUM_COLS];

static struct {
  TETRO_TYPE type;
  int row, col;
  unsigned rot; 
} current;

static void generate_new_tetro() {
  current.type = rand() % 7 + 1;
  current.row = 0;
  current.col = 3;
  current.rot = 0;
}

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

void tetris_init() {
  generate_new_tetro();
  srand(time(NULL));
}

void tetris_proc() {
  generate_new_tetro();
  gDrawRect(200, 0, 439, 479, G_WHITE);
  draw_grid(200, 0);
  draw_current_tetro(200, 0);
}

