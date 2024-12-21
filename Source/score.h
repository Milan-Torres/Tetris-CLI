#include <ncurses.h>

extern const int scores_table[];
extern unsigned long long int score;
extern unsigned long long int highscore;

void render_highscore();

void render_score();

void render_scores();

void add_clearing_score(unsigned char filled_lines);
