#include "utils.h"

#include <wchar.h>
#include <ncurses.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define BLOCK "\u2588"
#define SHADOW_BLOCK "\u2592"

Coordinates get_render_origin();
