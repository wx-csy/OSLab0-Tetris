#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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
static const uint8_t tetris_shape[8][4][4][4] = {
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

uint32_t score;

static inline void draw_square(int offx, int offy, int row, int col,
    TETRO_TYPE type) {
  gFillRect(offx + col * 24 + 2, offy + row * 24 + 2, 20, 20, 
      tetro_color[type]);
  gDrawRect(offx + col * 24 + 2, offy + row * 24 + 2,
      offx + col * 24 + 21, offy + row * 24 + 21, G_GRAY);
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
      if (is_in_playground(row, col) && 
          grid[row][col] != TETRO_NONE) return 0;
    }
  }
  return 1;
}

static TETRO_TYPE tetro_queue[14];
static int next_tetro_pos;

static void new_tetro_group(TETRO_TYPE group[static 7]) {
  int value = rand() % 5040;
  int flag[7] = {0};
  for (int i = 7; i; i--) {
    int n = value % i + 1; value /= i;
    int id;
    for (id = 0; id < 7; id++) {
      if (flag[id] == 0) n--;
      if (n == 0) break;
    }
    assert(id < 7 && flag[id] == 0);
    flag[id] = 1;
    group[i - 1] = id + 1;
  }
}

static void new_tetro() {
  current.type = tetro_queue[next_tetro_pos];
  next_tetro_pos = (next_tetro_pos + 1) % 14;
  if (next_tetro_pos == 7) new_tetro_group(tetro_queue);
  if (next_tetro_pos == 0) new_tetro_group(tetro_queue + 7);
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
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris_shape[current.type][current.rot][i][j] == 0) continue;
      if (current.row + i < 0) game_over();
      grid[current.row + i][current.col + j] = current.type;
    }
  }
}

static void perform_elimination() {
  int elim_count = 0;
  for (int i = NUM_ROWS - 1; i >= 0; i--) {
    for (int j = 0; j < NUM_COLS; j++)
      if (grid[i][j] == TETRO_NONE) goto next;
    elim_count++;
    for (int ii = i; ii > 0; ii--)
      for (int j = 0; j < NUM_COLS; j++) 
        grid[ii][j] = grid[ii-1][j];
    for (int j = 0; j < NUM_COLS; j++)
      grid[0][j] = TETRO_NONE;
    i++;
next:;
  }
  assert(elim_count <= 4);
  static const uint32_t score_add[] = {0, 1, 3, 5, 8};
  score += score_add[elim_count];
}

static void current_down() {
  current.row++;
  if (!is_valid_pos()) {
    current.row--;
    fix_current_to_grid();
    perform_elimination();
    new_tetro();
  }
}

static void tetris_key_proc() {
#define DEBUG
#ifdef DEBUG
  // These keys are for debug only.
  if (gIsKeyDown(G_KEY_Q)) {
    current.type++;
    current.type %= 8;
  }
  if (gIsKeyDown(G_KEY_W)) {
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
    current.rot--;
    current.rot &= 3;
    if (!is_valid_pos()) {
      current.rot++;
      current.rot &= 3;
    }
  }
  if (gIsKeyDown(G_KEY_UP)) {
    current.rot++;
    current.rot &= 3;
    if (!is_valid_pos()) {
      current.rot--;
      current.rot &= 3;
    }
  }
  if (gIsKeyDown(G_KEY_SPACE)) {
    current_down();
  }
}

uint32_t res_time;

void tetris_init() {
  srand(time(NULL));
  score = 0;
  res_time = 0;
  next_tetro_pos = 0;
  new_tetro_group(tetro_queue);
  new_tetro_group(tetro_queue + 7);
  new_tetro();
}

int speed_step = 250;

void tetris_proc() {
  tetris_key_proc();
  res_time += gGetFrameTime();
  if (res_time > speed_step) {
    res_time %= speed_step;
    current_down();
  }
  gDrawRect(200, 0, 439, 479, G_WHITE);
  draw_grid(200, 0);
  draw_current_tetro(200, 0);
  char score_str[32];
  sprintf(score_str,"Score: %u", score);
  gDrawString(20, 40, score_str, G_WHITE);
}

