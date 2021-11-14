#include "map_reader.h"
#include "game_manager.h"
#include "allegro_manager.h"

int main()
{
    /* My Initialization */

    //Variables
    int current_level = 0;
    MAP_STORER map_storer;
    GAME_MAP game_map;
    GAME_SCORE score;
    STATUS_BOOLS game_bools;
    COORDINATES player;
    COORDINATES hatch;
    //Read and load maps from map file
    map_storer = read_maps();
    //Allocate map wich will be used in-game
    game_map.map = map_alloc(map_storer.maps_width, map_storer.maps_height);
    //Initialize game score
    init_score(&score, &map_storer);
    //Initialize game bools
    initialize_bools(&game_bools);

    /* End of my Initialization */

    /* Allegro5 Initialization */

    //Variables
    MY_ALLEGRO_STRUCT my_al_struct;

    init_check(al_init(), "allegro");
    init_check(al_install_keyboard(), "keyboard");

    init_check(al_init_font_addon(), "font addon");
    init_check(al_init_ttf_addon(), "ttf addon");

    init_queue(&my_al_struct);
    display_init(&my_al_struct);

    init_timers(&my_al_struct);
    register_queue(&my_al_struct);

    sprites_init(&my_al_struct);
    font_init(&my_al_struct);
    keyboard_init();

    /* End of Allegro5 Initialization */

    start_timers(&my_al_struct);

    while(1)
    {
        al_wait_for_event(my_al_struct.queue, &(my_al_struct.event));

        switch(my_al_struct.event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(my_al_struct.event.timer.source == my_al_struct.timers.game_tick)
                {
                    if(game_bools.menu_is_open == false && game_bools.map_is_loaded == true)
                    {
                        reset_movement(&game_map);
                        terrain_update(&game_map, &game_bools);

                        if(game_bools.player_is_dead == false)
                        {
                            player_update(&game_map, &player, &score, &game_bools);
                        }

                        if(game_bools.is_time_up == true && game_bools.player_is_dead == false)
                        {
                            game_bools.player_is_dead = true;
                            kill_player(&game_map, player.y, player.x, &game_bools, TIMER_RAN_OUT);
                        }
                    }
                }

                if(my_al_struct.event.timer.source == my_al_struct.timers.game_second)
                {
                    if(game_bools.player_is_dead == false && game_bools.menu_is_open == false)
                        hud_timer_update(&score, &game_bools);
                }

                if(game_bools.level_win == true)
                {
                    if(current_level == (map_storer.number_of_maps - 1))
                    {
                        printf("Finished the game!\n");
                        game_bools.game_win = true;
                        game_bools.menu_is_open = true;
                        init_score(&score, &map_storer);
                    }
                    else
                    {
                        current_level++;
                        game_bools.level_win = false;
                        game_bools.hatch_is_open = false;
                        restart_score(&score, current_level, &map_storer);
                        load_map(&game_map, &map_storer, current_level);
                        init_player(&game_map, &player);
                    }
                }

                if(game_bools.game_win == true && game_bools.menu_is_open == true)
                {
                    current_level = 0;
                    init_score(&score, &map_storer);
                    game_bools.game_win = false;
                    game_bools.level_win = false;
                    game_bools.map_is_loaded = true;
                    printf("Loading level 0 AGAIN\n");
                    load_map(&game_map, &map_storer, current_level);
                    init_player(&game_map, &player);
                }

                if(key[ALLEGRO_KEY_ESCAPE])
                    game_bools.leave_game = true;

                if(key[ALLEGRO_KEY_P])
                {
                    game_bools.menu_is_open = false;
                }

                game_bools.redraw = true;
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_bools.leave_game = true;
            break;
        }

        if(game_bools.leave_game == true)
            break;

        keyboard_update(&my_al_struct);

        if((score.gems_collected >= score.gems_needed) && (game_bools.hatch_is_open == false) && (game_bools.map_is_loaded == true))
        {
            game_bools.hatch_is_open = true;
            open_hatch(&game_map, &hatch);
        }

        if(game_bools.menu_is_open == false && game_bools.map_is_loaded == false)
        {
            game_bools.map_is_loaded = true;
            game_bools.player_is_dead = false;
            //Load first level
            load_map(&game_map, &map_storer, current_level);
            //Load player position
            init_player(&game_map, &player);
            //Load hatch position
            init_hatch(&game_map, &hatch);
            //Loaded stats into score tracker
            score.gems_needed = game_map.gems_needed;
            score.timer = game_map.map_timer;
            score.gems_total = game_map.gems_total;
        }

        if(game_bools.redraw && my_al_struct.event.timer.source == my_al_struct.timers.fps)
        {
            display_pre_draw(&my_al_struct);
            al_clear_to_color(al_map_rgb(0,0,0));

            if(game_bools.menu_is_open == false && game_bools.map_is_loaded == true)
            {
                terrain_draw(&game_map, &my_al_struct);
                hud_draw(&my_al_struct, score.gems_collected, score.gems_needed, score.gems_total, score.timer, score.game_score);
            }

            display_post_draw(&my_al_struct);

            game_bools.redraw = false;
        }

    }

    //Free memory - avoid leaks
    free_map_storer(&map_storer);
    free_map(game_map.map, game_map.height);

    return 0;
}