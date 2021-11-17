#include "game_manager.h"
#include "allegro_manager.h"

void initialize_bools(STATUS_BOOLS* bools)
{
    bools->map_is_loaded = false;
    bools->game_has_started = false;
    bools->menu_is_open = true;
    bools->fame_is_open = false;
    bools->game_win = false;
    bools->help_is_open = false;
    bools->level_win = false;
    bools->player_is_dead = false;
    bools->is_time_up = false;
    bools->restart_level = false;
    bools->hatch_is_open = false;
    bools->leave_game = false;
    bools->redraw = true;

    bools->midas = false;
    bools->timelord = false;
    bools->roxploder = false;

    return;
}

void terrain_update(GAME_MAP* map, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {  
            if(map->map[i][j] == BOULDER || map->map[i][j] == GEM || map->map[i][j] == GOLD || map->map[i][j] == FALLING_GEM || map->map[i][j] == FALLING_BOULDER || map->map[i][j] == FALLING_GOLD)
            {
                update_fall(map, j, i, check_fall_status(map, j, i), bools, samples);
            }
            if((map->map[i][j] == GEM) || (map->map[i][j] == BOULDER) || (map->map[i][j] == GOLD) || (map->map[i][j] == FALLING_GEM) || (map->map[i][j] == FALLING_BOULDER) || (map->map[i][j] == FALLING_GOLD))
                check_roll_status(map, j, i);
            if((map->map[i][j] == FIREFLY_UP) || (map->map[i][j] == FIREFLY_LEFT) || (map->map[i][j] == FIREFLY_DOWN) || (map->map[i][j] == FIREFLY_RIGHT))
                update_firefly(map, j, i, check_firefly_status(map, j, i), bools, samples);
        }
    }
    return;
}

void update_firefly(GAME_MAP* map, int x, int y, int flag, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples)
{
    if((map->map[y][x] == FIREFLY_UP) && flag >= 1)
    {
        if(flag == 1)
        {
            map->map[y][x] = AIR;
            map->map[y - 1][x] = FIREFLY_MOVED_UP;
        }
        else if(flag == 2)
        {
            bools->player_is_dead = true;
            kill_player(map, y - 1, x, samples);
        }
        return;
    }
    if((map->map[y][x] == FIREFLY_LEFT) && flag >= 1)
    {
        if(flag == 1)
        {
            map->map[y][x] = AIR;
            map->map[y][x - 1] = FIREFLY_MOVED_LEFT;
        }
        else if(flag == 2)
        {
            bools->player_is_dead = true;
            kill_player(map, y, x - 1, samples);
        }
        return;
    }
    if((map->map[y][x] == FIREFLY_DOWN) && flag >= 1)
    {
        if(flag == 1)
        {
            map->map[y][x] = AIR;
            map->map[y + 1][x] = FIREFLY_MOVED_DOWN;
        }
        else if(flag == 2)
        {
            bools->player_is_dead = true;
            kill_player(map, y + 1, x, samples);
        }
        return;
    }
    if((map->map[y][x] == FIREFLY_RIGHT) && flag >= 1)
    {
        if(flag == 1)
        {
            map->map[y][x] = AIR;
            map->map[y][x + 1] = FIREFLY_MOVED_RIGHT;
        }
        else if(flag == 2)
        {
            bools->player_is_dead = true;
            kill_player(map, y, x + 1, samples);
        }
        return;
    }
}

int check_firefly_status(GAME_MAP* map, int x, int y)
{
    if(map->map[y][x] == FIREFLY_UP)
    {
        if(map->map[y - 1][x] == AIR)
            return 1;
        else if(map->map[y - 1][x] == PLAYER)
            return 2;
        return 0;
    }
    if(map->map[y][x] == FIREFLY_LEFT)
    {
        if(map->map[y][x - 1] == AIR)
            return 1;
        else if(map->map[y][x - 1] == PLAYER)
            return 2;
        return 0;
    }
    if(map->map[y][x] == FIREFLY_DOWN)
    {
        if(map->map[y + 1][x] == AIR)
            return 1;
        else if(map->map[y + 1][x] == PLAYER)
            return 2;
        return 0;
    }
    if(map->map[y][x] == FIREFLY_RIGHT)
    {
        if(map->map[y][x + 1] == AIR)
            return 1;
        else if(map->map[y][x + 1] == PLAYER)
            return 2;
        return 0;
    }
    return 0;
}

void remove_rocks(GAME_MAP* map)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {  
            if (map->map[i][j] == BOULDER || map->map[i][j] == FALLING_BOULDER || map->map[i][j] == BOULDER_MOVED)
            {
                map->map[i][j] = AIR;
            }
        }
    }
}

void goldify_rocks(GAME_MAP* map)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {  
            if (map->map[i][j] == BOULDER || map->map[i][j] == FALLING_BOULDER || map->map[i][j] == BOULDER_MOVED)
            {
                map->map[i][j] = GOLD;
            }
        }
    }
}

void reset_movement(GAME_MAP* map)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            // Checks Boulders, Gems and Gold
            if((map->map[i][j] == BOULDER_MOVED) || (map->map[i][j] == GEM_MOVED) || (map->map[i][j] == GOLD_MOVED))
            {
                if(map->map[i][j] == BOULDER_MOVED)
                {
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER)
                        map->map[i][j] = FALLING_BOULDER;
                    else if(map->map[i + 1][j] != AIR && map->map[i + 1][j] != PLAYER)
                    map->map[i][j] = BOULDER;
                }

                else if(map->map[i][j] == GEM_MOVED)
                {
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER)
                        map->map[i][j] = FALLING_GEM;
                    else if(map->map[i + 1][j] != AIR && map->map[i + 1][j] != PLAYER)
                        map->map[i][j] = GEM;
                }

                else if(map->map[i][j] == GOLD_MOVED)
                {
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER)
                        map->map[i][j] = FALLING_GOLD;
                    else if(map->map[i + 1][j] != AIR && map->map[i + 1][j] != PLAYER)
                        map->map[i][j] = GOLD;
                }
            }

            //Checks firefly movement
            else if((map->map[i][j] == FIREFLY_MOVED_UP) || (map->map[i][j] == FIREFLY_MOVED_DOWN) || (map->map[i][j] == FIREFLY_MOVED_LEFT) || (map->map[i][j] == FIREFLY_MOVED_RIGHT))
            {
                // Check firefly up
                if(map->map[i][j] == FIREFLY_MOVED_UP)
                {
                    // Can move left
                    if(map->map[i][j - 1] == AIR || map->map[i][j - 1] == PLAYER)
                    {
                        map->map[i][j] = FIREFLY_LEFT;
                        break;
                    }

                    // Can't move left
                    else
                    {   
                        // Can move up
                        if((map->map[i - 1][j] == AIR) || (map->map[i - 1][j] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_UP;
                            break;
                        }

                        // Can move right
                        if((map->map[i][j + 1] == AIR) || (map->map[i][j + 1] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_RIGHT;
                            break;
                        }

                        // Can move down
                        if((map->map[i + 1][j] == AIR) || (map->map[i + 1][j] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_DOWN;
                            break;
                        }
                    }
                }

                // Check firefly left
                if(map->map[i][j] == FIREFLY_MOVED_LEFT)
                {
                    // Can move down
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER)
                    {
                        map->map[i][j] = FIREFLY_DOWN;
                        break;
                    }

                    // Can't move down
                    else
                    {   
                        // Can move left
                        if((map->map[i][j - 1] == AIR) || (map->map[i][j - 1] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_LEFT;
                            break;
                        }

                        // Can move up
                        if((map->map[i - 1][j] == AIR) || (map->map[i - 1][j] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_UP;
                            break;
                        }

                        // Can move right
                        if((map->map[i][j + 1] == AIR) || (map->map[i][j + 1] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_RIGHT;
                            break;
                        }
                    }
                }

                // Check firefly down
                if(map->map[i][j] == FIREFLY_MOVED_DOWN)
                {
                    // Can move right
                    if(map->map[i][j + 1] == AIR || map->map[i][j + 1] == PLAYER)
                    {
                        map->map[i][j] = FIREFLY_RIGHT;
                        break;
                    }

                    // Can't move right
                    else
                    {   
                        // Can move down
                        if((map->map[i + 1][j] == AIR) || (map->map[i + 1][j] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_DOWN;
                            break;
                        }

                        // Can move left
                        if((map->map[i][j - 1] == AIR) || (map->map[i][j - 1] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_LEFT;
                            break;
                        }

                        // Can move up
                        if((map->map[i - 1][j] == AIR) || (map->map[i - 1][j] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_UP;
                            break;
                        }
                    }
                }

                // Check firefly right
                if(map->map[i][j] == FIREFLY_MOVED_RIGHT)
                {
                    // Can move up
                    if(map->map[i - 1][j] == AIR || map->map[i - 1][j] == PLAYER)
                    {
                        map->map[i][j] = FIREFLY_UP;
                        break;
                    }

                    // Can't move up
                    else
                    {   
                        // Can move right
                        if((map->map[i][j + 1] == AIR) || (map->map[i][j + 1] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_RIGHT;
                            break;
                        }

                        // Can move down
                        if((map->map[i + 1][j] == AIR) || (map->map[i + 1][j] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_DOWN;
                            break;
                        }

                        // Can move left
                        if((map->map[i][j - 1] == AIR) || (map->map[i][j - 1] == PLAYER))
                        {
                            map->map[i][j] = FIREFLY_LEFT;
                            break;
                        }
                    }
                }
            }
        }
    }
}

int check_fall_status(GAME_MAP* map, int x, int y)
{
    // returning 0 means the object can fall, but is not falling [BOULDER]
    // returning 1 means the object can fall and is falling [FALLING_BOULDER]
    // returning 2 means the object can fall and is falling right above the player's character [BOULDER_MOVED]
    // returning 3 means the object can fall, but is not falling [GEM]
    // returning 4 means the object can fall and is falling [FALLING_GEM]
    // returning 5 means the object can fall and is falling right above the player's character [GEM_MOVED]
    // returning 6 means the object can fall, but is not falling [GOLD]
    // returning 7 means the object can fall and is falling [FALLING_GOLD]
    // returning 8 means the object can fall and is falling right above the player's character [GOLD_MOVED]

    if(map->map[y][x] == BOULDER || map->map[y][x] == FALLING_BOULDER)
    {
        if(map->map[y + 1][x] == PLAYER && map->map[y][x] == FALLING_BOULDER)
            return 2; // boulder is falling (will kill player)
        else if(map->map[y + 1][x] == AIR)
            return 1; // boulder is falling
        else
            return 0; // boulder is not moving
    }

    if(map->map[y][x] == GEM || map->map[y][x] == FALLING_GEM)
    {
        if(map->map[y + 1][x] == PLAYER && map->map[y][x] == FALLING_GEM)
            return 5; // gem is falling (will kill player)
        else if(map->map[y + 1][x] == AIR)
            return 4; // gem is falling
        else
            return 3; // gem is not moving
    }

    if(map->map[y][x] == GOLD || map->map[y][x] == FALLING_GOLD)
    {
        if(map->map[y + 1][x] == PLAYER && map->map[y][x] == FALLING_GOLD)
            return 8; // gold is falling (will kill player)
        else if(map->map[y + 1][x] == AIR)
            return 7; // gold is falling
        else
            return 6; // gold is not moving
    }

    return 0;
}

void update_fall(GAME_MAP* map, int x, int y, int fall_status, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples)
{

    // returning 0 means the object can fall, but is not falling [BOULDER]
    // returning 1 means the object can fall and is falling [FALLING_BOULDER]
    // returning 2 means the object can fall and is falling right above the player's character [FALLING_BOULDER]
    // returning 3 means the object can fall, but is not falling [GEM]
    // returning 4 means the object can fall and is falling [FALLING_GEM]
    // returning 5 means the object can fall and is falling right above the player's character [FALLING_GEM]
    // returning 6 means the object can fall, but is not falling [GOLD]
    // returning 7 means the object can fall and is falling [FALLING_GOLD]
    // returning 8 means the object can fall and is falling right above the player's character [GOLD_MOVED]

    if(fall_status == 0)
        return;
    else if(fall_status == 1)
    {
        map->map[y][x] = AIR;
        map->map[y + 1][x] = BOULDER_MOVED;
        return;
    }
    else if(fall_status == 2)
    {
        bools->player_is_dead = true;
        kill_player(map, y + 1, x, samples);
        return;
    }
    else if(fall_status == 3)
        return;
    else if(fall_status == 4)
    {
        map->map[y][x] = AIR;
        map->map[y + 1][x] = GEM_MOVED;
        return;
    }
    else if(fall_status == 5)
    {
        bools->player_is_dead = true;
        kill_player(map, y + 1, x, samples);
        return;
    }
    else if(fall_status == 6)
        return;
    else if(fall_status == 7)
    {
        map->map[y][x] = AIR;
        map->map[y + 1][x] = GOLD_MOVED;
        return;
    }
    else if(fall_status == 8)
    {
        bools->player_is_dead = true;
        kill_player(map, y + 1, x, samples);
        return;
    }
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

void init_hatch(GAME_MAP* map, COORDINATES* hatch)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            if(map->map[i][j] == HATCH)
            {
                hatch->y = i;
                hatch->x = j;
            }
        }
    }
}

void open_hatch(GAME_MAP* map, COORDINATES* hatch)
{
    int found = 0;
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            if(map->map[i][j] == HATCH)
            {
                hatch->y = i;
                hatch->x = j;
                found = 1;
            }
        }
    }

    if(found == 1)
        map->map[hatch->y][hatch->x] = OPEN_HATCH;
    else
        return;
}

void update_frames(GAME_MAP* map)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            switch(map->map[i][j])
            {
                case P_EXPLOSION_0: map->map[i][j] += 2; break;
                //case P_EXPLOSION_1: map->map[i][j]; break;
                case P_EXPLOSION_2: map->map[i][j] += 2; break;
                //case P_EXPLOSION_3: map->map[i][j]++; break;
                case P_EXPLOSION_4: map->map[i][j] += 2; break;
                //case P_EXPLOSION_5: map->map[i][j]++; break;
                case P_EXPLOSION_6: map->map[i][j] += 2; break;
                //case P_EXPLOSION_7: map->map[i][j]++; break;
                case P_EXPLOSION_8: map->map[i][j] += 2; break;
                //case P_EXPLOSION_9: map->map[i][j]++; break;
                case P_EXPLOSION_10: map->map[i][j] = AIR; break;
                //case P_EXPLOSION_11: map->map[i][j] = AIR; break;
            }
        }
    }
}

void init_score(GAME_SCORE* score, MAP_STORER* ms)
{
    score->gems_collected = 0;
    score->gems_needed = ms->game_map_array[0].gems_needed;
    score->timer = ms->game_map_array[0].map_timer;
    score->game_score = 0;
}

void restart_score(GAME_SCORE* score, int current_level, MAP_STORER* ms)
{
    score->gems_collected = 0;
    score->gems_needed = ms->game_map_array[current_level].gems_needed;
    score->gems_total = ms->game_map_array[current_level].gems_total;
    score->timer = ms->game_map_array[current_level].map_timer;
}

void player_update(GAME_MAP* map, COORDINATES* player, GAME_SCORE* score, STATUS_BOOLS* bools, MY_ALLEGRO_STRUCT* my_al_struct)
{
    if(key[ALLEGRO_KEY_LEFT] || input_cache.key_left)
    {
        check_move(map, player, -1, 0, score, bools, my_al_struct);
        input_cache.key_left = false;
    }
    if(key[ALLEGRO_KEY_RIGHT] || input_cache.key_right)
    {
        check_move(map, player, 1, 0, score, bools, my_al_struct);
        input_cache.key_right = false;
    }
    if(key[ALLEGRO_KEY_UP] || input_cache.key_up)
    {
        check_move(map, player, 0, -1, score, bools, my_al_struct);
        input_cache.key_up = false;
    }
    if(key[ALLEGRO_KEY_DOWN] || input_cache.key_down)
    {
        check_move(map, player, 0, 1, score, bools, my_al_struct);
        input_cache.key_down = false;
    }
}

void check_move(GAME_MAP* map, COORDINATES* player, int x, int y, GAME_SCORE* score, STATUS_BOOLS* bools, MY_ALLEGRO_STRUCT* my_al_struct)
{
    if(map->map[player->y + y][player->x + x] == AIR)
    {
        update_move(map, player, x, y);
        return;
    }
    if(map->map[player->y + y][player->x + x] == DIRT)
    {
        update_move(map, player, x, y);
        add_score(score, DIRT, my_al_struct);
        return;
    }
    if(map->map[player->y + y][player->x + x] == GEM)
    {
        update_move(map, player, x, y);
        score->gems_collected++;
        add_score(score, GEM, my_al_struct);
        return;
    }
    if(map->map[player->y + y][player->x + x] == CLOCK)
    {
        update_move(map, player, x, y);
        add_score(score, CLOCK, my_al_struct);
        return;
    }
    if(map->map[player->y + y][player->x + x] == GOLD)
    {
        update_move(map, player, x, y);
        add_score(score, GOLD, my_al_struct);
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
    if(map->map[player->y + y][player->x + x] == OPEN_HATCH)
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
    player->y = player->y + dest_y;
    player->x = player->x + dest_x;
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
    map->map[dest_y][dest_x] = BOULDER_MOVED;
}

void add_score(GAME_SCORE* score, int flag, MY_ALLEGRO_STRUCT* my_al_struct)
{
    if(flag == GEM)
    {
        play_gem_collect(&(my_al_struct->samples));
        score->game_score += 100;
    }
    if(flag == CLOCK)
    {
        score->timer += 20;
    }
    if(flag == GOLD)
    {
        score->game_score += 200;
    }
}

void kill_player(GAME_MAP* map, int y, int x, MY_ALLEGRO_SAMPLES* samples)
{
    play_explosion(samples);
    for(int i = (y - 1); i <= (y + 1); i++)
    {
         for(int j = (x - 1); j <= (x + 1); j++)
        {
            if(map->map[i][j] != BOUNDS)
            {
                map->map[i][j] = P_EXPLOSION_0;
            }
        }
    }
    return;
}

void kill_firefly(GAME_MAP* map, int y, int x, MY_ALLEGRO_SAMPLES* samples)
{
    play_explosion(samples);
    for(int i = (y - 1); i <= (y + 1); i++)
    {
         for(int j = (x - 1); j <= (x + 1); j++)
        {
            if(map->map[i][j] != BOUNDS)
            {
                map->map[i][j] = E_EXPLOSION_0;
            }
        }
    }
    return;
}

void check_roll_status(GAME_MAP* map, int x, int y)
{
    int object = -1;

    if(map->map[y][x] == BOULDER || map->map[y][x] == FALLING_BOULDER)
        object = 0;
    else if(map->map[y][x] == GEM || map->map[y][x] == FALLING_GEM)
        object = 1;
    else if(map->map[y][x] == GOLD || map->map[y][x] == FALLING_GOLD)
        object = 2;

    if(map->map[y + 1][x] == BOULDER || map->map[y + 1][x] == GEM || map->map[y + 1][x] == GOLD || map->map[y + 1][x] == FALLING_BOULDER || map->map[y + 1][x] == FALLING_GEM || map->map[y + 1][x] == FALLING_GOLD)
    {
        //Check right-side
        if(map->map[y][x + 1] == AIR && map->map[y + 1][x + 1] == AIR)
            update_roll(map, x, y, object, 1);
        //Check left-side
        else if (map->map[y][x - 1] == AIR && map->map[y + 1][x - 1] == AIR)
            update_roll(map, x, y, object, -1);
    }
    return;
}

void update_roll(GAME_MAP* map, int x, int y, int object, int direction)
{
    map->map[y][x] = AIR;
    if(object == 0)
        map->map[y][x + direction] = BOULDER_MOVED;
    else if(object == 1)
        map->map[y][x + direction] = GEM_MOVED;
    else
        map->map[y][x + direction] = GOLD_MOVED;
    return;
}

void hud_timer_update(GAME_SCORE* score, STATUS_BOOLS* bools)
{
    if(score->timer >= 1)
        score->timer--;

    if(score->timer == 0)
        bools->is_time_up = true;
}