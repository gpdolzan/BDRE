/* GRR20209948 Gabriel Pimentel Dolzan */

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

/* Libraries used */
#include <stdbool.h>
#include "allegro_manager.h"
#include "scoreboard.h"
#include "map_reader.h"

/* Defines all objects used in game map */
#define AIR 0
#define PLAYER 1
#define DIRT 2
#define BOULDER 3
#define FALLING_BOULDER 4
#define BOULDER_MOVED 96
#define GEM 5
#define FALLING_GEM 6
#define GEM_MOVED 97
#define BRICK 7
#define BOUNDS 8
#define HATCH 9
#define OPEN_HATCH 10
#define CLOCK 11
#define GOLD 12
#define FALLING_GOLD 13
#define GOLD_MOVED 98

/* Firefly object */
#define FIREFLY_UP 14
#define FIREFLY_MOVED_UP 99
#define FIREFLY_RIGHT 15
#define FIREFLY_MOVED_RIGHT 100
#define FIREFLY_DOWN 16
#define FIREFLY_MOVED_DOWN 101
#define FIREFLY_LEFT 17
#define FIREFLY_MOVED_LEFT 102

/* Player Explosion */
#define P_EXPLOSION_0   18
#define P_EXPLOSION_1   19
#define P_EXPLOSION_2   20
#define P_EXPLOSION_3   21
#define P_EXPLOSION_4   22
#define P_EXPLOSION_5   23

/* Enemy Explosion */
#define E_EXPLOSION_0   24
#define E_EXPLOSION_1   25
#define E_EXPLOSION_2   26
#define E_EXPLOSION_3   27
#define E_EXPLOSION_4   28
#define E_EXPLOSION_5   29

/* Game bools structure */
typedef struct STATUS_BOOLS
{
    /* Menu related */
    bool game_has_started;
    bool menu_is_open;
    bool help_is_open;
    bool fame_is_open;
    bool map_is_loaded;
    bool leave_game;

    /* Gameplay related */
    bool player_is_dead;
    bool hatch_is_open;
    bool is_time_up;
    bool level_win;
    bool game_win;
    bool restart_level;

    /* Easter Egg Related */
    bool timelord;  /* Key [1] */
    bool midas;     /* Key [2] */
    bool roxploder; /* Key [3] */
    
    /* Redraws screen */
    bool redraw;

}STATUS_BOOLS;

/* Coordinates structure */
typedef struct COORDINATES
{
    int x;
    int y;
    
}COORDINATES;

/* Game score structure */
typedef struct GAME_SCORE
{
    int gems_collected;
    int gems_needed;
    long timer;
    long game_score;
    
}GAME_SCORE;

/* Functions headers */

/* Initialization functions */
void initialize_bools(STATUS_BOOLS* bools);
void init_player(GAME_MAP* map, COORDINATES* player);
void init_hatch(GAME_MAP* map, COORDINATES* hatch);
void init_score(GAME_SCORE* score, MAP_STORER* ms);

/* Terrain update functions */
void terrain_update(GAME_MAP* map, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples);
int check_fall_status(GAME_MAP* map, int x, int y);
void update_fall(GAME_MAP* map, int x, int y, int fall_status, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples);
void update_frames(GAME_MAP* map);
bool is_firefly(GAME_MAP* map, int x, int y);
void reset_movement(GAME_MAP* map);
void open_hatch(GAME_MAP* map, COORDINATES* hatch);
void restart_score(GAME_SCORE* score, int current_level, MAP_STORER* ms);
void player_update(GAME_MAP* map, COORDINATES* player, GAME_SCORE* score, STATUS_BOOLS* bools, MY_ALLEGRO_STRUCT* my_al_struct);
void check_move(GAME_MAP* map, COORDINATES* player, int x, int y, GAME_SCORE* score, STATUS_BOOLS* bools, MY_ALLEGRO_STRUCT* my_al_struct);
void update_move(GAME_MAP* map, COORDINATES* player, int dest_x, int dest_y);
void add_score(GAME_SCORE* score, int flag, MY_ALLEGRO_STRUCT* my_al_struct);
bool check_boulder_push(GAME_MAP* map, int dest_x, int dest_y);
void push_boulder(GAME_MAP* map, int ori_x, int ori_y, int dest_x, int dest_y);
void kill_player(GAME_MAP* map, int y, int x, MY_ALLEGRO_SAMPLES* samples);
void kill_firefly(GAME_MAP* map, int y, int x, MY_ALLEGRO_SAMPLES* samples, STATUS_BOOLS* bools);
void check_roll_status(GAME_MAP* map, int x, int y);
void update_roll(GAME_MAP* map, int x, int y, int object, int direction);
void hud_timer_update(GAME_SCORE* score, STATUS_BOOLS* bools);
void update_firefly(GAME_MAP* map, int x, int y, int flag, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples);

/* Firefly functions */
int check_firefly_status(GAME_MAP* map, int x, int y);
void check_ff_move_up(GAME_MAP* map, int j, int i);
void check_ff_move_left(GAME_MAP* map, int j, int i);
void check_ff_move_down(GAME_MAP* map, int j, int i);
void check_ff_move_right(GAME_MAP* map, int j, int i);

/* Menu navigation */
void start_level(MAP_STORER* ms, GAME_MAP* map, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples, int current_level);
void open_fame_menu(SCOREBOARD* sb, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples);
void open_help_menu(STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples);

/* Easter eggs */
void remove_rocks(GAME_MAP* map);
void goldify_rocks(GAME_MAP* map);

#endif