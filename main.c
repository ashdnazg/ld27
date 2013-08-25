#include "config.h"
#include "video.h"
#include "sound.h"
#include "tween.h"
#include "game.h"
#include "font.h"
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
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "field.png"), "field");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "logo.png"), "logo");
    
    
    //SOUTH
    
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_s00.png"), "streaker_run_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_s01.png"), "streaker_run_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_s00.png"), "streaker_wiggle_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_s01.png"), "streaker_wiggle_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_s02.png"), "streaker_wiggle_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_s00.png"), "streaker_jump_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_s01.png"), "streaker_jump_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_s02.png"), "streaker_jump_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_stand_s.png"), "streaker_stand_s");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_run_s00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_run_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "streaker_run_s");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_jump_s00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_jump_s01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_jump_s02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "streaker_jump_s");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_wiggle_s00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_wiggle_s00");
    sprites[3] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    sprites[4] = asset_manager_get(game->sprites, "streaker_wiggle_s02");
    sprites[5] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    sprites[6] = asset_manager_get(game->sprites, "streaker_wiggle_s02");
    sprites[7] = asset_manager_get(game->sprites, "streaker_wiggle_s01");
    asset_manager_add(game->animations, animation_new(sprites, 8), "streaker_wiggle_s");
    
    
    //NORTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_n00.png"), "streaker_run_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_n01.png"), "streaker_run_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_n00.png"), "streaker_wiggle_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_n01.png"), "streaker_wiggle_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_n02.png"), "streaker_wiggle_n02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_n00.png"), "streaker_jump_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_n01.png"), "streaker_jump_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_n02.png"), "streaker_jump_n02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_stand_n.png"), "streaker_stand_n");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_run_n00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_run_n01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "streaker_run_n");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_jump_n00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_jump_n01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_jump_n02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "streaker_jump_n");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_wiggle_n00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_wiggle_n01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_wiggle_n00");
    sprites[3] = asset_manager_get(game->sprites, "streaker_wiggle_n01");
    sprites[4] = asset_manager_get(game->sprites, "streaker_wiggle_n02");
    sprites[5] = asset_manager_get(game->sprites, "streaker_wiggle_n01");
    sprites[6] = asset_manager_get(game->sprites, "streaker_wiggle_n02");
    sprites[7] = asset_manager_get(game->sprites, "streaker_wiggle_n01");
    asset_manager_add(game->animations, animation_new(sprites, 8), "streaker_wiggle_n");
    
    //WEST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_w00.png"), "streaker_run_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_w01.png"), "streaker_run_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_w00.png"), "streaker_wiggle_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_w01.png"), "streaker_wiggle_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_w02.png"), "streaker_wiggle_w02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_w00.png"), "streaker_jump_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_w01.png"), "streaker_jump_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_w02.png"), "streaker_jump_w02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_stand_w.png"), "streaker_stand_w");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_run_w00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_run_w01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "streaker_run_w");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_jump_w00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_jump_w01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_jump_w02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "streaker_jump_w");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_wiggle_w00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_wiggle_w01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_wiggle_w00");
    sprites[3] = asset_manager_get(game->sprites, "streaker_wiggle_w01");
    sprites[4] = asset_manager_get(game->sprites, "streaker_wiggle_w02");
    sprites[5] = asset_manager_get(game->sprites, "streaker_wiggle_w01");
    sprites[6] = asset_manager_get(game->sprites, "streaker_wiggle_w02");
    sprites[7] = asset_manager_get(game->sprites, "streaker_wiggle_w01");
    asset_manager_add(game->animations, animation_new(sprites, 8), "streaker_wiggle_w");
    
    //EAST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_e00.png"), "streaker_run_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_run_e01.png"), "streaker_run_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_e00.png"), "streaker_wiggle_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_e01.png"), "streaker_wiggle_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_wiggle_e02.png"), "streaker_wiggle_e02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_e00.png"), "streaker_jump_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_e01.png"), "streaker_jump_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_jump_e02.png"), "streaker_jump_e02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "streaker_stand_e.png"), "streaker_stand_e");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_run_e00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_run_e01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "streaker_run_e");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_jump_e00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_jump_e01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_jump_e02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "streaker_jump_e");
    
    sprites[0] = asset_manager_get(game->sprites, "streaker_wiggle_e00");
    sprites[1] = asset_manager_get(game->sprites, "streaker_wiggle_e01");
    sprites[2] = asset_manager_get(game->sprites, "streaker_wiggle_e00");
    sprites[3] = asset_manager_get(game->sprites, "streaker_wiggle_e01");
    sprites[4] = asset_manager_get(game->sprites, "streaker_wiggle_e02");
    sprites[5] = asset_manager_get(game->sprites, "streaker_wiggle_e01");
    sprites[6] = asset_manager_get(game->sprites, "streaker_wiggle_e02");
    sprites[7] = asset_manager_get(game->sprites, "streaker_wiggle_e01");
    asset_manager_add(game->animations, animation_new(sprites, 8), "streaker_wiggle_e");
    
    
    //NORTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_n00.png"),    "security_run_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_n01.png"),    "security_run_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_n00.png"),   "security_jump_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_n01.png"),   "security_jump_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_n02.png"),   "security_jump_n02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_stand_n.png"),    "security_stand_n");
    
    sprites[0] = asset_manager_get(game->sprites, "security_run_n00");
    sprites[1] = asset_manager_get(game->sprites, "security_run_n01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "security_run_n");
    
    sprites[0] = asset_manager_get(game->sprites, "security_jump_n00");
    sprites[1] = asset_manager_get(game->sprites, "security_jump_n01");
    sprites[2] = asset_manager_get(game->sprites, "security_jump_n02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "security_jump_n");
    
    //SOUTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_s00.png"),    "security_run_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_s01.png"),    "security_run_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_s00.png"),   "security_jump_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_s01.png"),   "security_jump_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_s02.png"),   "security_jump_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_stand_s.png"),    "security_stand_s");
    
    sprites[0] = asset_manager_get(game->sprites, "security_run_s00");
    sprites[1] = asset_manager_get(game->sprites, "security_run_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "security_run_s");
    
    sprites[0] = asset_manager_get(game->sprites, "security_jump_s00");
    sprites[1] = asset_manager_get(game->sprites, "security_jump_s01");
    sprites[2] = asset_manager_get(game->sprites, "security_jump_s02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "security_jump_s");
    
    //WEST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_w00.png"),    "security_run_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_w01.png"),    "security_run_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_w00.png"),   "security_jump_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_w01.png"),   "security_jump_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_w02.png"),   "security_jump_w02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_stand_w.png"),    "security_stand_w");
    
    sprites[0] = asset_manager_get(game->sprites, "security_run_w00");
    sprites[1] = asset_manager_get(game->sprites, "security_run_w01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "security_run_w");
    
    sprites[0] = asset_manager_get(game->sprites, "security_jump_w00");
    sprites[1] = asset_manager_get(game->sprites, "security_jump_w01");
    sprites[2] = asset_manager_get(game->sprites, "security_jump_w02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "security_jump_w");
    
    //EAST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_e00.png"),    "security_run_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_run_e01.png"),    "security_run_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_e00.png"),   "security_jump_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_e01.png"),   "security_jump_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_jump_e02.png"),   "security_jump_e02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "security_stand_e.png"),    "security_stand_e");
    
    sprites[0] = asset_manager_get(game->sprites, "security_run_e00");
    sprites[1] = asset_manager_get(game->sprites, "security_run_e01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "security_run_e");
    
    sprites[0] = asset_manager_get(game->sprites, "security_jump_e00");
    sprites[1] = asset_manager_get(game->sprites, "security_jump_e01");
    sprites[2] = asset_manager_get(game->sprites, "security_jump_e02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "security_jump_e");
    
    
    
    //NORTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_n00.png"),    "rplayer_run_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_n01.png"),    "rplayer_run_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_n00.png"),   "rplayer_jump_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_n01.png"),   "rplayer_jump_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_n02.png"),   "rplayer_jump_n02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_stand_n.png"),    "rplayer_stand_n");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_run_n00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_run_n01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "rplayer_run_n");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_jump_n00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_jump_n01");
    sprites[2] = asset_manager_get(game->sprites, "rplayer_jump_n02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "rplayer_jump_n");
    
    //SOUTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_s00.png"),    "rplayer_run_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_s01.png"),    "rplayer_run_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_s00.png"),   "rplayer_jump_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_s01.png"),   "rplayer_jump_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_s02.png"),   "rplayer_jump_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_stand_s.png"),    "rplayer_stand_s");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_run_s00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_run_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "rplayer_run_s");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_jump_s00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_jump_s01");
    sprites[2] = asset_manager_get(game->sprites, "rplayer_jump_s02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "rplayer_jump_s");
    
    //WEST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_w00.png"),    "rplayer_run_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_w01.png"),    "rplayer_run_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_w00.png"),   "rplayer_jump_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_w01.png"),   "rplayer_jump_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_w02.png"),   "rplayer_jump_w02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_stand_w.png"),    "rplayer_stand_w");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_run_w00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_run_w01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "rplayer_run_w");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_jump_w00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_jump_w01");
    sprites[2] = asset_manager_get(game->sprites, "rplayer_jump_w02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "rplayer_jump_w");
    
    //EAST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_e00.png"),    "rplayer_run_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_run_e01.png"),    "rplayer_run_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_e00.png"),   "rplayer_jump_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_e01.png"),   "rplayer_jump_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_jump_e02.png"),   "rplayer_jump_e02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "rplayer_stand_e.png"),    "rplayer_stand_e");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_run_e00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_run_e01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "rplayer_run_e");
    
    sprites[0] = asset_manager_get(game->sprites, "rplayer_jump_e00");
    sprites[1] = asset_manager_get(game->sprites, "rplayer_jump_e01");
    sprites[2] = asset_manager_get(game->sprites, "rplayer_jump_e02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "rplayer_jump_e");
    
    //NORTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_n00.png"),    "bplayer_run_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_n01.png"),    "bplayer_run_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_n00.png"),   "bplayer_jump_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_n01.png"),   "bplayer_jump_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_n02.png"),   "bplayer_jump_n02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_stand_n.png"),    "bplayer_stand_n");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_run_n00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_run_n01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "bplayer_run_n");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_jump_n00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_jump_n01");
    sprites[2] = asset_manager_get(game->sprites, "bplayer_jump_n02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "bplayer_jump_n");
    
    //SOUTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_s00.png"),    "bplayer_run_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_s01.png"),    "bplayer_run_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_s00.png"),   "bplayer_jump_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_s01.png"),   "bplayer_jump_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_s02.png"),   "bplayer_jump_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_stand_s.png"),    "bplayer_stand_s");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_run_s00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_run_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "bplayer_run_s");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_jump_s00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_jump_s01");
    sprites[2] = asset_manager_get(game->sprites, "bplayer_jump_s02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "bplayer_jump_s");
    
    //WEST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_w00.png"),    "bplayer_run_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_w01.png"),    "bplayer_run_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_w00.png"),   "bplayer_jump_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_w01.png"),   "bplayer_jump_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_w02.png"),   "bplayer_jump_w02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_stand_w.png"),    "bplayer_stand_w");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_run_w00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_run_w01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "bplayer_run_w");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_jump_w00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_jump_w01");
    sprites[2] = asset_manager_get(game->sprites, "bplayer_jump_w02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "bplayer_jump_w");
    
    //EAST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_e00.png"),    "bplayer_run_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_run_e01.png"),    "bplayer_run_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_e00.png"),   "bplayer_jump_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_e01.png"),   "bplayer_jump_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_jump_e02.png"),   "bplayer_jump_e02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "bplayer_stand_e.png"),    "bplayer_stand_e");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_run_e00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_run_e01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "bplayer_run_e");
    
    sprites[0] = asset_manager_get(game->sprites, "bplayer_jump_e00");
    sprites[1] = asset_manager_get(game->sprites, "bplayer_jump_e01");
    sprites[2] = asset_manager_get(game->sprites, "bplayer_jump_e02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "bplayer_jump_e");
    
        //NORTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_n00.png"),    "police_run_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_n01.png"),    "police_run_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_n00.png"),   "police_jump_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_n01.png"),   "police_jump_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_n02.png"),   "police_jump_n02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_n00.png"),    "police_aim_n00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_n01.png"),    "police_aim_n01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_stand_n.png"),    "police_stand_n");
    
    sprites[0] = asset_manager_get(game->sprites, "police_run_n00");
    sprites[1] = asset_manager_get(game->sprites, "police_run_n01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_run_n");
    
    sprites[0] = asset_manager_get(game->sprites, "police_jump_n00");
    sprites[1] = asset_manager_get(game->sprites, "police_jump_n01");
    sprites[2] = asset_manager_get(game->sprites, "police_jump_n02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "police_jump_n");
    
    
    sprites[0] = asset_manager_get(game->sprites, "police_aim_n00");
    sprites[1] = asset_manager_get(game->sprites, "police_aim_n01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_aim_n");
    
    //SOUTH
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_s00.png"),    "police_run_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_s01.png"),    "police_run_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_s00.png"),   "police_jump_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_s01.png"),   "police_jump_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_s02.png"),   "police_jump_s02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_s00.png"),    "police_aim_s00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_s01.png"),    "police_aim_s01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_stand_s.png"),    "police_stand_s");
    
    sprites[0] = asset_manager_get(game->sprites, "police_run_s00");
    sprites[1] = asset_manager_get(game->sprites, "police_run_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_run_s");
    
    sprites[0] = asset_manager_get(game->sprites, "police_jump_s00");
    sprites[1] = asset_manager_get(game->sprites, "police_jump_s01");
    sprites[2] = asset_manager_get(game->sprites, "police_jump_s02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "police_jump_s");
    
    
    sprites[0] = asset_manager_get(game->sprites, "police_aim_s00");
    sprites[1] = asset_manager_get(game->sprites, "police_aim_s01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_aim_s");
    
    //WEST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_w00.png"),    "police_run_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_w01.png"),    "police_run_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_w00.png"),   "police_jump_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_w01.png"),   "police_jump_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_w02.png"),   "police_jump_w02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_w00.png"),    "police_aim_w00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_w01.png"),    "police_aim_w01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_stand_w.png"),    "police_stand_w");
    
    sprites[0] = asset_manager_get(game->sprites, "police_run_w00");
    sprites[1] = asset_manager_get(game->sprites, "police_run_w01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_run_w");
    
    sprites[0] = asset_manager_get(game->sprites, "police_jump_w00");
    sprites[1] = asset_manager_get(game->sprites, "police_jump_w01");
    sprites[2] = asset_manager_get(game->sprites, "police_jump_w02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "police_jump_w");
    
    
    sprites[0] = asset_manager_get(game->sprites, "police_aim_w00");
    sprites[1] = asset_manager_get(game->sprites, "police_aim_w01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_aim_w");
    
    //EAST
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_e00.png"),    "police_run_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_run_e01.png"),    "police_run_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_e00.png"),   "police_jump_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_e01.png"),   "police_jump_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_jump_e02.png"),   "police_jump_e02");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_e00.png"),    "police_aim_e00");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_aim_e01.png"),    "police_aim_e01");
    asset_manager_add(game->sprites, load_sprite(game->r_manager, ASSETS_DIR "police_stand_e.png"),    "police_stand_e");
    
    sprites[0] = asset_manager_get(game->sprites, "police_run_e00");
    sprites[1] = asset_manager_get(game->sprites, "police_run_e01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_run_e");
    
    sprites[0] = asset_manager_get(game->sprites, "police_jump_e00");
    sprites[1] = asset_manager_get(game->sprites, "police_jump_e01");
    sprites[2] = asset_manager_get(game->sprites, "police_jump_e02");
    asset_manager_add(game->animations, animation_new(sprites, 3), "police_jump_e");
    
    
    sprites[0] = asset_manager_get(game->sprites, "police_aim_e00");
    sprites[1] = asset_manager_get(game->sprites, "police_aim_e01");
    asset_manager_add(game->animations, animation_new(sprites, 2), "police_aim_e");
    
    font_manager_load_font(game, game->f_manager, ASSETS_DIR "font.png", 8, 8);
    
    
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "ambient.wav"), "ambient");
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "wiggle.wav"), "wiggle");
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "bump.wav"), "bump");
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "getaway.wav"), "getaway");
    asset_manager_add(game->samples, load_sample(game->s_manager, ASSETS_DIR "jump.wav"), "jump");
    
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
                //printf("\nframes: %d", frames_this_second);
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
