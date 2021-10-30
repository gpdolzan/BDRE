#include "map_reader.h"
#include "game_manager.h"
#include "allegro_manager.h"

int main()
{
    /* My Initialization */

    //Variables
    MAP_STORER map_storer;
    GAME_MAP game_map;
    GAME_SCORE score;
    STATUS_BOOLS game_bools;
    COORDINATES player;
    //Read and load maps from map file
    map_storer = read_maps();
    //Allocate map wich will be used in-game
    game_map.map = map_alloc(map_storer.maps_width, map_storer.maps_height);
    //Load first level
    load_map(&game_map, &map_storer, 0);
    //Load player position
    init_player(&game_map, &player);
    //Initialize game score
    init_score(&score);
    //Initialize game bools
    initialize_bools(&game_bools);

    /* End of my Initialization */

    /* Allegro5 Initialization */

    //Variables
    MY_ALLEGRO_STRUCT my_al_struct;

    init_check(al_init(), "allegro");
    init_check(al_install_keyboard(), "keyboard");

    init_queue(&my_al_struct);
    display_init(&my_al_struct);

    init_timers(&my_al_struct);
    register_queue(&my_al_struct);

    sprites_init(&my_al_struct);
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
                    if(game_bools.menu_is_open == false)
                    {
                        terrain_update(&game_map, &game_bools);
                        if(game_bools.player_is_dead == false)
                            player_update(&game_map, &player, &score, &game_bools);
                    }
                }

                if(key[ALLEGRO_KEY_ESCAPE])
                    game_bools.leave_game = true;

                if(key[ALLEGRO_KEY_F1])
                    game_bools.menu_is_open = false;

                game_bools.redraw = true;
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_bools.leave_game = true;
            break;
        }

        if(game_bools.leave_game || game_bools.game_win)
            break;

        keyboard_update(&my_al_struct);

        if(game_bools.redraw && my_al_struct.event.timer.source == my_al_struct.timers.fps)
        {
            display_pre_draw(&my_al_struct);
            al_clear_to_color(al_map_rgb(0,0,0));

            if(game_bools.menu_is_open == false)
                terrain_draw(&game_map, &my_al_struct);

            display_post_draw(&my_al_struct);

            game_bools.redraw = false;
        }

    }

    //Free memory - avoid leaks
    free_map_storer(&map_storer);
    free_map(game_map.map, game_map.height);

    return 0;
}