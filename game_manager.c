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
            // Test boulder, gem and gold if they should fall or not
            if(map->map[i][j] == BOULDER || map->map[i][j] == GEM || map->map[i][j] == GOLD || map->map[i][j] == FALLING_GEM || map->map[i][j] == FALLING_BOULDER || map->map[i][j] == FALLING_GOLD)
                update_fall(map, j, i, check_fall_status(map, j, i), bools, samples);

            // Test boulder, gem and gold if they should roll or not
            if((map->map[i][j] == GEM) || (map->map[i][j] == BOULDER) || (map->map[i][j] == GOLD) || (map->map[i][j] == FALLING_GEM) || (map->map[i][j] == FALLING_BOULDER) || (map->map[i][j] == FALLING_GOLD))
                check_roll_status(map, j, i);

            // Test firefly and if its direction should change or not
            if((map->map[i][j] == FIREFLY_UP) || (map->map[i][j] == FIREFLY_LEFT) || (map->map[i][j] == FIREFLY_DOWN) || (map->map[i][j] == FIREFLY_RIGHT))
                update_firefly(map, j, i, check_firefly_status(map, j, i), bools, samples);

            //Transform firefly explosion into gem
            if(map->map[i][j] == P_EXPLOSION_5)
                map->map[i][j] = AIR;
            else if(map->map[i][j] == E_EXPLOSION_5)
                map->map[i][j] = GEM;
        }
    }
    return;
}

void update_firefly(GAME_MAP* map, int x, int y, int flag, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples)
{
    // Firefly going up
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

    // Firefly going left
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

    // Firefly going down
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

    // Firefly going right
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

    // Based on the direction, returns how it should compute
    // if returns 0: do nothing
    // if returns 1: move to the desired direction
    // if returns 2: move to the desired direction, killing the player

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

// Remove all rocks from game map
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

// Transforms all rocks to gold
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

// Checks firefly movement up
void check_ff_move_up(GAME_MAP* map, int j, int i)
{
    // Check firefly up
    if(map->map[i][j] == FIREFLY_MOVED_UP)
    {
        // Can move left
        if(map->map[i][j - 1] == AIR || map->map[i][j - 1] == PLAYER)
        {
            map->map[i][j] = FIREFLY_LEFT;
            return;
        }

        // Can't move left
        else
        {   
            // Can move up
            if((map->map[i - 1][j] == AIR) || (map->map[i - 1][j] == PLAYER))
            {
                map->map[i][j] = FIREFLY_UP;
                return;
            }

            // Can move right
            if((map->map[i][j + 1] == AIR) || (map->map[i][j + 1] == PLAYER))
            {
                map->map[i][j] = FIREFLY_RIGHT;
                return;
            }

            // Can move down
            if((map->map[i + 1][j] == AIR) || (map->map[i + 1][j] == PLAYER))
            {
                map->map[i][j] = FIREFLY_DOWN;
                return;
            }
        }
    }
}

// Checks firefly movement left
void check_ff_move_left(GAME_MAP* map, int j, int i)
{
    // Check firefly left
    if(map->map[i][j] == FIREFLY_MOVED_LEFT)
    {
        // Can move down
        if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER)
        {
            map->map[i][j] = FIREFLY_DOWN;
            return;
        }

        // Can't move down
        else
        {   
            // Can move left
            if((map->map[i][j - 1] == AIR) || (map->map[i][j - 1] == PLAYER))
            {
                map->map[i][j] = FIREFLY_LEFT;
                return;
            }

            // Can move up
            if((map->map[i - 1][j] == AIR) || (map->map[i - 1][j] == PLAYER))
            {
                map->map[i][j] = FIREFLY_UP;
                return;
            }

            // Can move right
            if((map->map[i][j + 1] == AIR) || (map->map[i][j + 1] == PLAYER))
            {
                map->map[i][j] = FIREFLY_RIGHT;
                return;
            }
        }
    }
}

// Checks firefly movement down
void check_ff_move_down(GAME_MAP* map, int j, int i)
{
    // Check firefly down
    if (map->map[i][j] == FIREFLY_MOVED_DOWN)
    {
        // Can move right
        if (map->map[i][j + 1] == AIR || map->map[i][j + 1] == PLAYER)
        {
            map->map[i][j] = FIREFLY_RIGHT;
            return;
        }

        // Can't move right
        else
        {
            // Can move down
            if ((map->map[i + 1][j] == AIR) || (map->map[i + 1][j] == PLAYER))
            {
                map->map[i][j] = FIREFLY_DOWN;
                return;
            }

            // Can move left
            if ((map->map[i][j - 1] == AIR) || (map->map[i][j - 1] == PLAYER))
            {
                map->map[i][j] = FIREFLY_LEFT;
                return;
            }

            // Can move up
            if ((map->map[i - 1][j] == AIR) || (map->map[i - 1][j] == PLAYER))
            {
                map->map[i][j] = FIREFLY_UP;
                return;
            }
        }
    }
}

// Checks firefly movement right
void check_ff_move_right(GAME_MAP* map, int j, int i)
{
    // Check firefly right
    if (map->map[i][j] == FIREFLY_MOVED_RIGHT)
    {
        // Can move up
        if (map->map[i - 1][j] == AIR || map->map[i - 1][j] == PLAYER)
        {
            map->map[i][j] = FIREFLY_UP;
            return;
        }

        // Can't move up
        else
        {
            // Can move right
            if ((map->map[i][j + 1] == AIR) || (map->map[i][j + 1] == PLAYER))
            {
                map->map[i][j] = FIREFLY_RIGHT;
                return;
            }

            // Can move down
            if ((map->map[i + 1][j] == AIR) || (map->map[i + 1][j] == PLAYER))
            {
                map->map[i][j] = FIREFLY_DOWN;
                return;
            }

            // Can move left
            if ((map->map[i][j - 1] == AIR) || (map->map[i][j - 1] == PLAYER))
            {
                map->map[i][j] = FIREFLY_LEFT;
                return;
            }
        }
    }
}

bool is_firefly(GAME_MAP* map, int x, int y)
{
    if(map->map[y][x] == FIREFLY_UP || map->map[y][x] == FIREFLY_DOWN || map->map[y][x] == FIREFLY_RIGHT || map->map[y][x] == FIREFLY_LEFT 
    || map->map[y][x] == FIREFLY_MOVED_UP || map->map[y][x] == FIREFLY_MOVED_DOWN || map->map[y][x] == FIREFLY_MOVED_RIGHT || map->map[y][x] == FIREFLY_MOVED_LEFT)
        return true;
    return false;
}

// Resets movement of all movable objects, so that they can be moved again
void reset_movement(GAME_MAP* map)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            switch(map->map[i][j])
            {
                // Checks Boulders, Gems and Gold
                case BOULDER_MOVED:
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER || is_firefly(map, j, i + 1))
                        map->map[i][j] = FALLING_BOULDER;
                    else if(map->map[i + 1][j] != AIR && map->map[i + 1][j] != PLAYER)
                        map->map[i][j] = BOULDER;
                    break;
                
                case GEM_MOVED:
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER || is_firefly(map, j, i + 1))
                        map->map[i][j] = FALLING_GEM;
                    else if(map->map[i + 1][j] != AIR && map->map[i + 1][j] != PLAYER)
                        map->map[i][j] = GEM;
                    break;

                case GOLD_MOVED:
                    if(map->map[i + 1][j] == AIR || map->map[i + 1][j] == PLAYER || is_firefly(map, j, i + 1))
                        map->map[i][j] = FALLING_GOLD;
                    else if(map->map[i + 1][j] != AIR && map->map[i + 1][j] != PLAYER)
                        map->map[i][j] = GOLD;
                    break;

                //Checks firefly movement
                case FIREFLY_MOVED_UP: check_ff_move_up(map, j, i); break;
                case FIREFLY_MOVED_LEFT: check_ff_move_left(map, j, i); break;
                case FIREFLY_MOVED_DOWN: check_ff_move_down(map, j, i); break;
                case FIREFLY_MOVED_RIGHT: check_ff_move_right(map, j, i); break;
            }
        }
    }
}

int check_fall_status(GAME_MAP* map, int x, int y)
{
    // returning 0 means the object can fall, but is not falling [BOULDER]
    // returning 1 means the object can fall and is falling [FALLING_BOULDER]
    // returning 2 means the object can fall and is falling right above the player's / firefly's character [BOULDER_MOVED]
    // returning 3 means the object can fall, but is not falling [GEM]
    // returning 4 means the object can fall and is falling [FALLING_GEM]
    // returning 5 means the object can fall and is falling right above the player's / firefly's character [GEM_MOVED]
    // returning 6 means the object can fall, but is not falling [GOLD]
    // returning 7 means the object can fall and is falling [FALLING_GOLD]
    // returning 8 means the object can fall and is falling right above the player's / firefly's character [GOLD_MOVED]

    if(map->map[y][x] == BOULDER || map->map[y][x] == FALLING_BOULDER)
    {
        if(((map->map[y + 1][x] == PLAYER) || is_firefly(map, x, y + 1)) && map->map[y][x] == FALLING_BOULDER)
            return 2; // boulder is falling (will kill player or firefly)
        else if(map->map[y + 1][x] == AIR)
            return 1; // boulder is falling
        else
            return 0; // boulder is not moving
    }

    if(map->map[y][x] == GEM || map->map[y][x] == FALLING_GEM)
    {
        if(((map->map[y + 1][x] == PLAYER) || is_firefly(map, x, y + 1)) && map->map[y][x] == FALLING_GEM)
            return 5; // gem is falling (will kill player)
        else if(map->map[y + 1][x] == AIR)
            return 4; // gem is falling
        else
            return 3; // gem is not moving
    }

    if(map->map[y][x] == GOLD || map->map[y][x] == FALLING_GOLD)
    {
        if(((map->map[y + 1][x] == PLAYER) || is_firefly(map, x, y + 1)) && map->map[y][x] == FALLING_GOLD)
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
        if(map->map[y + 1][x] != PLAYER)
            kill_firefly(map, y + 1, x, samples, bools);
        else
        {
            bools->player_is_dead = true;
            kill_player(map, y + 1, x, samples);
        }
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
        if(map->map[y + 1][x] != PLAYER)
            kill_firefly(map, y + 1, x, samples, bools);
        else
        {
            bools->player_is_dead = true;
            kill_player(map, y + 1, x, samples);
        }
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
        if(map->map[y + 1][x] != PLAYER)
            kill_firefly(map, y + 1, x, samples, bools);
        else
        {
            bools->player_is_dead = true;
            kill_player(map, y + 1, x, samples);
        }
        return;
    }
}

// Initialize player struct based on player's coordinates
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

// Initialize hatch based on hatch's coordinates
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

// Open hatch
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

// Update explosion frames
void update_frames(GAME_MAP* map)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            switch(map->map[i][j])
            {
                // Player Death
                case P_EXPLOSION_0: map->map[i][j]++; break;
                case P_EXPLOSION_1: map->map[i][j]++; break;
                case P_EXPLOSION_2: map->map[i][j]++; break;
                case P_EXPLOSION_3: map->map[i][j]++; break;
                case P_EXPLOSION_4: map->map[i][j]++; break;

                // Enemy Death
                case E_EXPLOSION_0: map->map[i][j]++; break;
                case E_EXPLOSION_1: map->map[i][j]++; break;
                case E_EXPLOSION_2: map->map[i][j]++; break;
                case E_EXPLOSION_3: map->map[i][j]++; break;
                case E_EXPLOSION_4: map->map[i][j]++; break;
            }
        }
    }
}

// Initializes score
void init_score(GAME_SCORE* score, MAP_STORER* ms)
{
    score->gems_collected = 0;
    score->gems_needed = ms->game_map_array[0].gems_needed;
    score->timer = ms->game_map_array[0].map_timer;
    score->game_score = 0;
}

// Restarts player-related collectibles and timer [Score is handled separated from here]
void restart_score(GAME_SCORE* score, int current_level, MAP_STORER* ms)
{
    score->gems_collected = 0;
    score->gems_needed = ms->game_map_array[current_level].gems_needed;
    score->timer = ms->game_map_array[current_level].map_timer;
}

// Updates the player based on what movement it wants to perform
void player_update(GAME_MAP* map, COORDINATES* player, GAME_SCORE* score, STATUS_BOOLS* bools, MY_ALLEGRO_STRUCT* my_al_struct)
{
    if(key[ALLEGRO_KEY_LEFT] || input_cache.key_left)
    {
        check_move(map, player, -1, 0, score, bools, my_al_struct);
        input_cache.key_left = false; // resets cache
    }
    if(key[ALLEGRO_KEY_RIGHT] || input_cache.key_right)
    {
        check_move(map, player, 1, 0, score, bools, my_al_struct);
        input_cache.key_right = false; // resets cache
    }
    if(key[ALLEGRO_KEY_UP] || input_cache.key_up)
    {
        check_move(map, player, 0, -1, score, bools, my_al_struct);
        input_cache.key_up = false; // resets cache
    }
    if(key[ALLEGRO_KEY_DOWN] || input_cache.key_down)
    {
        check_move(map, player, 0, 1, score, bools, my_al_struct);
        input_cache.key_down = false; // resets cache
    }
}

// Check if desired move is able to be completed, if so, executes move else, it returns
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
        add_score(score, OPEN_HATCH, my_al_struct);
        return;
    }
    return;
}

// Actually finishes player movement, by moving the player to a certain position in the game map
void update_move(GAME_MAP* map, COORDINATES* player, int dest_x, int dest_y)
{
    map->map[player->y][player->x] = AIR;
    map->map[player->y + dest_y][player->x + dest_x] = PLAYER;
    player->y = player->y + dest_y;
    player->x = player->x + dest_x;
}

// Returns true if boulder can be pushed
bool check_boulder_push(GAME_MAP* map, int dest_x, int dest_y)
{
    if(map->map[dest_x][dest_y] == AIR)
        return true;
    return false;
}

// Pushes boulder to the side
void push_boulder(GAME_MAP* map, int ori_y, int ori_x, int dest_y, int dest_x)
{
    map->map[ori_y][ori_x] = AIR;
    map->map[dest_y][dest_x] = BOULDER_MOVED;
}

// Add score to player based on what was collected
void add_score(GAME_SCORE* score, int flag, MY_ALLEGRO_STRUCT* my_al_struct)
{
    if(flag == GEM)
    {
        play_gem_collect(&(my_al_struct->samples));
        score->game_score += 100;
    }
    if(flag == CLOCK)
    {
        play_gem_collect(&(my_al_struct->samples));
        score->timer += 5;
    }
    if(flag == GOLD)
    {
        play_gem_collect(&(my_al_struct->samples));
        score->game_score += 200;
    }
    if(flag == OPEN_HATCH)
    {
        score->game_score += (score->timer * 3);
    }
}

// Kills player, starts player explosion animation
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

// Kills firefly, starts enemy explosion animation
void kill_firefly(GAME_MAP* map, int y, int x, MY_ALLEGRO_SAMPLES* samples, STATUS_BOOLS* bools)
{
    play_explosion(samples);
    for(int i = (y - 1); i <= (y + 1); i++)
    {
         for(int j = (x - 1); j <= (x + 1); j++)
        {
            if(map->map[i][j] != BOUNDS)
            {
                if(map->map[i][j] == PLAYER)
                    bools->player_is_dead = true;
                map->map[i][j] = E_EXPLOSION_0;
            }
        }
    }
    return;
}

// Check if boulder/gem/gold can roll, if it can, update it in the game map
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

// Actually updates boulder roll in game map
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

// Updates timer hud and checks if time is up
void hud_timer_update(GAME_SCORE* score, STATUS_BOOLS* bools)
{
    if(score->timer >= 1)
        score->timer--;

    if(score->timer == 0)
        bools->is_time_up = true;
}

void start_level(MAP_STORER* ms, GAME_MAP* map, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples, int current_level)
{
    key[ALLEGRO_KEY_P] &= KEY_RELEASED;
    reset_input_cache();

    bools->menu_is_open = false;
    bools->game_has_started = true;
    bools->is_time_up = false;
    bools->roxploder = false;
    bools->timelord = false;
    bools->midas = false;
    bools->hatch_is_open = false;
    bools->player_is_dead = false;

    load_map(map, ms, current_level);

    play_click(samples);
}

void open_fame_menu(SCOREBOARD* sb, STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples)
{
    fetch_sb(sb);
    key[ALLEGRO_KEY_F] &= KEY_RELEASED;
    if(bools->fame_is_open == false)
    {
        bools->help_is_open = false;
        bools->fame_is_open = true;
        play_click(samples);
    }
}

void open_help_menu(STATUS_BOOLS* bools, MY_ALLEGRO_SAMPLES* samples)
{
    key[ALLEGRO_KEY_H] &= KEY_RELEASED;
    key[ALLEGRO_KEY_F1] &= KEY_RELEASED;
    if(bools->help_is_open == false)
    {
        bools->help_is_open = true;
        bools->fame_is_open = false;
        play_click(samples);
    }
}