/* GRR20209948 Gabriel Pimentel Dolzan */

/* Libraries used */
#include "allegro_manager.h"
#include "game_manager.h"
#include "scoreboard.h"
#include <string.h>

int main()
{
    /* My Variables */
    int current_level = 0;
    int persistent_score = 0;
    int restart_timer = 3;
    int persistent_rt = restart_timer;
    MAP_STORER map_storer;
    GAME_MAP game_map;
    GAME_SCORE score;
    STATUS_BOOLS game_bools;
    COORDINATES player;
    COORDINATES hatch;
    SCOREBOARD sb;

    /* Allegro5 Variables */
    MY_ALLEGRO_STRUCT my_al_struct;


    /* My initialization */
    map_storer = read_maps(); /* Read and load maps from map file */
    /* Allocate map wich will be used in-game */
    game_map.map = map_alloc(map_storer.maps_width, map_storer.maps_height);
    if(game_map.map == NULL)
        perror("Insufficient Memory!");
    load_map(&game_map, &map_storer, current_level);

    /* Initialize score */
    init_score(&score, &map_storer);
    /* Initialize game bools */
    initialize_bools(&game_bools);
    /* Initialize scoreboard and get file */
    init_sb(&sb);
    get_file(&sb);

    /* Allegro5 initialization */
    init_check(al_init(), "allegro");
    init_check(al_install_keyboard(), "keyboard");

    init_check(al_init_font_addon(), "font addon");
    init_check(al_init_ttf_addon(), "ttf addon");

    init_queue(&my_al_struct);
    display_init(&my_al_struct);

    init_timers(&my_al_struct);
    register_queue(&my_al_struct);

    audio_init(&(my_al_struct.samples));
    init_check(al_install_audio(), "audio");
    init_check(al_init_acodec_addon(), "audio codecs");
    init_check(al_reserve_samples(128), "reserve samples");

    sprites_init(&my_al_struct);
    font_init(&my_al_struct);
    keyboard_init();

    /* Program starts here! */
    start_timers(&my_al_struct); /* Start timers */

    while(1) /* Game loop is here */
    {
        al_wait_for_event(my_al_struct.queue, &(my_al_struct.event));
        switch(my_al_struct.event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                /* Checks and updates game status every game tick (6 ticks every 1 second) */
                if(my_al_struct.event.timer.source == my_al_struct.timers.game_tick)
                {
                    /* Check if map is being played */
                    if(game_bools.menu_is_open == false && game_bools.help_is_open == false && game_bools.fame_is_open == false && game_bools.map_is_loaded == true)
                    {
                        /* Resets and updates game functionalities */
                        reset_movement(&game_map);
                        terrain_update(&game_map, &game_bools, &(my_al_struct.samples));
                        update_frames(&game_map);

                        /* Check if can update player info(Position inside map and player sprite) */
                        if(game_bools.player_is_dead == false)
                            player_update(&game_map, &player, &score, &game_bools, &my_al_struct);

                        /* Kill player if player is still alive and time is up */
                        if(game_bools.is_time_up == true && game_bools.player_is_dead == false)
                        {
                            game_bools.player_is_dead = true;
                            kill_player(&game_map, player.y, player.x, &(my_al_struct.samples));
                        }
                    }

                    /* Starts TIMELORD EASTER EGG */
                    if((key[ALLEGRO_KEY_1] || input_cache.key_1 == true) && game_bools.map_is_loaded == true)
                    {
                        input_cache.key_1 = false;
                        game_bools.timelord = true;
                        play_click(&(my_al_struct.samples));
                    }

                    /* Starts MIDAS EASTER EGG */
                    if((key[ALLEGRO_KEY_2] || input_cache.key_2 == true) && game_bools.game_has_started == true)
                    {
                        input_cache.key_2 = false;
                        game_bools.midas = true;
                        play_click(&(my_al_struct.samples));
                        goldify_rocks(&game_map);
                    }

                    /* Starts ROXPLODER EASTER EGG */
                    if((key[ALLEGRO_KEY_3] || input_cache.key_3 == true) && game_bools.map_is_loaded == true)
                    {
                        input_cache.key_3 = false;
                        game_bools.midas = true;
                        play_click(&(my_al_struct.samples));
                        remove_rocks(&game_map);
                    }

                }

                /* Every second the screen updates 60 times */
                if(my_al_struct.event.timer.source == my_al_struct.timers.fps)
                {
                    /* Activates level, this will make the game draw map terrain */
                    if(key[ALLEGRO_KEY_P] && game_bools.game_has_started == false && game_bools.help_is_open == false && game_bools.fame_is_open == false)
                    {
                        /* This will make sure the player can only press play via the game menu */
                        if(game_bools.game_has_started == false && game_bools.help_is_open == false && game_bools.fame_is_open == false)
                            start_level(&map_storer, &game_map, &game_bools, &(my_al_struct.samples), current_level);
                    }
                    

                    /* Open Hall of Fame Menu */
                    if(key[ALLEGRO_KEY_F])
                        open_fame_menu(&sb, &game_bools, &(my_al_struct.samples));

                    /* Open Help Menu */
                    if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1])
                        open_help_menu(&game_bools, &(my_al_struct.samples));

                    /* Universal "BACK/RETURN" key, this will make navigating the manu screen very easy */
                    if(key[ALLEGRO_KEY_ESCAPE])
                    {
                        /* Always plays click sound */
                        play_click(&(my_al_struct.samples));

                        /* Resets input cache */
                        key[ALLEGRO_KEY_ESCAPE] &= KEY_RELEASED;
                        reset_input_cache();

                        /* Verify which menu is open, then closes it appropriately */
                        if(game_bools.help_is_open == true) /* Help menu is open */
                            game_bools.help_is_open = false;
                        else if(game_bools.fame_is_open == true) /* Hall of fame menu is open */
                            game_bools.fame_is_open = false;
                        else if(game_bools.menu_is_open == false && game_bools. help_is_open == false && game_bools.fame_is_open == false && game_bools.map_is_loaded == true)
                        {
                            /* If game was running, return to main menu */
                            /* Resets input cache */
                            key[ALLEGRO_KEY_ESCAPE] &= KEY_RELEASED;
                            reset_input_cache();

                            /* Resets game to main menu */
                            persistent_score = 0;
                            current_level = 0;
                            init_score(&score, &map_storer);
                            game_bools.timelord = false;
                            game_bools.menu_is_open = true;
                            game_bools.game_has_started = false;
                            game_bools.is_time_up = false;
                            game_bools.roxploder = false;
                            game_bools.midas = false;
                            game_bools.game_win = false;
                            game_bools.level_win = false;
                            game_bools.map_is_loaded = false;
                            load_map(&game_map, &map_storer, current_level);
                            init_player(&game_map, &player);   
                        }
                        else if(game_bools.menu_is_open == true && game_bools.fame_is_open == false && game_bools.help_is_open == false) /* Player wishes to quit the game */
                            game_bools.leave_game = true;
                    }

                    /* Restart game if R is pressed or if player died */
                    if((key[ALLEGRO_KEY_R] && game_bools.menu_is_open == false) || (input_cache.key_r == true && game_bools.menu_is_open == false) || (game_bools.player_is_dead == true && restart_timer == 0))
                    {
                        /* Play click sound */
                        play_click(&(my_al_struct.samples));

                        /* Resets input cache */
                        key[ALLEGRO_KEY_R] &= KEY_RELEASED;
                        reset_input_cache();
                        input_cache.key_r = false;

                        /* Resets game variables */
                        restart_timer = persistent_rt;
                        game_bools.restart_level = true;
                    }
                }

                /* Ticks every second, used to check if timers have ran out */
                if(my_al_struct.event.timer.source == my_al_struct.timers.game_second)
                {
                    /* Ticks one second of the map timer if one second has elapsed */
                    if(game_bools.player_is_dead == false && game_bools.menu_is_open == false && game_bools.timelord == false && game_bools.fame_is_open == false && game_bools.help_is_open == false)
                        hud_timer_update(&score, &game_bools);

                    /* Ticks restart timer if player is dead */
                    if(game_bools.player_is_dead == true)
                        restart_timer--;
                }

                /* If level will be restarted - resets game variables */
                if(game_bools.restart_level == true)
                {
                    game_bools.restart_level = false;
                    score.game_score = persistent_score;
                    game_bools.player_is_dead = false;
                    game_bools.is_time_up = false;
                    game_bools.level_win = false;
                    game_bools.hatch_is_open = false;
                    game_bools.timelord = false;
                    game_bools.midas = false;
                    game_bools.roxploder = false;
                    restart_score(&score, current_level, &map_storer);
                    load_map(&game_map, &map_storer, current_level);
                    init_player(&game_map, &player);
                }

                /* Tests if map "rotation" has ended */
                if(game_bools.level_win == true)
                {
                    /* Play level win sound */
                    play_win(&(my_al_struct.samples));


                    /* Check if map "rotation" ended */
                    if(current_level == (map_storer.number_of_maps - 1))
                    {
                        /* Add and write score to the scoreboard file */
                        routine_add(&sb, score.game_score);
                        write_to_file(&sb);

                        /* Map "rotation ended" return to main menu */
                        game_bools.game_win = true;
                        game_bools.menu_is_open = true;
                        game_bools.map_is_loaded = false;
                    }
                    else /* Map rotation still has maps, load next map */
                    {
                        /* Resets input cache */
                        reset_input_cache();

                        /* Resets game variables */
                        persistent_score = score.game_score;
                        current_level++;
                        game_bools.timelord = false;
                        game_bools.roxploder = false;
                        game_bools.midas = false;
                        game_bools.level_win = false;
                        game_bools.hatch_is_open = false;
                        restart_score(&score, current_level, &map_storer);

                        /* Load next map */
                        load_map(&game_map, &map_storer, current_level);
                        init_player(&game_map, &player);
                    }
                }

                /* Check if player has already finished the game */
                if(game_bools.game_win == true && game_bools.menu_is_open == true)
                {
                    /* Reset everything, so that the game can be replayed */
                    persistent_score = 0;
                    current_level = 0;
                    game_bools.timelord = false;
                    game_bools.game_has_started = false;
                    game_bools.roxploder = false;
                    game_bools.midas = false;
                    game_bools.game_win = false;
                    game_bools.level_win = false;
                    game_bools.map_is_loaded = true;
                    init_score(&score, &map_storer);

                    /* Load initial map again */
                    load_map(&game_map, &map_storer, current_level);
                    init_player(&game_map, &player);
                }

                game_bools.redraw = true; /* Prepares to redraw screen again */
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_bools.leave_game = true; /* Display will be closed, application will stop running */
            break;
        }

        /* If ESC key has been pressed in the main menu, close game window */
        if(game_bools.leave_game == true)
            break;

        /* Update keyboard input */
        keyboard_update(&my_al_struct);

        /* Check if hatch needs to be opened */
        if((score.gems_collected >= score.gems_needed) && (game_bools.hatch_is_open == false) && (game_bools.map_is_loaded == true))
        {
            /* Opens game hatch */
            play_hatch_open(&(my_al_struct.samples));
            game_bools.hatch_is_open = true;
            open_hatch(&game_map, &hatch);
        }

        /* Load next map if the player reached next level */
        if(game_bools.menu_is_open == false && game_bools.map_is_loaded == false)
        {
            /* Sets game variables */
            game_bools.map_is_loaded = true;
            game_bools.player_is_dead = false;

            /* Load level and its objects */
            load_map(&game_map, &map_storer, current_level);
            init_player(&game_map, &player);
            init_hatch(&game_map, &hatch);
            score.gems_needed = game_map.gems_needed;
            score.timer = game_map.map_timer;
        }

        /* Draws screen based on what is being accessed */
        if(game_bools.redraw && my_al_struct.event.timer.source == my_al_struct.timers.fps)
        {
            display_pre_draw(&my_al_struct);
            al_clear_to_color(al_map_rgb(0,0,0));

            /* Draws terrain (game map) */
            if(game_bools.menu_is_open == false && game_bools.map_is_loaded == true)
            {
                terrain_draw(&game_map, &my_al_struct);
                hud_draw(&my_al_struct, score.gems_collected, score.gems_needed, score.timer, score.game_score);
            }

            /* Draws main menu */
            if(game_bools.menu_is_open == true && game_bools.game_has_started == false)
                title_screen_draw(&my_al_struct);

            /* Draws hall of fame */
            if(game_bools.fame_is_open == true)
                hall_of_fame_draw(&my_al_struct, &sb);

            /* Draws help menu */
            if(game_bools.help_is_open == true)
                help_draw(&my_al_struct);

            /* Prepares display for next frame */
            display_post_draw(&my_al_struct);

            game_bools.redraw = false;
        }

    } 

    /* Deinits allegro structures */
    sprites_deinit(&my_al_struct);
    audio_deinit(&(my_al_struct.samples));

    /* Free memory - avoid leaks */
    free_map_storer(&map_storer);
    free_map(game_map.map, game_map.height);
    free(sb.scores_array);

    return 0;
}