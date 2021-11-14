#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdbool.h>
#include "map_reader.h"
#include "allegro_manager.h"

#define AIR 0
#define PLAYER 1
#define DIRT 2
#define BOULDER 3
#define FALLING_BOULDER 4
#define GEM 5
#define FALLING_GEM 6
#define BRICK 7
#define BOUNDS 8
#define HATCH 9
#define OPEN_HATCH 10

#define BOULDER_MOVED 99
#define GEM_MOVED 98

#define BOULDER_CRUSH 100
#define GEM_CRUSH 101
#define TIMER_RAN_OUT 102

typedef struct STATUS_BOOLS
{
    /* Menu related */
    bool menu_is_open;
    bool map_is_loaded;
    bool leave_game;

    /* Gameplay related */
    bool player_is_dead;
    bool hatch_is_open;
    bool is_time_up;
    bool level_win;
    bool game_win;
    bool restart_level;
    
    /* Redraws screen */
    bool redraw;

}STATUS_BOOLS;

typedef struct COORDINATES
{
    int x;
    int y;
    
}COORDINATES;

typedef struct GAME_SCORE
{
    int gems_collected;
    int gems_needed;
    int gems_total;
    long timer;
    long game_score;
    
}GAME_SCORE;

void initialize_bools(STATUS_BOOLS* bools);
void terrain_update(GAME_MAP* map, STATUS_BOOLS* bools);
int check_fall_status(GAME_MAP* map, int x, int y);
void update_fall(GAME_MAP* map, int x, int y, int fall_status, STATUS_BOOLS* bools);
void reset_movement(GAME_MAP* map);
void init_player(GAME_MAP* map, COORDINATES* player);
void init_hatch(GAME_MAP* map, COORDINATES* hatch);
void open_hatch(GAME_MAP* map, COORDINATES* hatch);
void init_score(GAME_SCORE* score, MAP_STORER* ms);
void restart_score(GAME_SCORE* score, int current_level, MAP_STORER* ms);
void player_update(GAME_MAP* map, COORDINATES* player, GAME_SCORE* score, STATUS_BOOLS* bools);
void check_move(GAME_MAP* map, COORDINATES* player, int x, int y, GAME_SCORE* score, STATUS_BOOLS* bools);
void update_move(GAME_MAP* map, COORDINATES* player, int dest_x, int dest_y);
void add_score(GAME_SCORE* score, int flag);
bool check_boulder_push(GAME_MAP* map, int dest_x, int dest_y);
void push_boulder(GAME_MAP* map, int ori_x, int ori_y, int dest_x, int dest_y);
void kill_player(GAME_MAP* map, int y, int x, STATUS_BOOLS* bools, int death_flag);
void check_roll_status(GAME_MAP* map, int x, int y);
void update_roll(GAME_MAP* map, int x, int y, bool is_boulder, int direction);
void hud_timer_update(GAME_SCORE* score, STATUS_BOOLS* bools);

#endif