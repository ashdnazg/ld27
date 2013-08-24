#ifndef __FIELD_H__
#define __FIELD_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "game.h"

#define COL_WIDTH 20
#define ROW_HEIGHT 6
#define NUM_ROWS 75
#define MAX_COLS 24
#define FIELD_DEPTH -100

void init_field(game_t *game, int first_x, int first_y);

#ifdef __cplusplus
}
#endif

#endif
