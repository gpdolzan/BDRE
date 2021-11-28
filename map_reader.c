/* GRR20209948 Gabriel Pimentel Dolzan */

/* Libraries used */
#include "map_reader.h"

/* Read maps from file and store them on map_storer structure */
MAP_STORER read_maps()
{
    MAP_STORER ms;
    FILE* map_file;

    /* Trying to open map_file */
    map_file = fopen("./resources/map/map.txt", "r");
    if(map_file == NULL)
    {
        fprintf(stderr, "Couldn't open map file. Aborting execution!\n");
        exit(1);
    }

    /* Get maps info */
    fscanf(map_file, "%d", &(ms.number_of_maps));
    fscanf(map_file, "%d %d", &(ms.maps_width), &(ms.maps_height));

    /* Allocate game map array based on number of maps */
    ms.game_map_array = game_map_alloc(ms.number_of_maps);

    /* Allocate map inside game map array based on number of maps on map file */
    for(int l = 0; l < ms.number_of_maps; l++)
        ms.game_map_array[l].map = map_alloc(ms.maps_width, ms.maps_height);
    
    /* map initialization */
    map_initialize(&ms);

    for(int k = 0; k < ms.number_of_maps; k++)
    {
        /* Get map-specific information */
        fscanf(map_file, "%d %d", &(ms.game_map_array[k].map_timer), &(ms.game_map_array[k].gems_needed));

        /* Sets map width and height */
        ms.game_map_array[k].width = ms.maps_width;
        ms.game_map_array[k].height = ms.maps_height;

        /* Fetches game map, iterating through each "game cell" */
        for(int i = 0; i < ms.maps_height; i++)
        {
            for(int j = 0; j < ms.maps_width; j++)
            {
                fscanf(map_file, "%d", &(ms.game_map_array[k].map[i][j]));
            }
        }
    }
            
    fclose(map_file);

    return ms;
}

/* Allocates game_map structure based on how many maps exists in map file */
GAME_MAP* game_map_alloc(int number_of_maps)
{
    GAME_MAP* gm;

    gm = malloc(number_of_maps * sizeof(GAME_MAP));
    if(gm == NULL)
    {
        perror("Couldn't malloc game map: ");
        exit(1);
    }
    return gm;
}

/* Allocates map array based on width and height */
int** map_alloc(int width, int height)
{
    int** map_array;
    map_array = malloc(height * sizeof(int*));
	if(map_array == NULL)
	{
		perror("Couldn't malloc map: ");
		exit(1);
	}
	for(int i = 0; i < height; i++)
	{
		map_array[i] = malloc(width * sizeof(int));
		if(map_array[i] == NULL)
		{
			perror("Couldn't malloc map: ");
			exit(1);
		}
	}
	return map_array;
}

/* Initializes all existing maps in map_storer as 0 */
void map_initialize(MAP_STORER* ms)
{
    for(int k = 0; k < ms->number_of_maps; k++)
        for(int i = 0; i < ms->maps_height; i++)
            for(int j = 0; j < ms->maps_width; j++)
                ms->game_map_array[k].map[i][j] = 0;
}

/* Load to game_map the map_number inside map_storer */
void load_map(GAME_MAP* map, MAP_STORER* ms, int map_number)
{
    map->width = ms->maps_width;
    map->height = ms->maps_height;
    map->map_timer = ms->game_map_array[map_number].map_timer;
    map->gems_needed = ms->game_map_array[map_number].gems_needed;
    for(int i = 0; i < ms->maps_height; i++)
        for(int j = 0; j < ms->maps_width; j++)
            map->map[i][j] = ms->game_map_array[map_number].map[i][j];
}

/* Print map storer information - DEBUGGING FUNCTION - NOT USED IN-GAME */
void print_map_storer(MAP_STORER* ms)
{
    for(int k = 0; k < ms->number_of_maps; k++)
    {
        printf("map %d - time to complete = %d\n", (k + 1), ms->game_map_array[k].map_timer);
        printf("map %d - gems needed = %d\n", (k + 1), ms->game_map_array[k].gems_needed);
        for(int i = 0; i < ms->maps_height; i++)
        {
            for(int j = 0; j < ms->maps_width; j++)
            {
                printf("%d ", ms->game_map_array[k].map[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

/* Print game map information - DEBUGGING FUNCTION - NOT USED IN-GAME */
void print_map(GAME_MAP* map)
{
    printf("time to complete = %d\n", map->map_timer);
    printf("gems needed = %d\n", map->gems_needed);
    for(int i = 0; i < map->height; i++)
    {
        for(int j = 0; j < map->width; j++)
        {
            printf("%d ", map->map[i][j]);
        }
        printf("\n");
    }

}

/* Frees map - avoiding memory leaks */
void free_map(int** map, int height)
{
    for(int i = 0; i < height; i++)
        free(map[i]);
    free(map);
}

/* Frees map_storer - avoiding memory leaks */
void free_map_storer(MAP_STORER* ms)
{
    for(int i = 0; i < ms->number_of_maps; i++)
        free_map(ms->game_map_array[i].map, ms->maps_height);
    free(ms->game_map_array);
}