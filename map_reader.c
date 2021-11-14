#include "map_reader.h"

MAP_STORER read_maps()
{
    MAP_STORER ms;
    FILE* map_file;

    //Trying to open map_file
    map_file = fopen("./resources/map/map.txt", "r");
    if(map_file == NULL)
    {
        fprintf(stderr, "Couldn't open map file. Aborting execution!\n");
        exit(1);
    }

    fscanf(map_file, "%d", &(ms.number_of_maps));
    fscanf(map_file, "%d %d", &(ms.maps_width), &(ms.maps_height));

    ms.game_map_array = game_map_alloc(ms.number_of_maps);

    for(int l = 0; l < ms.number_of_maps; l++)
        ms.game_map_array[l].map = map_alloc(ms.maps_width, ms.maps_height);
    
    map_initialize(&ms);

    for(int k = 0; k < ms.number_of_maps; k++)
    {
        fscanf
        (
        map_file, "%d %d %d", 
        &(ms.game_map_array[k].map_timer),
        &(ms.game_map_array[k].gems_needed),
        &(ms.game_map_array[k].gems_total)
        );
        ms.game_map_array[k].width = ms.maps_width;
        ms.game_map_array[k].height = ms.maps_height;

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

void map_initialize(MAP_STORER* ms)
{
    for(int k = 0; k < ms->number_of_maps; k++)
        for(int i = 0; i < ms->maps_height; i++)
            for(int j = 0; j < ms->maps_width; j++)
                ms->game_map_array[k].map[i][j] = 0;
}

void load_map(GAME_MAP* map, MAP_STORER* ms, int map_number)
{
    map->width = ms->maps_width;
    map->height = ms->maps_height;
    map->map_timer = ms->game_map_array[map_number].map_timer;
    map->gems_needed = ms->game_map_array[map_number].gems_needed;
    map->gems_total = ms->game_map_array[map_number].gems_total;
    for(int i = 0; i < ms->maps_height; i++)
        for(int j = 0; j < ms->maps_width; j++)
            map->map[i][j] = ms->game_map_array[map_number].map[i][j];
}

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

void free_map(int** map, int height)
{
    for(int i = 0; i < height; i++)
        free(map[i]);
    free(map);
}

void free_map_storer(MAP_STORER* ms)
{
    for(int i = 0; i < ms->number_of_maps; i++)
        free_map(ms->game_map_array[i].map, ms->maps_height);
    free(ms->game_map_array);
}