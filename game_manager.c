#include "game_manager.h"

void initialize_bools(STATUS_BOOLS* bools)
{
    bools->menu_is_open = true;
    bools->player_control = false;
    bools->game_win = false;
    bools->level_win = false;
    bools->player_is_dead = false;
    bools->is_time_ticking = false;
    bools->is_time_up = false;
    bools->restart_level = false;
    bools->player_is_spawning = false;
    bools->leave_game = false;
    bools->redraw = true;

    return;
}

void terrain_update(GAME_MAP* map, STATUS_BOOLS* bools)
{
    for(int i = (map->height - 1); i > 0; i--)
    {
        for(int j = (map->width - 1); j > 0; j--)
        {
            if((map->map[i][j] == BOULDER) || (map->map[i][j] == GEM || map->map[i][j] == FALLING_GEM) || (map->map[i][j] == FALLING_BOULDER))
                check_fall_status(map, j, i, bools);
            if((map->map[i][j] == GEM) || (map->map[i][j] == BOULDER))
                check_roll_status(map, j, i);
        }
    }
    return;
}

void check_fall_status(GAME_MAP* map, int x, int y, STATUS_BOOLS* bools)
{
    bool is_boulder = false;

    if(map->map[y][x] == BOULDER || map->map[y][x] == FALLING_BOULDER)
        is_boulder = true;

    //Check Fall
    if(map->map[y + 1][x] == AIR)
        update_fall(map, x, y, is_boulder);
    else if (map->map[y][x] == FALLING_BOULDER || map->map[y][x] == FALLING_GEM)
        stop_fall(map, x, y, is_boulder, bools);
    return;
}

void update_fall(GAME_MAP* map, int x, int y, bool is_boulder)
{
    map->map[y][x] = AIR;
    if(is_boulder)
        map->map[y + 1][x] = FALLING_BOULDER;
    else
        map->map[y + 1][x] = FALLING_GEM;
    return;
}

void stop_fall(GAME_MAP* map, int x, int y, bool is_boulder, STATUS_BOOLS* bools)
{
    if(map->map[y + 1][x] == PLAYER)
        kill_player(map, y + 1, x, bools, BOULDER_CRUSH);
    else
    {
        if(is_boulder)
            map->map[y][x] = BOULDER;
        else
            map->map[y][x] = GEM;
    }
    return;
}

void init_player(GAME_MAP* map, COORDINATES* player)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            if(map->map[i][j] == PLAYER)
            {
                player->y = i;
                player->x = j;
            }
        }
    }
}

void init_score(GAME_SCORE* score)
{
    score->game_score = 0;
    score->score_display = 0;
}

void player_update(GAME_MAP* map, COORDINATES* player, GAME_SCORE* score, STATUS_BOOLS* bools)
{
    if(key[ALLEGRO_KEY_LEFT])
        check_move(map, player, -1, 0, score, bools);
    if(key[ALLEGRO_KEY_RIGHT])
        check_move(map, player, 1, 0, score, bools);
    if(key[ALLEGRO_KEY_UP])
        check_move(map, player, 0, -1, score, bools);
    if(key[ALLEGRO_KEY_DOWN])
        check_move(map, player, 0, 1, score, bools);
}

void check_move(GAME_MAP* map, COORDINATES* player, int x, int y, GAME_SCORE* score, STATUS_BOOLS* bools)
{
    if(map->map[player->y + y][player->x + x] == AIR)
    {
        update_move(map, player, x, y);
        return;
    }
    if(map->map[player->y + y][player->x + x] == DIRT)
    {
        update_move(map, player, x, y);
        add_score(score, DIRT);
        return;
    }
    if(map->map[player->y + y][player->x + x] == GEM)
    {
        update_move(map, player, x, y);
        add_score(score, GEM);
        return;
    }
    if(map->map[player->y][player->x + x] == BOULDER)
    {
        if(check_boulder_push(map, player->y, player->x + x + x))
        {
            push_boulder(map, player->y, player->x + x, player->y, player->x + x + x);
            update_move(map, player, x, y);
        }
        return;
    }
    if(map->map[player->y][player->x + x] == OPEN_HATCH)
    {
        bools->level_win = true;
        return;
    }
    return;
}

void update_move(GAME_MAP* map, COORDINATES* player, int dest_x, int dest_y)
{
    map->map[player->y][player->x] = AIR;
    map->map[player->y + dest_y][player->x + dest_x] = PLAYER;
    player->y += dest_y;
    player->x += dest_x;
}

bool check_boulder_push(GAME_MAP* map, int dest_x, int dest_y)
{
    if(map->map[dest_x][dest_y] == AIR)
        return true;
    return false;
}

void push_boulder(GAME_MAP* map, int ori_y, int ori_x, int dest_y, int dest_x)
{
    map->map[ori_y][ori_x] = AIR;
    map->map[dest_y][dest_x] = BOULDER;
}

void add_score(GAME_SCORE* score, int flag)
{
    if(flag == DIRT)
        score->game_score += 2;
    if(flag == GEM)
        score->game_score += 100;
}

void kill_player(GAME_MAP* map, int y, int x, STATUS_BOOLS* bools, int death_flag)
{
    for(int i = (y - 1); i <= (y + 1); i++)
    {
         for(int j = (x - 1); j <= (x + 1); j++)
        {
            if(map->map[i][j] != BOUNDS)
            {
                map->map[i][j] = AIR;
            }
        }
    }

    if(death_flag == TIMER_RAN_OUT)
        bools->is_time_up = true;

    bools->is_time_ticking = false;
    bools->player_is_dead = true;
    bools->player_control = false;
    return;
}

void check_roll_status(GAME_MAP* map, int x, int y)
{
    bool is_boulder = false;

    if(map->map[y][x] == BOULDER)
        is_boulder = true;

    if(map->map[y + 1][x] == BOULDER || map->map[y + 1][x] == GEM)
    {
        //Check right-side
        if(map->map[y][x + 1] == AIR && map->map[y + 1][x + 1] == AIR)
            update_roll(map, x, y, is_boulder, 1);
        //Check left-side
        else if (map->map[y][x - 1] == AIR && map->map[y + 1][x - 1] == AIR)
            update_roll(map, x, y, is_boulder, -1);
    }
    return;
}

void update_roll(GAME_MAP* map, int x, int y, bool is_boulder, int direction)
{
    map->map[y][x] = AIR;
    if(is_boulder)
        map->map[y][x + direction] = FALLING_BOULDER;
    else
        map->map[y][x + direction] = FALLING_GEM;
    return;
}