#include <stdbool.h>

#include "tetrimino.h"

Coordinates get_render_origin();

void init_board();

void destroy_board();

char get_pixel_at(int x, int y);

void set_pixel_at(int x, int y, char pixel);

bool is_line_full(int y);

void fall_board_on(int y);

char *get_board_4x4_matrix_at(int x, int y);

void render_board();
