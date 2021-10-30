#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdbool.h>
#include "map_reader.h"

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

#define BOULDER_CRUSH 100
#define TIMER_RAN_OUT 101

typedef struct STATUS_BOOLS
{
    bool player_control;
    bool player_is_spawning;
    bool player_is_dead;
    bool is_time_ticking;
    bool is_time_up;
    bool level_win;
    bool game_win;
    bool restart_level;
    bool redraw;

}STATUS_BOOLS;

void initialize_bools(STATUS_BOOLS* bools);
void terrain_update(GAME_MAP* map, STATUS_BOOLS* bools);
void check_fall_status(GAME_MAP* map, int x, int y, STATUS_BOOLS* bools);
void update_fall(GAME_MAP* map, int x, int y, bool is_boulder);
void stop_fall(GAME_MAP* map, int x, int y, bool is_boulder, STATUS_BOOLS* bools);
void kill_player(GAME_MAP* map, int y, int x, STATUS_BOOLS* bools, int death_flag);
void check_roll_status(GAME_MAP* map, int x, int y);
void update_roll(GAME_MAP* map, int x, int y, bool is_boulder, int direction);;

#endif