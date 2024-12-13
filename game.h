#include <stdbool.h>

#include "render.h"

typedef struct {
	unsigned char tetrimino_type_id;
	Coordinates coordinates;
	unsigned char orientation;
} FallingTetrimino;

bool does_overlap(char *matrix_1, char *matrix_2);
