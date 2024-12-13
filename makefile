all:
	rm -f tetris-cli
	gcc -o tetris-cli -g main.c colors.c render.c tetrimino.c board.c utils.c game.c preview.c storage.c score.c -lncurses
