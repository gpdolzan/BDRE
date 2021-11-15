#ifndef ALLEGRO_MANAGER_H
#define ALLEGRO_MANAGER_H

#include<stdio.h>

#include "allegro_structs.h"
#include "game_manager.h"
#include "map_reader.h"
#include "scoreboard.h"

#define BUFFER_W 640
#define BUFFER_H 384

#define DISP_SCALE 2
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define KEY_SEEN     1
#define KEY_RELEASED 2

unsigned char key[ALLEGRO_KEY_MAX];

INPUT_CACHE input_cache;
void reset_input_cache();

/* General */
void init_check(bool test, char* description);

/* Timers */
void init_timers(MY_ALLEGRO_STRUCT* my_al_struct);
void start_timers(MY_ALLEGRO_STRUCT* my_al_struct);

/* Queue */
void init_queue(MY_ALLEGRO_STRUCT* my_al_struct);
void register_queue(MY_ALLEGRO_STRUCT* my_al_struct);

/* Display */
void display_init(MY_ALLEGRO_STRUCT* my_al_struct);
void display_deinit(MY_ALLEGRO_STRUCT* my_al_struct);
void display_pre_draw(MY_ALLEGRO_STRUCT* my_al_struct);
void display_post_draw(MY_ALLEGRO_STRUCT* my_al_struct);
void display_clear(MY_ALLEGRO_STRUCT* my_al_struct);

/* Keyboard */
void keyboard_init();
void keyboard_update(MY_ALLEGRO_STRUCT* my_al_struct);

/* Fonts */
void font_init(MY_ALLEGRO_STRUCT* my_al_struct);

/* Audio */
void audio_init(MY_ALLEGRO_SAMPLES* samples);
void play_click(MY_ALLEGRO_SAMPLES* samples);
void play_gem_collect(MY_ALLEGRO_SAMPLES* samples);
void play_explosion(MY_ALLEGRO_SAMPLES* samples);
void play_hatch_open(MY_ALLEGRO_SAMPLES* samples);
void audio_deinit(MY_ALLEGRO_SAMPLES* samples);

/* Sprites */
void sprites_init(MY_ALLEGRO_STRUCT* my_al_struct);
ALLEGRO_BITMAP* sprite_grab(MY_ALLEGRO_STRUCT* my_al_struct, int x, int y, int w, int h);
void sprites_deinit(MY_ALLEGRO_STRUCT* my_al_struct);

/* Draw */
void terrain_draw(GAME_MAP* map, MY_ALLEGRO_STRUCT* my_al_struct);
void hud_draw(MY_ALLEGRO_STRUCT* my_al_struct, int gems_collected, int gems_needed, int gems_total, long timer, long hud_score);
void title_screen_draw(MY_ALLEGRO_STRUCT* my_al_struct);
void hall_of_fame_draw(MY_ALLEGRO_STRUCT* my_al_struct, SCOREBOARD* sb);

#endif