#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ncurses.h>

char kbhit(void) {
	int ch = getch();

	if (ch != ERR) {
		ungetch(ch);
		return 1;
	}
	return 0;
}

long getCurrentTimeMillis() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;
}


int generateRandomNumber(int min, int max) {
	int randomNumber = (rand() % (max - min)) + min;

	return randomNumber;
}

Coordinates new_coordinates(int x, int y) {
	Coordinates coordinates = {x, y};
	return coordinates;
}
