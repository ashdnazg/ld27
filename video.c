#include "video.h"
#include "mem_wrap.h"

#include <SDL2/SDL.h>
#include <assert.h>

#include "stb_image.h"
#include "int_list.h"
#include "macros.h"
#include "assets.h"
#include "tween.h"

animation_playback_t * animation_playback_new(renderable_t * renderable, animation_t * animation,
                                                unsigned int interval, bool loop) {
    animation_playback_t *playback = mem_alloc(sizeof(animation_playback_t));
    playback->interval = interval;
    playback->total_steps = 0;
    playback->renderable = renderable;
    playback->animation = animation;
    playback->loop = loop;
    link_init(&(playback->animation_playbacks_link));
    
    return playback;
}

void animation_playback_free(animation_playback_t * playback) {
    link_remove_from_list(&(playback->animation_playbacks_link));
    playback->renderable->animation_playback = NULL;
    playback->renderable->sprite = playback->renderable->default_sprite;
    mem_free(playback);
}

void animation_playback_animate(animation_playback_t * playback) {
    if (!(playback->loop) && (playback->total_steps >= playback->interval * playback->animation->num_frames)) {
        animation_playback_free(playback);
        return;
    }
    playback->renderable->sprite = playback->animation->frames[(playback->total_steps / playback->interval) %
                                                                playback->animation->num_frames];
    ++playback->total_steps;
}


renderable_t * renderable_new(sprite_t *default_sprite, int x, int y, int depth) {
    renderable_t *renderable = mem_alloc(sizeof(renderable_t));
    renderable->tweens = list_new(tween_t, tweens_link);
    renderable->sprite = default_sprite;
    renderable->default_sprite = default_sprite;
    renderable->x = x;
    renderable->y = y;
    renderable->depth = depth;
    renderable->scale = 1.0;
    renderable->angle = 0;
    renderable->center = NULL;
    renderable->flip = SDL_FLIP_NONE;
    renderable->animation_playback = NULL;
    link_init(&(renderable->renderables_link));
    return renderable;
}

void renderable_free(renderable_t *renderable) {
    if (renderable->animation_playback != NULL) {
        animation_playback_free(renderable->animation_playback);
    }
    list_for_each(renderable->tweens, tween_t *, tween) {
        tween_free(tween);
    }
    list_free(renderable->tweens);
    link_remove_from_list(&(renderable->renderables_link));
    mem_free(renderable);
}

animation_t * animation_new(sprite_t **frames, unsigned int num_frames) {
    animation_t *animation = mem_alloc(sizeof(animation_t));
    animation->num_frames = num_frames;
    animation->frames = mem_alloc(sizeof(sprite_t *) * num_frames);
    memcpy(animation->frames, frames, sizeof(sprite_t *) * num_frames);
    return animation;
}

void animation_free(animation_t *animation) {
    mem_free(animation->frames);
    mem_free(animation);
}

render_manager_t * render_manager_new(SDL_Renderer *renderer) {
    render_manager_t *r_manager = mem_alloc(sizeof(render_manager_t));
    r_manager->renderer = renderer;
    r_manager->renderables = list_new(renderable_t, renderables_link);
    r_manager->animation_playbacks = list_new(animation_playback_t, animation_playbacks_link);
    r_manager->textures = asset_manager_new((free_cb_t) SDL_DestroyTexture);
    r_manager->x_offset = 0;
    r_manager->y_offset = 0;
    return r_manager;
}

void render_manager_free(render_manager_t *r_manager) {
    list_for_each(r_manager->renderables, renderable_t *, renderable) {
        renderable_free(renderable);
    }
    list_free(r_manager->renderables);
    assert(list_head(r_manager->animation_playbacks) == NULL);
    list_free(r_manager->animation_playbacks);
    asset_manager_free(r_manager->textures);
    mem_free(r_manager);
}

void render_manager_clear(render_manager_t *r_manager) {
    list_for_each(r_manager->renderables, renderable_t *, renderable) {
        renderable_free(renderable);
    }
}

renderable_t * render_manager_create_renderable(render_manager_t *r_manager, sprite_t *default_sprite, int x, int y, int depth) {
    renderable_t *renderable = renderable_new(default_sprite, x , y, depth);
    list_insert_tail(r_manager->renderables, renderable);
    return renderable;
}

void render_manager_play_animation(render_manager_t *r_manager, renderable_t *renderable, 
                                    animation_t *animation, unsigned int interval, bool loop) {
    render_manager_stop_animation(r_manager, renderable);
    animation_playback_t * playback = animation_playback_new(renderable, animation, interval, loop);
    renderable->animation_playback = playback;
    list_insert_tail(r_manager->animation_playbacks, playback);
}

void render_manager_stop_animation(render_manager_t *r_manager, renderable_t *renderable) {
    if (renderable->animation_playback != NULL) {
        animation_playback_free(renderable->animation_playback);
    }
}

void render_manager_animate(render_manager_t *r_manager) {
    list_for_each(r_manager->animation_playbacks, animation_playback_t *, playback){
        animation_playback_animate(playback);
    }
}
int get_y(renderable_t *renderable) {
    return -(renderable->y + renderable->depth);
}
void render_manager_draw(render_manager_t *r_manager) {
    SDL_RenderClear(r_manager->renderer);
    list_sort(r_manager->renderables, (key_cb_t) get_y);
    list_for_each(r_manager->renderables, renderable_t *, renderable){
        draw_sprite(r_manager, renderable->sprite, renderable->x + r_manager->x_offset, renderable->y + r_manager->y_offset, 
                                renderable->scale, renderable->angle, renderable->center, renderable->flip);
    }
    SDL_RenderPresent(r_manager->renderer);
}


void exit_on_SDL_error(void * pt) {
    if(pt == NULL) {
        printf("SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void exit_on_stbi_error(void * pt) {
    if(pt == NULL) {
        printf("STBI Error: %s\n", stbi_failure_reason());
        exit(1);
    }
}

SDL_Texture * load_image(render_manager_t *r_manager, const char * path) {
    int im_w, im_h;
    unsigned char *image = NULL;
    SDL_Surface *bitmap = NULL;
    SDL_Texture *texture = asset_manager_get(r_manager->textures, path);
    if (texture != NULL) {
        return texture;
    }
    image = stbi_load(path, &im_w, &im_h, NULL, RGBA);
    exit_on_stbi_error(image);
    
    bitmap = SDL_CreateRGBSurfaceFrom(image, im_w, im_h, RGBA * 8, RGBA * im_w,
                                   RMASK, GMASK, BMASK, AMASK);
    exit_on_SDL_error(bitmap);
    texture = SDL_CreateTextureFromSurface(r_manager->renderer, bitmap);
    
    SDL_FreeSurface(bitmap);
    free(image);
    asset_manager_add(r_manager->textures, texture, path);
    return texture;
}

sprite_t * sprite_new(SDL_Texture *texture, int x, int y, int w, int h) {
    sprite_t *sprite = mem_alloc(sizeof(sprite_t));
    SDL_Rect *rect = mem_alloc(sizeof(SDL_Rect));
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    sprite->rect = rect;
    sprite->texture = texture;
    return sprite;
}

void sprite_free(sprite_t *sprite) {
    mem_free(sprite->rect);
    mem_free(sprite);
}

void draw_sprite(render_manager_t *r_manager, sprite_t *sprite, int x, int y, double scale, 
                 double angle, SDL_Point *center, SDL_RendererFlip flip) {
    SDL_Rect pos;
    pos.w = (int) (sprite->rect->w * scale);
    pos.h = (int) (sprite->rect->h * scale);
    pos.x = x + (sprite->rect->w - pos.w) / 2;
    pos.y = y + (sprite->rect->h - pos.h) / 2;
    
    //SDL_RenderCopy(r_manager->renderer, sprite->texture, sprite->rect, &pos);
    SDL_RenderCopyEx(r_manager->renderer, sprite->texture, sprite->rect, &pos, angle, center, flip);
}

void draw_image(render_manager_t *r_manager, SDL_Texture *texture, int x, int y) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
 
    SDL_RenderCopy(r_manager->renderer, texture, NULL, &pos);
}

sprite_t * load_sprite(render_manager_t *r_manager, const char * path) {
    int texture_width, texture_height;
    SDL_Texture *texture = load_image(r_manager, path);
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    return sprite_new(texture, 0, 0, texture_width, texture_height);
}

sprite_t ** load_sprite_sheet(render_manager_t *r_manager, const char * path, 
                        int spr_width, int spr_height, int padding, unsigned int *out_num_sprites) {
    SDL_Texture *texture = NULL;
    sprite_t ** sprites = NULL;
    int num_rows, num_cols, texture_width, texture_height, i;
    
    texture = load_image(r_manager, path);
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    num_cols = ((texture_width - padding) / (spr_width + padding));
    num_rows = ((texture_height - padding) / (spr_height + padding));
    *out_num_sprites = num_rows * num_cols;

    sprites = mem_alloc(sizeof(sprite_t *) * (*out_num_sprites));
    
    for (i = 0;i < (*out_num_sprites); ++i) {
        sprites[i] = sprite_new(texture, (i % num_cols) * (spr_width + padding) + padding, 
                                        (i / num_cols) * (spr_height + padding) + padding, spr_width, spr_height);
    }
    
    return sprites;
}
