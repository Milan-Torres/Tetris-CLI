#include "tetrimino.h"

#include <stdlib.h>
#include <ncurses.h>

TetriminoType new_tetrimino_type(int structure_argument, int color) {
	char *structure;
	structure = malloc(sizeof(char) * 2);
	structure[1] = (char) (structure_argument & 0xFF);
	structure[0] = (char) ((structure_argument >> 8) & 0xFF);

	TetriminoType tetrimino = {structure, color};
	return tetrimino;
}

void destroy_tetrimino_type(TetriminoType *tetrimino) {
	free((*tetrimino).structure);
}

TetriminoType *tetriminos_types;
int tetriminos_types_length = 0;

void register_base_tetriminos_types() {
	tetriminos_types_length = TETRIMINOS_NUMBER;
	tetriminos_types = malloc(sizeof(TetriminoType) * tetriminos_types_length);

	tetriminos_types[I_TETRIMINO] = new_tetrimino_type(0b1111 << 12, CYAN);
	tetriminos_types[O_TETRIMINO] = new_tetrimino_type(0b0000011001100000, YELLOW);
	tetriminos_types[T_TETRIMINO] = new_tetrimino_type(0b11100100 << 8, MAGENTA);
	tetriminos_types[J_TETRIMINO] = new_tetrimino_type(0b11100010 << 8, BLUE);
	tetriminos_types[L_TETRIMINO] = new_tetrimino_type(0b11101000 << 8, WHITE);
	tetriminos_types[S_TETRIMINO] = new_tetrimino_type(0b01101100 << 8, GREEN);
	tetriminos_types[Z_TETRIMINO] = new_tetrimino_type(0b11000110 << 8, RED);
}

void unregister_tetriminos_types() {
	for (int i = 0; i < tetriminos_types_length; i++) {
		destroy_tetrimino_type(&tetriminos_types[i]);
	}
	free(tetriminos_types);
}

TetriminoType get_tetrimino_type_by_id(unsigned int tetrimino_type_id) {
	return tetriminos_types[tetrimino_type_id];
}

void set_bit_in_matrix(char matrix[2], unsigned char x, unsigned char y, char value) {
	matrix[(unsigned int) (y / 2)] = ((matrix[(unsigned int) (y / 2)] & ((0x0F << (y % 2) * 4) |
																		 ((((0xFF ^ (1 << (3 - x))) & 0x0F))
																				 << (((y + 1) % 2) * 4)))) |
									  (((value & 0x01) << (3 - x)) << ((y + 1) % 2) * 4));
}

char get_bit_in_matrix(char matrix[2], unsigned char x, unsigned char y) {
	return ((matrix[(unsigned int) (y / 2)] >> (((y + 1) % 2) * 4)) >> (3 - x)) & 0x01;
}

char get_line_in_matrix(char matrix[2], unsigned char y) {
	return (matrix[((unsigned int) (y / 2))] >> (((y + 1) % 2) * 4)) & 0x0F;
}

void set_line_in_matrix(char matrix[2], unsigned char y, char line) {
	matrix[((unsigned int) (y / 2))] = ((matrix[((unsigned int) (y / 2))] & (0x0F << (y % 2) * 4)) |
										((line & 0x0F) << (((y + 1) % 2) * 4)));
}

Coordinates get_min_coordinates(char *structure) {
	Coordinates min_coordinates = {100, 100};

	for (int i = 0; i < 4; i++) {
		char current_line = get_line_in_matrix(structure, i);
		for (int j = 0; j < 4; j++) {
			if (((current_line >> (3 - j)) & 0x01)) {
				if (j < min_coordinates.x) {
					min_coordinates.x = j;
				}
				if (i < min_coordinates.y) {
					min_coordinates.y = i;
				}
			}
		}
	}

	return min_coordinates;
}

void move_matrix_up(char matrix[2], char amount) {
	for (int i = 0; i < (4 - amount); i++) {
		set_line_in_matrix(matrix, i, get_line_in_matrix(matrix, i + amount));
	}
	for (int i = (4 - amount); i < 4; i++) {
		set_line_in_matrix(matrix, i, 0x00);
	}
}

void move_matrix_left(char matrix[2], char amount) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4 - amount; j++) {
			set_bit_in_matrix(matrix, j, i, get_bit_in_matrix(matrix, j + amount, i));
		}
		for (int j = (4 - amount); j < 4; j++) {
			set_bit_in_matrix(matrix, j, i, 0x00);
		}
	}
}

void rotate_structure(char **result, char *base_structure, char rotation) {
	if (rotation == 0) {
		(*result)[0] = base_structure[0];
		(*result)[1] = base_structure[1];
	}

	for (int i = 0; i < 4; i++) {
		char current_line = get_line_in_matrix(base_structure, i);
		for (int j = 0; j < 4; j++) {
			set_bit_in_matrix((*result), i, j, (current_line & (1 << j)) >> j);
		}
	}

	if (rotation - 1 != 0) {
		char *new_base_structure = malloc(2 * sizeof(char));
		new_base_structure[0] = (*result)[0];
		new_base_structure[1] = (*result)[1];
		rotate_structure(result, new_base_structure, rotation - 1);
		free(new_base_structure);
	}
}

void align_and_rotate_structure(char **result, char *base_structure, char rotation) {
	rotate_structure(result, base_structure, rotation);
	Coordinates min_coordinates = get_min_coordinates(*result);
	if (min_coordinates.y != 0) {
		move_matrix_up(*result, min_coordinates.y);
	}
	if (min_coordinates.x != 0) {
		move_matrix_left(*result, min_coordinates.x);
	}
}

char *get_tetrimino_matrix(unsigned int tetrimino_type_id, unsigned char orientation) {
	char *matrix = malloc(2 * sizeof(char));
	align_and_rotate_structure(&matrix, tetriminos_types[tetrimino_type_id].structure, orientation);
	return matrix;
}

void render_tetrimino(int x, int y, int tetrimino_id, unsigned char orientation, bool preview) {
	char *rotated_structure = malloc(2 * sizeof(char));
	align_and_rotate_structure(&rotated_structure, tetriminos_types[tetrimino_id].structure, orientation);

	attron(COLOR_PAIR(tetriminos_types[tetrimino_id].color));
	for (int i = 0; i < 4; i++) {
		char current_line = (rotated_structure[((unsigned int) (i / 2))] >> (((i + 1) % 2) * 4)) & 0x0F;
		for (int j = 0; j < 4; j++) {
			if ((current_line >> j) & 0x01) {
				if (preview) {
					mvaddstr(y + i, x + ((3 - j) * 2), SHADOW_BLOCK SHADOW_BLOCK);
				} else {
					mvaddstr(y + i, x + ((3 - j) * 2), BLOCK BLOCK);
				}
			} else {
				//mvaddstr(y + i, (x + (3 - j))*2, "..");
			}
		}
	}
	free(rotated_structure);
	attroff(COLOR_PAIR(tetriminos_types[tetrimino_id].color));
}
