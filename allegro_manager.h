#ifndef ALLEGRO_MANAGER_H
#define ALLEGRO_MANAGER_H

#include<stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "map_reader.h"
#include "game_manager.h"

#define BUFFER_W 640
#define BUFFER_H 384

#define DISP_SCALE 2
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define KEY_SEEN     1
#define KEY_RELEASED 2

unsigned char key[ALLEGRO_KEY_MAX];

typedef struct INPUT_CACHE
{
    bool key_up;
    bool key_down;
    bool key_right;
    bool key_left;
}INPUT_CACHE;

INPUT_CACHE input_cache;

typedef struct MY_ALLEGRO_SPRITES
{
    ALLEGRO_BITMAP* stone_brick;
    ALLEGRO_BITMAP* deepslate_brick;
    ALLEGRO_BITMAP* dirt;

    ALLEGRO_BITMAP* boulder;
    ALLEGRO_BITMAP* gem;

    ALLEGRO_BITMAP* miner;

    ALLEGRO_BITMAP* hatch;

}MY_ALLEGRO_SPRITES;

typedef struct MY_ALLEGRO_SAMPLES
{
    ALLEGRO_SAMPLE* sample_dirt_dig[4];
    ALLEGRO_SAMPLE* sample_stone_roll[3];
    ALLEGRO_SAMPLE* sample_walk;
    ALLEGRO_SAMPLE* sample_win;
    ALLEGRO_SAMPLE* sample_gem_collect;
    ALLEGRO_SAMPLE* sample_hatch_open;

}MY_ALLEGRO_SAMPLES;

typedef struct MY_ALLEGRO_TIMERS
{
    ALLEGRO_TIMER* fps;
    ALLEGRO_TIMER* game_tick;
    //ALLEGRO_TIMER* player_move;
    ALLEGRO_TIMER* game_second;

}MY_ALLEGRO_TIMERS;

typedef struct MY_ALLEGRO_STRUCT
{
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* display_buffer;
    ALLEGRO_FONT* font;
    MY_ALLEGRO_SAMPLES* samples;
    ALLEGRO_EVENT_QUEUE* queue;
    MY_ALLEGRO_TIMERS timers;
    MY_ALLEGRO_SPRITES sprites;
    ALLEGRO_EVENT event;

}MY_ALLEGRO_STRUCT;

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

/* Sprites */
void sprites_init(MY_ALLEGRO_STRUCT* my_al_struct);
void sprites_deinit(MY_ALLEGRO_STRUCT* my_al_struct);

/* Draw */
void terrain_draw(GAME_MAP* map, MY_ALLEGRO_STRUCT* my_al_struct);
void hud_draw(MY_ALLEGRO_STRUCT* my_al_struct, int gems_collected, int gems_needed, int gems_total, long timer, long hud_score);

#endif