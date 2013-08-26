#include "font.h"
#include "video.h"
#include "game.h"
#include "mem_wrap.h"
#include "string.h"

font_manager_t * font_manager_new(void) {
    font_manager_t *f_manager = mem_alloc(sizeof(font_manager_t));
    f_manager->letters = NULL;
    f_manager->num_letters = 0;
    f_manager->size_x = 0;
    f_manager->size_y = 0;
    return f_manager;
}

void font_manager_free(font_manager_t *f_manager) {
    int i;
    if (f_manager->letters != NULL) {
        for(i = 0;i < f_manager->num_letters; ++i) {
            sprite_free(f_manager->letters[i]);
        }
        mem_free(f_manager->letters);
    }
    mem_free(f_manager);
}

void font_manager_load_font(game_t *game, font_manager_t *f_manager, const char *font_path, unsigned int size_x, unsigned int size_y) {
    f_manager->letters = load_sprite_sheet(game->r_manager, font_path, size_x, size_y, 0, &(f_manager->num_letters));
    f_manager->size_x = size_x;
    f_manager->size_y = size_y;
}

renderable_t **font_manager_print(game_t *game, font_manager_t *f_manager, const char *text, int x, int y, unsigned int columns, int depth) {
    int temp_x = x;
    int temp_y = y;
    int i;
    int len = strlen(text);
    renderable_t **renderables = mem_alloc(len * sizeof(renderable_t *));
    
    for (i = 0;i < len;++i){
        temp_x = x + (f_manager->size_x + X_SPACING) * (i % columns);
        temp_y = y + (f_manager->size_y + Y_SPACING) * (i / columns);
        renderables[i] = render_manager_create_renderable(game->r_manager, f_manager->letters[text[i]], temp_x, temp_y, depth);
    }
    return renderables;
}