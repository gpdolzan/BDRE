#ifndef ALLEGRO_STRUCTS_H
#define ALLEGRO_STRUCTS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define EXPLOSION_FRAMES 54

typedef struct INPUT_CACHE
{
    bool key_up;
    bool key_down;
    bool key_right;
    bool key_left;
    bool key_r;
    bool key_1;
    bool key_2;
    bool key_3;
}INPUT_CACHE;

typedef struct MY_ALLEGRO_SPRITES
{
    ALLEGRO_BITMAP* title_screen;

    ALLEGRO_BITMAP* stone_brick;
    ALLEGRO_BITMAP* deepslate_brick;
    ALLEGRO_BITMAP* dirt;

    ALLEGRO_BITMAP* boulder;
    ALLEGRO_BITMAP* gem;
    ALLEGRO_BITMAP* clock;
    ALLEGRO_BITMAP* gold;

    ALLEGRO_BITMAP* miner;

    ALLEGRO_BITMAP* hatch;
    ALLEGRO_BITMAP* open_hatch;

    ALLEGRO_BITMAP* explosion_sheet;
    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];

}MY_ALLEGRO_SPRITES;

typedef struct MY_ALLEGRO_SAMPLES
{
    //ALLEGRO_SAMPLE* sample_stone_roll[3];
    ALLEGRO_SAMPLE* sample_click;
    ALLEGRO_SAMPLE* sample_gem_collect;
    ALLEGRO_SAMPLE* sample_hatch_open;
    ALLEGRO_SAMPLE* sample_explosion;
    //ALLEGRO_SAMPLE* sample_win;

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
    ALLEGRO_EVENT_QUEUE* queue;
    MY_ALLEGRO_SAMPLES samples;
    MY_ALLEGRO_TIMERS timers;
    MY_ALLEGRO_SPRITES sprites;
    ALLEGRO_EVENT event;

}MY_ALLEGRO_STRUCT;

#endif