#include "render.h"

Coordinates RENDER_ORIGIN;

int cols_cache = -1;
int lines_cache = -1;

Coordinates get_render_origin() {
	if (cols_cache != COLS || lines_cache != LINES) {
		cols_cache = COLS;
		lines_cache = LINES;
		RENDER_ORIGIN = new_coordinates((int) ((COLS - BOARD_WIDTH * 2) / 2), (int) ((LINES - BOARD_HEIGHT) / 2));
		clear();
	}

	return RENDER_ORIGIN;
}