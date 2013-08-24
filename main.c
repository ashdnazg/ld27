#include "config.h"
#include "video.h"
#include "sound.h"
#include "tween.h"
#include "game.h"
#include "mem_wrap.h"
#include "macros.h"
#include "tween.h"
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define SKIP_THRESHOLD 5
#define MAX_SKIP 5


bool handle_input(game_t *game){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return TRUE;
        }
        if (e.type == SDL_KEYDOWN) {
            game_key_down(game, e.key.keysym.scancode);
        } else if (e.type == SDL_KEYUP) {
            game_key_up(game, e.key.keysym.scancode);
        }
    }
    return FALSE;
}

void load_assets(game_t *game){
    sprite_t **sprites = mem_alloc(16 *sizeof(sprite_t *));
    animation_t * anim = NULL;
    int num_sprites, i;
    
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "grass.png"), "grass");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_s00.png"), "streaker_run_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_s01.png"), "streaker_run_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_s00.png"), "streaker_wiggle_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_s01.png"), "streaker_wiggle_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_s02.png"), "streaker_wiggle_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_stand_s.png"), "streaker_stand_s");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_run_s00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_run_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "streaker_run_s");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_wiggle_s00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_wiggle_s00");
    sprites[3] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    sprites[4] = asset_manager_get(game->sprites, "streaker_wiggle_s02");
    sprites[5] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    sprites[6] = asset_manager_get(game->sprites, "streaker_wiggle_s02");
    sprites[7] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    asset_manager_add(game->animations, animation_new(sprites, 8), "streaker_wiggle_s");
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "ambient.wav"), "ambient");
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "wiggle.wav"), "wiggle");
    
    mem_free(sprites);
}

int main(int argc, char* argv[]){
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    SDL_Surface *bmp = NULL;
    
    render_manager_t * r_manager = NULL;
    renderable_t *renderables[4];
    
    Uint32 next_frame_time = 0;
    Sint32 time_to_next = 0;
    int frames_skipped = 0;
    int frames_this_second = 0;
    int last_time = 0;
    bool draw = TRUE;
    game_t *game;

    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE) == -1){
        printf("SDLInit Error:%s", SDL_GetError());
        return 1;
    }
    win = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH * WINDOW_SCALE, GAME_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
    exit_on_SDL_error(win);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    exit_on_SDL_error(ren);
    SDL_RenderSetLogicalSize(ren, GAME_WIDTH, GAME_HEIGHT);
    
    mem_wrap_init();
    
    r_manager = render_manager_new(ren);
    
    game = game_new(r_manager);
    load_assets(game);
    game_init(game);
    
    next_frame_time = SDL_GetTicks();
    last_time = SDL_GetTicks();
    while (1) {
        if(handle_input(game) || !(game->running)) {
            break;
        }
        
        time_to_next = (Sint32)(next_frame_time - SDL_GetTicks());

        if(time_to_next <= 0) {
            next_frame_time += STEP_INTERVAL;
            draw = TRUE;
            if(time_to_next < -(STEP_INTERVAL * SKIP_THRESHOLD))
            {
                if(frames_skipped >= MAX_SKIP) {
                    next_frame_time = SDL_GetTicks();
                    frames_skipped = 0;
                } else {
                    ++frames_skipped;
                    draw = FALSE;
                    printf("\nskipped");
                }
            } else {
                frames_skipped = 0;
            }
            game_step(game, draw);
            ++frames_this_second;
            if ((SDL_GetTicks() - last_time) >= 1000){
                printf("\nframes: %d", frames_this_second);
                frames_this_second = 0;
                last_time += 1000;
            }
        } else {
            SDL_Delay(1);
        }
    }
    game_free(game);
    render_manager_free(r_manager);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();
    mem_wrap_print_mallocs();
    return 0;
}
