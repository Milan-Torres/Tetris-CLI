#define APP_DIR "tetris-cli"
#define HIGHSCORE_FILE "highscore"

void save_highscore_on_disk(unsigned long long int value);

unsigned long long int retrieve_highscore_from_disk();
