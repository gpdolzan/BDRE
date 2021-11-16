#include "allegro_manager.h"

/* Input cache */
void reset_input_cache()
{
    input_cache.key_1 = false;
    input_cache.key_2 = false;
    input_cache.key_3 = false;
    input_cache.key_down = false;
    input_cache.key_up = false;
    input_cache.key_left = false;
    input_cache.key_right = false;
    input_cache.key_r = false;
}

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
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_RIGHT])
                input_cache.key_right = true;
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_UP])
                input_cache.key_up = true;
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_DOWN])
                input_cache.key_down = true;
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_R])
                input_cache.key_r = true;
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_1])
                input_cache.key_1 = true;
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_2])
                input_cache.key_2 = true;
            else if(key[my_al_struct->event.keyboard.keycode] == key[ALLEGRO_KEY_3])
                input_cache.key_3 = true;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[my_al_struct->event.keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

/* Fonts */
void font_init(MY_ALLEGRO_STRUCT* my_al_struct)
{
    my_al_struct->font = al_load_font("./resources/fonts/Barlow-Bold.ttf", 32, 0);
    init_check(my_al_struct->font, "BARLOW font initialization");
    my_al_struct->font16 = al_load_font("./resources/fonts/Barlow-Bold.ttf", 16, 0);
    init_check(my_al_struct->font16, "BARLOW font initialization");
}

/* Audio */
void audio_init(MY_ALLEGRO_SAMPLES* samples)
{

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    samples->sample_click = al_load_sample("./resources/SFX/click.ogg");
    init_check(samples->sample_click, "audio click");
    samples->sample_gem_collect = al_load_sample("./resources/SFX/collect_gem.ogg");
    init_check(samples->sample_gem_collect, "audio sample gem collect");
    samples->sample_explosion = al_load_sample("./resources/SFX/explosion.ogg");
    init_check(samples->sample_explosion, "audio sample explosion");
    samples->sample_hatch_open = al_load_sample("./resources/SFX/hatch_open.ogg");
    init_check(samples->sample_hatch_open, "audio sample hatch open");
}

void play_click(MY_ALLEGRO_SAMPLES* samples)
{
    al_play_sample(samples->sample_click, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void play_gem_collect(MY_ALLEGRO_SAMPLES* samples)
{
    al_play_sample(samples->sample_gem_collect, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void play_explosion(MY_ALLEGRO_SAMPLES* samples)
{
    al_play_sample(samples->sample_explosion, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void play_hatch_open(MY_ALLEGRO_SAMPLES* samples)
{
    al_play_sample(samples->sample_hatch_open, 0.9, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void audio_deinit(MY_ALLEGRO_SAMPLES* samples)
{
    al_destroy_sample(samples->sample_click);
    al_destroy_sample(samples->sample_gem_collect);
    al_destroy_sample(samples->sample_explosion);
    al_destroy_sample(samples->sample_hatch_open);
}


/* Sprites */

ALLEGRO_BITMAP* sprite_grab(MY_ALLEGRO_STRUCT* my_al_struct, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(my_al_struct->sprites.explosion_sheet, x, y, w, h);
    init_check(sprite, "sprite grab");
    return sprite;
}

void sprites_init(MY_ALLEGRO_STRUCT* my_al_struct)
{
    init_check(al_init_image_addon(), "image");

    my_al_struct->sprites.title_screen = al_load_bitmap("./resources/textures/title_screen.png");
    init_check(my_al_struct->sprites.title_screen, "title screen sprite");

    my_al_struct->sprites.transparent_screen = al_load_bitmap("./resources/textures/dimmed_background.png");
    init_check(my_al_struct->sprites.transparent_screen, "transparent screen sprite");

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
    my_al_struct->sprites.clock = al_load_bitmap("./resources/textures/clock.png");
    init_check(my_al_struct->sprites.clock, "clock sprite");
    my_al_struct->sprites.gold = al_load_bitmap("./resources/textures/gold.png");
    init_check(my_al_struct->sprites.gold, "gold sprite");
    my_al_struct->sprites.firefly = al_load_bitmap("./resources/textures/creeper.png");
    init_check(my_al_struct->sprites.firefly, "firefly sprite");
    my_al_struct->sprites.miner = al_load_bitmap("./resources/textures/miner.png");
    init_check(my_al_struct->sprites.miner, "miner sprite");
    my_al_struct->sprites.hatch = al_load_bitmap("./resources/textures/hatch.png");
    init_check(my_al_struct->sprites.hatch, "hatch sprite");
    my_al_struct->sprites.open_hatch = al_load_bitmap("./resources/textures/open_hatch.png");
    init_check(my_al_struct->sprites.open_hatch, "open hatch sprite");

    my_al_struct->sprites.explosion_sheet = al_load_bitmap("./resources/textures/explosion.png");
    init_check(my_al_struct->sprites.explosion_sheet, "explosion sheet sprite");

    //int sprite_pos = 0;
    //int x = 0;
    //int y = 0;

    /*for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            my_al_struct->sprites.explosion[sprite_pos] = sprite_grab(my_al_struct, x, y, 16, 16);
            sprite_pos++;
            x += 16;
        }
        x = 0;
        y += 16;
    }*/

}

void sprites_deinit(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_destroy_bitmap(my_al_struct->sprites.title_screen);
    al_destroy_bitmap(my_al_struct->sprites.transparent_screen);
    al_destroy_bitmap(my_al_struct->sprites.stone_brick);
    al_destroy_bitmap(my_al_struct->sprites.deepslate_brick);
    al_destroy_bitmap(my_al_struct->sprites.dirt);
    al_destroy_bitmap(my_al_struct->sprites.boulder);
    al_destroy_bitmap(my_al_struct->sprites.gem);
    al_destroy_bitmap(my_al_struct->sprites.miner);
    al_destroy_bitmap(my_al_struct->sprites.firefly);
    al_destroy_bitmap(my_al_struct->sprites.hatch);
    al_destroy_bitmap(my_al_struct->sprites.open_hatch);
    al_destroy_bitmap(my_al_struct->sprites.clock);
    al_destroy_bitmap(my_al_struct->sprites.gold);
    al_destroy_bitmap(my_al_struct->sprites.explosion_sheet);

    //for(int i = 0; i < 54; i++)
       //al_destroy_bitmap(my_al_struct->sprites.explosion[i]);
}

/* Draw */
void title_screen_draw(MY_ALLEGRO_STRUCT* my_al_struct)
{
    al_draw_bitmap(my_al_struct->sprites.title_screen, 0, 0, 0);
    al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, ((BUFFER_H/2) - 48), ALLEGRO_ALIGN_CENTRE, "[P]lay the game!");
    al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, BUFFER_H/2, ALLEGRO_ALIGN_CENTRE, "Hall of [F]ame!");
    al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, ((BUFFER_H/2) + 48), ALLEGRO_ALIGN_CENTRE, "[H]elp!");
    al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, ((BUFFER_H/2) + 96), ALLEGRO_ALIGN_CENTRE, "[Esc] to quit!");
}

void hall_of_fame_draw(MY_ALLEGRO_STRUCT* my_al_struct, SCOREBOARD* sb)
{
    int sum = 64;
    al_draw_bitmap(my_al_struct->sprites.transparent_screen, 0, 0, 0);
    al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, 0, ALLEGRO_ALIGN_CENTRE, "Welcome to the HALL OF FAME!");
    if(sb->sb_size > 0)
    {
        for(int i = (sb->sb_size - 1); i >= 0 ; i--)
        {
            al_draw_textf(my_al_struct->font16, al_map_rgb(255, 255, 255), BUFFER_W/2, sum, ALLEGRO_ALIGN_CENTRE, "[Position: %d] [Score: %ld]", (sb->sb_size - i), sb->scores_array[i]);
            sum += 24;
        }
        al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, (BUFFER_H - 64), ALLEGRO_ALIGN_CENTRE, "Press [F] to return!");
    }
    else
    {
        al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, 32, ALLEGRO_ALIGN_CENTRE, "There is nothing here!");
        al_draw_text(my_al_struct->font, al_map_rgb(255, 255, 255), BUFFER_W/2, 64, ALLEGRO_ALIGN_CENTRE, "Play the game and then come back here!");
    }
}

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
            if(map->map[i][j] == CLOCK)
                al_draw_bitmap(my_al_struct->sprites.clock, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == GOLD)
                al_draw_bitmap(my_al_struct->sprites.gold, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == FALLING_GOLD)
                al_draw_bitmap(my_al_struct->sprites.gold, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == GOLD_MOVED)
                al_draw_bitmap(my_al_struct->sprites.gold, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == GEM)
                al_draw_bitmap(my_al_struct->sprites.gem, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == FALLING_GEM)
                al_draw_bitmap(my_al_struct->sprites.gem, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == GEM_MOVED)
                al_draw_bitmap(my_al_struct->sprites.gem, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == BOULDER)
                al_draw_bitmap(my_al_struct->sprites.boulder, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == FALLING_BOULDER)
                al_draw_bitmap(my_al_struct->sprites.boulder, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == BOULDER_MOVED)
                al_draw_bitmap(my_al_struct->sprites.boulder, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == PLAYER)
                al_draw_bitmap(my_al_struct->sprites.miner, (j * 16), ((i * 16) + 32), 0);
            if((map->map[i][j] == FIREFLY_UP) || (map->map[i][j] == FIREFLY_MOVED_UP) 
                || (map->map[i][j] == FIREFLY_LEFT) || (map->map[i][j] == FIREFLY_MOVED_LEFT) 
                || (map->map[i][j] == FIREFLY_DOWN) || (map->map[i][j] == FIREFLY_MOVED_DOWN) 
                || (map->map[i][j] == FIREFLY_RIGHT) || (map->map[i][j] == FIREFLY_MOVED_RIGHT))
                al_draw_bitmap(my_al_struct->sprites.firefly, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == HATCH)
                al_draw_bitmap(my_al_struct->sprites.hatch, (j * 16), ((i * 16) + 32), 0);
            if(map->map[i][j] == OPEN_HATCH)
                al_draw_bitmap(my_al_struct->sprites.open_hatch, (j * 16), ((i * 16) + 32), 0);
        }
    }
}

void hud_draw(MY_ALLEGRO_STRUCT* my_al_struct, int gems_collected, int gems_needed, int gems_total, long timer, long hud_score)
{
    al_draw_textf(my_al_struct->font, al_map_rgb(255, 255, 255), 16, 0, 0, "%02d/%02d - %02d", gems_collected, gems_total, gems_needed);
    al_draw_textf(my_al_struct->font, al_map_rgb(255, 255, 255), 640/2, 0, ALLEGRO_ALIGN_CENTRE, "%03ld", timer);
    al_draw_textf(my_al_struct->font, al_map_rgb(255, 255, 255), 640-16, 0, ALLEGRO_ALIGN_RIGHT, "%06ld", hud_score);
}