typedef struct {
	int x;
	int y;
} Coordinates;


char kbhit(void);

long getCurrentTimeMillis();

int generateRandomNumber(int min, int max);

Coordinates new_coordinates(int x, int y);
