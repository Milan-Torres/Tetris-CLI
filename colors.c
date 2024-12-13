#include <ncurses.h>
#include "colors.h"

void setup_colors() {
	start_color();
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
}
