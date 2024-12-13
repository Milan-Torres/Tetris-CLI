#include "storage.h"
#include "board.h"
#include "score.h"

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

bool running = true;

float SECONDS_BETWEEN_TICKS = 0.85f;

char next_falling_piece_id = 0;

bool has_stashed_this_turn = false;
char stashed_piece_id = -1;

FallingTetrimino current_falling_piece;

void trigger_end_game() {
	running = false;
}

void render_next_piece() {
	Coordinates render_at = new_coordinates(get_render_origin().x + BOARD_WIDTH * 2 + 3, get_render_origin().y + 2);
	if (next_falling_piece_id == I_TETRIMINO) {
		render_at.x -= 1;
	}
	if (next_falling_piece_id == O_TETRIMINO) {
		render_at.x += 1;
	}

	render_tetrimino(render_at.x, render_at.y, next_falling_piece_id, 0, false);
}

void render_stashed_piece() {
	if (stashed_piece_id == -1)
		return;

	Coordinates render_at = new_coordinates(get_render_origin().x - 9, get_render_origin().y + 2);
	if (stashed_piece_id == O_TETRIMINO) {
		render_at.x += 1;
	}
	if (stashed_piece_id == I_TETRIMINO) {
		render_at.x -= 1;
	}

	render_tetrimino(render_at.x, render_at.y, stashed_piece_id, 0, false);
}

void render_preview() {
	char render_at = get_fell_tetrimino_y(current_falling_piece);
	render_tetrimino(get_render_origin().x + current_falling_piece.coordinates.x + BOARD_WIDTH % 2,
					 get_render_origin().y + render_at, current_falling_piece.tetrimino_type_id,
					 current_falling_piece.orientation, true);
}

void render() {
	render_board();
	render_next_piece();
	render_stashed_piece();
	render_preview();
	render_scores();
	render_tetrimino(get_render_origin().x + current_falling_piece.coordinates.x + BOARD_WIDTH % 2,
					 get_render_origin().y + current_falling_piece.coordinates.y,
					 current_falling_piece.tetrimino_type_id, current_falling_piece.orientation, false);
	refresh();
}

bool move_to(char x, char y, char orientation, bool last_push) {
	char has_moved = false;

	char *next_pos_board = get_board_4x4_matrix_at((int) ((current_falling_piece.coordinates.x + x) / 2),
												   current_falling_piece.coordinates.y + y);
	char *matrix_render = get_tetrimino_matrix(current_falling_piece.tetrimino_type_id,
											   (unsigned char) ((current_falling_piece.orientation + orientation) % 4));

	if (!does_overlap(next_pos_board, matrix_render)) {
		current_falling_piece.coordinates.x += x;
		current_falling_piece.coordinates.y += y;
		current_falling_piece.orientation = (current_falling_piece.orientation + orientation) % 4;
		render();
		has_moved = true;
	}

	free(next_pos_board);
	free(matrix_render);

	char iterator = 0;
	while (!last_push && !has_moved && y == 0 && iterator < 6) {
		char offset = ((unsigned char) (iterator / 2) + 1) * 2 * ((iterator % 2) * -1); //2, -2, 4, -4, ...
		has_moved = move_to(x + offset, y, orientation, true);
		iterator++;
	}

	if (has_moved && !last_push) {
		render();
	}
	return has_moved;
}

void create_a_new_falling_piece() {
	current_falling_piece.orientation = 0;
	current_falling_piece.coordinates = new_coordinates(BOARD_WIDTH - 2, 0);
	current_falling_piece.tetrimino_type_id = next_falling_piece_id;

	next_falling_piece_id = generateRandomNumber(0, TETRIMINOS_NUMBER);

	char *tetrimino_matrix = get_tetrimino_matrix(current_falling_piece.tetrimino_type_id,
												  current_falling_piece.orientation);
	char *board_matrix = get_board_4x4_matrix_at(current_falling_piece.coordinates.x / 2,
												 current_falling_piece.coordinates.y);

	if (does_overlap(tetrimino_matrix, board_matrix)) {
		free(tetrimino_matrix);
		free(board_matrix);
		trigger_end_game();
	}
}

void trigger_end_fall() {
	score += 1;
	char *tetrimino_matrix = get_tetrimino_matrix(current_falling_piece.tetrimino_type_id,
												  current_falling_piece.orientation);

	bool filled[] = {false, false, false, false};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (get_bit_in_matrix(tetrimino_matrix, i, j)) {
				set_pixel_at((int) ((current_falling_piece.coordinates.x / 2) + i),
							 current_falling_piece.coordinates.y + j,
							 get_tetrimino_type_by_id(current_falling_piece.tetrimino_type_id).color);
			}
		}
	}

	for (int j = 0; j < 4; j++) {
		if (is_line_full(current_falling_piece.coordinates.y + j)) {
			filled[j] = true;
			fall_board_on(current_falling_piece.coordinates.y + j);
		}
	}

	unsigned char filled_lines_number = 0;

	for (int i = 0; i < 4; i++) {
		if (filled[i]) {
			filled_lines_number += 1;
		}
	}

	add_clearing_score(filled_lines_number);

	free(tetrimino_matrix);
	create_a_new_falling_piece();
	has_stashed_this_turn = false;
}

void fall_piece() {
	if (!move_to(0, 1, 0, false)) {
		trigger_end_fall();
	}
	render();
}

void instant_fall() {
	current_falling_piece.coordinates.y = get_fell_tetrimino_y(current_falling_piece);
	score += 1;
	trigger_end_fall();
	render();
}

void stash_piece() {
	if (!has_stashed_this_turn) {
		has_stashed_this_turn = true;

		if (stashed_piece_id == -1) {
			stashed_piece_id = current_falling_piece.tetrimino_type_id;
			create_a_new_falling_piece();
			render();
			return;
		}

		char tmp_stashed_piece_id = stashed_piece_id;
		stashed_piece_id = current_falling_piece.tetrimino_type_id;
		current_falling_piece.coordinates.x = BOARD_WIDTH;
		current_falling_piece.coordinates.y = 0;
		current_falling_piece.orientation = 0;
		current_falling_piece.tetrimino_type_id = tmp_stashed_piece_id;
		render();
	}
}

void key_management() {
	if (kbhit()) {
		switch (getch()) {
			case 'q':
				move_to(0, 0, 1, false);
				break;
			case 'l':
				current_falling_piece.tetrimino_type_id =
						(char) (current_falling_piece.tetrimino_type_id + 1) % TETRIMINOS_NUMBER;
				render();
				break;
			case 'd':
				move_to(2, 0, 0, false);
				break;
			case 'a':
				move_to(-2, 0, 0, false);
				break;
			case 's':
				fall_piece();
				break;
			case 'w':
				instant_fall();
				break;
			case 'e':
				stash_piece();
				break;
			case 'p':
				running = false;
				break;
		}
	}
}

void setup_timer() {
	struct sigaction sa;
	struct itimerval timer;

	sa.sa_handler = &fall_piece;
	sigaction(SIGALRM, &sa, NULL);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = SECONDS_BETWEEN_TICKS * 10e5;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = SECONDS_BETWEEN_TICKS * 10e5;

	setitimer(ITIMER_REAL, &timer, NULL);
}

void init_game() {
	highscore = retrieve_highscore_from_disk();

	setlocale(LC_ALL, "");
	srand(time(NULL));

	initscr();
	noecho();
	curs_set(0);
	setup_colors();

	init_board();
	register_base_tetriminos_types();

	render();

	create_a_new_falling_piece();
	create_a_new_falling_piece();
	render();

	__attribute__((unused)) long start_time = getCurrentTimeMillis();
	setup_timer();
}

void end_game() {
	unregister_tetriminos_types();
	destroy_board();

	if (score > highscore) {
		save_highscore_on_disk(score);
	}

	curs_set(1);
	endwin();
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		SECONDS_BETWEEN_TICKS = 5 / atof(argv[1]);
	}
	init_game();
	signal(SIGINT, trigger_end_game);

	while (running) {
		key_management();
	}

	end_game();
	return 0;
}
