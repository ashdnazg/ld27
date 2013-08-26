#ifndef __FONT_H__
#define __FONT_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct font_manager_s font_manager_t;

#include "int_list.h"
#include "game.h"
#include "video.h"

#define X_SPACING 1
#define Y_SPACING 1

#define TEXT_DEPTH 1000

struct font_manager_s {
    sprite_t **letters;    
    unsigned int num_letters;
    unsigned int size_x;
    unsigned int size_y;
};
    
font_manager_t * font_manager_new(void);
void font_manager_free(font_manager_t *f_manager);

void font_manager_load_font(game_t *game, font_manager_t *f_manager, const char *font_path, unsigned int size_x, unsigned int size_y);

renderable_t **font_manager_print(game_t *game, font_manager_t *f_manager, const char *text, int x, int y, unsigned int columns, int depth);
#ifdef __cplusplus
}
#endif

#endif 
