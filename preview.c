#include "board.h"

char get_fell_tetrimino_y(FallingTetrimino tetrimino) {
	char *tetrimino_matrix = get_tetrimino_matrix(tetrimino.tetrimino_type_id, tetrimino.orientation);

	for (int i = tetrimino.coordinates.y; i < BOARD_HEIGHT + 1; i++) {
		char *board_matrix = get_board_4x4_matrix_at(((int) tetrimino.coordinates.x / 2), i);

		if (does_overlap(tetrimino_matrix, board_matrix)) {
			return i - 1;
		}
	}

	return -1;
}
