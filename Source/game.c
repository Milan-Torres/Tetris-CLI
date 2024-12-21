#include "game.h"

bool does_overlap(char *matrix_1, char *matrix_2) {
	//checks if two 4x4 binary matrices overlap
	for (int i = 0; i < 2; i++) {
		if (matrix_1[i] & matrix_2[i]) {
			return true;
		}
	}

	return false;
}
