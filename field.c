#include "field.h"
#include "game.h"
#include "video.h"
#include "macros.h"

void init_field(game_t *game, int first_x, int first_y) {
    int i, j, num_cols;
    int x = first_x;
    int y = first_y;
    for (i = 0;i <= NUM_ROWS; ++i) {
        num_cols = MIN(MIN(i,NUM_ROWS - i + 1), MAX_COLS);
        for (j = 0; j < num_cols; ++j) {
            render_manager_create_renderable(game->r_manager, asset_manager_get(game->sprites, "grass"), x, y, FIELD_DEPTH);
            x += COL_WIDTH;
        }
        x -= COL_WIDTH * num_cols + (COL_WIDTH / 2) * (NUM_ROWS - i < MAX_COLS ? -1 : 1);
        y += ROW_HEIGHT;
    }
}