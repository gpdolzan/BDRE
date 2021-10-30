#include "map_reader.h"
#include "game_manager.h"

int main()
{
    /* My Initialization */

    //Variables
    MAP_STORER map_storer;
    GAME_MAP game_map;
    STATUS_BOOLS game_bools;
    //Read and load maps from map file
    map_storer = read_maps();
    //Allocate map wich will be used in-game
    game_map.map = map_alloc(map_storer.maps_width, map_storer.maps_height);
    //Load first level
    load_map(&game_map, &map_storer, 0);
    //Initialize game bools
    initialize_bools(&game_bools);

    /* End of my Initialization */

    /* Allegro5 Initialization */



    /* End of Allegro5 Initialization */

    //Free memory - avoid leaks
    free_map_storer(&map_storer);
    free_map(game_map.map, game_map.height);

    return 0;
}