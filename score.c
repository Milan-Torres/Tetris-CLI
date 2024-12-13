#include "score.h"

#include "render.h"

const int scores_table[] = {40, 100, 300,1200};
//1: One line clear, 2: Two lines clear, 3: Three lines clear, 4: Four lines clear

unsigned long long int score = 0;
unsigned long long int highscore = 0;

unsigned int combo_streak = 0;

void render_highscore() {
	mvprintw(get_render_origin().y, get_render_origin().x - 9, "%06llu", (highscore > score ? highscore : score));
}

void render_score() {
	mvprintw(get_render_origin().y, get_render_origin().x + BOARD_WIDTH * 2 + 3, "%06llu", score);
}

void add_clearing_score(unsigned char filled_lines) {
	if (filled_lines == 0) {
		combo_streak = 0;
		return;
	}

	score += scores_table[filled_lines - 1] * (combo_streak + 1);

	combo_streak += filled_lines;
}

void render_scores() {
	render_highscore();
	render_score();
}