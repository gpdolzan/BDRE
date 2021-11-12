#include "allegro_manager.h"

/* General */
void init_check(bool test, char* description)
{
    if(test)
        return;
    printf("Couldn't initialize %s\n", description);
    exit(1);
}

/* Timers */
void init_timers(MY_ALLEGRO_STRUCT* my_al_struct)
{
    my_al_struct->timers.fps = al_create_timer(1.0/60.0);
    init_check(my_al_struct->timers.fps, "fps timer");
    //my_al_struct->timers.player_move = al_create_timer(1.0/12.0);
    //init_check(my_al_struct->timers.player_move, "player move timer");
    my_al_struct->timers.game_tick = al_create_timer(1.0/6.0);
    init_check(my_al_struct->timers.game_tick, "game tick timer");
    my_al_struct->timers.game_second = al_create_timer(1.0);
    init_check(my_al_struct->timers.game_second, "game second timer");
}

void start_timers(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_start_timer(my_al_struct->timers.fps);
    //al_start_timer(my_al_struct->timers.player_move);
    al_start_timer(my_al_struct->timers.game_second);
    al_start_timer(my_al_struct->timers.game_tick);
}

/* Queue */
void init_queue(MY_ALLEGRO_STRUCT* my_al_struct)
{
    my_al_struct->queue = al_create_event_queue();
    init_check(my_al_struct->queue, "queue");
}

void register_queue(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_register_event_source(my_al_struct->queue, al_get_keyboard_event_source());
    al_register_event_source(my_al_struct->queue, al_get_display_event_source(my_al_struct->display));
    al_register_event_source(my_al_struct->queue, al_get_timer_event_source(my_al_struct->timers.fps));
    //al_register_event_source(my_al_struct->queue, al_get_timer_event_source(my_al_struct->timers.player_move));
    al_register_event_source(my_al_struct->queue, al_get_timer_event_source(my_al_struct->timers.game_tick));
    al_register_event_source(my_al_struct->queue, al_get_timer_event_source(my_al_struct->timers.game_second));
}

/* Display */
void display_init(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    my_al_struct->display = al_create_display(DISP_W, DISP_H);
    init_check(my_al_struct->display, "display");

    my_al_struct->display_buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    init_check(my_al_struct->display_buffer, "bitmap buffer");
}

void display_deinit(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_destroy_bitmap(my_al_struct->display_buffer);
    al_destroy_display(my_al_struct->display);
}

void display_pre_draw(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_set_target_bitmap(my_al_struct->display_buffer);
}

void display_post_draw(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_set_target_backbuffer(my_al_struct->display);
    al_draw_scaled_bitmap(my_al_struct->display_buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}

/* Keyboard */
void keyboard_init()
{
    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
        key[i] = 0;
}

void keyboard_update(MY_ALLEGRO_STRUCT* my_al_struct)
{
    switch(my_al_struct->event.type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[my_al_struct->event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_LEFT])
                input_cache.key_left = true;
            if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_RIGHT])
                input_cache.key_right = true;
            if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_UP])
                input_cache.key_up = true;
            if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_DOWN])
                input_cache.key_down = true;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[my_al_struct->event.keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

/* Sprites */
void sprites_init(MY_ALLEGRO_STRUCT* my_al_struct)
{
    init_check(al_init_image_addon(), "image");

    my_al_struct->sprites.stone_brick = al_load_bitmap("./resources/textures/stone_brick.png");
    init_check(my_al_struct->sprites.stone_brick, "stone brick sprite");
    my_al_struct->sprites.deepslate_brick = al_load_bitmap("./resources/textures/ds_brick.png");
    init_check(my_al_struct->sprites.deepslate_brick, "deepslate bricks sprite");
    my_al_struct->sprites.dirt = al_load_bitmap("./resources/textures/dirt.png");
    init_check(my_al_struct->sprites.dirt, "dirt sprite");
    my_al_struct->sprites.boulder = al_load_bitmap("./resources/textures/boulder.png");
    init_check(my_al_struct->sprites.boulder, "boulder sprite");
    my_al_struct->sprites.gem = al_load_bitmap("./resources/textures/gem.png");
    init_check(my_al_struct->sprites.gem, "gem sprite");
    my_al_struct->sprites.miner = al_load_bitmap("./resources/textures/miner.png");
    init_check(my_al_struct->sprites.miner, "miner sprite");
    my_al_struct->sprites.hatch = al_load_bitmap("./resources/textures/hatch.png");
    init_check(my_al_struct->sprites.hatch, "hatch sprite");
}

void sprites_deinit(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_destroy_bitmap(my_al_struct->sprites.stone_brick);
    al_destroy_bitmap(my_al_struct->sprites.deepslate_brick);
    al_destroy_bitmap(my_al_struct->sprites.dirt);
    al_destroy_bitmap(my_al_struct->sprites.boulder);
    al_destroy_bitmap(my_al_struct->sprites.gem);
    al_destroy_bitmap(my_al_struct->sprites.miner);
    al_destroy_bitmap(my_al_struct->sprites.hatch);
}

/* Draw */
void terrain_draw(GAME_MAP* map, MY_ALLEGRO_STRUCT* my_al_struct)
{
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            if(map->map[i][j] == BOUNDS)
                al_draw_bitmap(my_al_struct->sprites.deepslate_brick, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == BRICK)
                al_draw_bitmap(my_al_struct->sprites.stone_brick, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == DIRT)
                al_draw_bitmap(my_al_struct->sprites.dirt, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == GEM)
                al_draw_bitmap(my_al_struct->sprites.gem, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == FALLING_GEM)
                al_draw_bitmap(my_al_struct->sprites.gem, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == BOULDER)
                al_draw_bitmap(my_al_struct->sprites.boulder, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == FALLING_BOULDER)
                al_draw_bitmap(my_al_struct->sprites.boulder, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == PLAYER)
                al_draw_bitmap(my_al_struct->sprites.miner, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == HATCH)
                al_draw_bitmap(my_al_struct->sprites.hatch, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == OPEN_HATCH)
                al_draw_bitmap(my_al_struct->sprites.hatch, (j * 16), ((i * 16) + 32), 0);
        }
    }
}