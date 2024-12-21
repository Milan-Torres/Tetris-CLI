#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>

#include "board.h"

char *board;

const int BOARD_SIZE = (BOARD_HEIGHT * BOARD_WIDTH) / 2; //each pixel of the board is made of 3 bits

void init_board() {
	board = malloc(BOARD_SIZE * sizeof(char));
	for (int i = 0; i < BOARD_SIZE; i++) {
		board[i] = 0;
	}
}

void destroy_board() {
	free(board);
}

char get_pixel_at(int x, int y) {
	return (((board[(unsigned int) (((y * BOARD_WIDTH) / 2) + ((unsigned int) (x / 2)))]) >> (((x + 1) % 2) * 4)) &
			0x0F);
}

void set_pixel_at(int x, int y, char pixel) {
	unsigned int index = (unsigned int) (((y * BOARD_WIDTH) / 2)) + ((unsigned int) (x / 2));

	board[index] = (board[index] & (0x0F << ((x % 2) * 4))) | ((pixel & 0x0F) << (((x + 1) % 2) * 4));
}

char *get_board_4x4_matrix_at(int x, int y) {
	char *matrix = malloc(sizeof(char) * 2);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (x + i < 0 || x + i >= BOARD_WIDTH || y + j < 0 || y + j >= BOARD_HEIGHT) {
				set_bit_in_matrix(matrix, i, j, 1);
				continue;
			}
#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
			set_bit_in_matrix(matrix, i, j, !!(get_pixel_at(x + i, y + j)));
#pragma clang diagnostic pop
		}
	}

	return matrix;
}

bool is_line_full(int y) {
	bool full = true;
	for (int i = 0; i < BOARD_WIDTH; i++) {
		if (get_pixel_at(i, y) == 0) {
			full = false;
			break;
		}
	}
	return full;
}

void fall_board_on(int y) {
	for (int j = y; j >= 1; j--) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			set_pixel_at(i, j, get_pixel_at(i, j - 1));
		}
	}
}

void render_ui() {
	Coordinates render_origin_cache = get_render_origin(); //avoid  reevaluating the same condition over and over

	for (int i = 0; i < BOARD_WIDTH; i++) {
		mvaddstr(render_origin_cache.y + BOARD_HEIGHT, render_origin_cache.x + i * 2, "━━");
		mvaddstr(render_origin_cache.y - 1, render_origin_cache.x + i * 2, "━━");
	}
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		mvaddstr(render_origin_cache.y + i, render_origin_cache.x - 1, "┃");
		mvaddstr(render_origin_cache.y + i, render_origin_cache.x + BOARD_WIDTH * 2, "┃");
	}

	mvaddstr(render_origin_cache.y - 1, render_origin_cache.x - 1, "┏");
	mvaddstr(render_origin_cache.y - 1, render_origin_cache.x + BOARD_WIDTH * 2, "┳");
	mvaddstr(render_origin_cache.y + BOARD_HEIGHT, render_origin_cache.x - 1, "┗");
	mvaddstr(render_origin_cache.y + BOARD_HEIGHT, render_origin_cache.x + BOARD_WIDTH * 2, "┛");

	int x = render_origin_cache.x + BOARD_WIDTH * 2;

	mvaddstr(render_origin_cache.y + 4, x, "┣");
	mvaddstr(render_origin_cache.y + 1, x, "┣");

	for (int i = 1; i < 11; i++) {
		mvaddstr(render_origin_cache.y - 1, x + i, "━");
		mvaddstr(render_origin_cache.y + 4, x + i, "━");
		mvaddstr(render_origin_cache.y + 1, x + i, "━");
	}

	x += 11;

	mvaddstr(render_origin_cache.y - 1, x, "┓");
	mvaddstr(render_origin_cache.y + 0, x, "┃");
	mvaddstr(render_origin_cache.y + 1, x, "┫");
	mvaddstr(render_origin_cache.y + 2, x, "┃");
	mvaddstr(render_origin_cache.y + 3, x, "┃");
	mvaddstr(render_origin_cache.y + 4, x, "┛");

	for (int i = 1; i < 11; i++) {
		for (int j = 2; j < 4; j++) {
			mvaddstr(render_origin_cache.y + j, render_origin_cache.x + BOARD_WIDTH * 2 + i, " ");
		}
	}

	x = render_origin_cache.x - 12;

	for (int i = 0; i < 12; i++) {
		mvaddstr(render_origin_cache.y - 1, x + i, "━");
		mvaddstr(render_origin_cache.y + 1, x + i, "━");
		mvaddstr(render_origin_cache.y + 4, x + i, "━");
	}

	mvaddstr(render_origin_cache.y - 1, render_origin_cache.x - 1, "┳");
	mvaddstr(render_origin_cache.y + 1, render_origin_cache.x - 1, "┫");
	mvaddstr(render_origin_cache.y + 4, render_origin_cache.x - 1, "┫");

	mvaddstr(render_origin_cache.y - 1, x, "┏");
	mvaddstr(render_origin_cache.y + 0, x, "┃");
	mvaddstr(render_origin_cache.y + 1, x, "┣");
	mvaddstr(render_origin_cache.y + 2, x, "┃");
	mvaddstr(render_origin_cache.y + 3, x, "┃");
	mvaddstr(render_origin_cache.y + 4, x, "┗");

	for (int i = 1; i < 11; i++) {
		mvaddstr(render_origin_cache.y, x + i, " ");
		for (int j = 0; j < 2; j++) {
			mvaddstr(render_origin_cache.y + 2 + j, x + i, " ");
		}
	}

	mvaddstr(render_origin_cache.y - 1, render_origin_cache.x + BOARD_WIDTH * 2 + 3, "Score");
	mvaddstr(render_origin_cache.y + 1, render_origin_cache.x + BOARD_WIDTH * 2 + 4, "Next");
	mvaddstr(render_origin_cache.y - 1, render_origin_cache.x - 11, "Highscore");
	mvaddstr(render_origin_cache.y + 1, render_origin_cache.x - 10, "Stashed");
}

void render_board() {
	Coordinates render_origin_cache = get_render_origin();

	render_ui();
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			char pixel = get_pixel_at(i, j);
			if (pixel == 0) {
				mvaddstr(render_origin_cache.y + j, render_origin_cache.x + (i * 2), "  ");
				continue;
			}
			attron(COLOR_PAIR(pixel));
			mvaddstr(render_origin_cache.y + j, render_origin_cache.x + (i * 2), BLOCK BLOCK);
			attroff(COLOR_PAIR(pixel));
		}
	}
}
