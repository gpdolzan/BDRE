/* GRR20209948 Gabriel Pimentel Dolzan */

#ifndef ALLEGRO_STRUCTS_H
#define ALLEGRO_STRUCTS_H

/* Libraries used */
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/* Number of frames in explosion */
#define EXPLOSION_FRAMES 6

/* Input cache structure */
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

/* Sprites structure */
typedef struct MY_ALLEGRO_SPRITES
{
    ALLEGRO_BITMAP* title_screen;
    ALLEGRO_BITMAP* transparent_screen;

    ALLEGRO_BITMAP* stone_brick;
    ALLEGRO_BITMAP* deepslate_brick;
    ALLEGRO_BITMAP* dirt;

    ALLEGRO_BITMAP* boulder;
    ALLEGRO_BITMAP* gem;
    ALLEGRO_BITMAP* clock;
    ALLEGRO_BITMAP* gold;

    ALLEGRO_BITMAP* miner;
    ALLEGRO_BITMAP* firefly;

    ALLEGRO_BITMAP* hatch;
    ALLEGRO_BITMAP* open_hatch;

    ALLEGRO_BITMAP* explosion_sheet;
    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];

}MY_ALLEGRO_SPRITES;

/* Samples structure */
typedef struct MY_ALLEGRO_SAMPLES
{
    ALLEGRO_SAMPLE* sample_click;
    ALLEGRO_SAMPLE* sample_gem_collect;
    ALLEGRO_SAMPLE* sample_hatch_open;
    ALLEGRO_SAMPLE* sample_explosion;
    ALLEGRO_SAMPLE* sample_win;

}MY_ALLEGRO_SAMPLES;

/* Timers structure */
typedef struct MY_ALLEGRO_TIMERS
{
    ALLEGRO_TIMER* fps;
    ALLEGRO_TIMER* game_tick;
    ALLEGRO_TIMER* game_second;

}MY_ALLEGRO_TIMERS;

/* All allegro information condensed in one single structure */
typedef struct MY_ALLEGRO_STRUCT
{
    MY_ALLEGRO_SAMPLES samples;
    MY_ALLEGRO_TIMERS timers;
    MY_ALLEGRO_SPRITES sprites;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* display_buffer;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* font16;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_EVENT event;

}MY_ALLEGRO_STRUCT;

#endif