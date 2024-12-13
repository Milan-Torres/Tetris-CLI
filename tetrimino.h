#include "preview.h"
#include "colors.h"

#define I_TETRIMINO 0
#define O_TETRIMINO 1
#define T_TETRIMINO 2
#define J_TETRIMINO 3
#define L_TETRIMINO 4
#define S_TETRIMINO 5
#define Z_TETRIMINO 6

#define TETRIMINOS_NUMBER 7

typedef struct {
	//tetriminos are 4x4 matrices (so 16 bits = 2 chars), the first four bytes are the first row, the next four the second, etc...
	//for example the straight bar in tetris would look something like that:
	//1000
	//1000
	//1000
	//1000
	char *structure;
	int color;
} TetriminoType;

TetriminoType new_tetrimino_type(int structure_argument, int color);

TetriminoType get_tetrimino_type_by_id(unsigned int tetrimino_type_id);

void set_bit_in_matrix(char matrix[2], unsigned char x, unsigned char y, char value);

char get_bit_in_matrix(char matrix[2], unsigned char x, unsigned char y);

char get_line_in_matrix(char matrix[2], unsigned char y);

void set_line_in_matrix(char matrix[2], unsigned char y, char line);

void destroy_tetrimino_type(TetriminoType *tetrimino);

void register_base_tetriminos_types();

void unregister_tetriminos_types();

char *get_tetrimino_matrix(unsigned int tetrimino_type_id, unsigned char orientation);

void render_tetrimino(int x, int y, int tetrimino_id, unsigned char orientation, bool preview);
