#ifndef MAP_READER_H
#define MAP_READER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct GAME_MAP
{
    int map_timer;
    int gems_needed;
    int width;
    int height;
    int** map;

}GAME_MAP;

typedef struct MAP_STORER
{
    int number_of_maps;
    int maps_width;
    int maps_height;
    GAME_MAP* game_map_array;

}MAP_STORER;

MAP_STORER read_maps();
GAME_MAP* game_map_alloc(int number_of_maps);
int** map_alloc(int width, int height);
void map_initialize(MAP_STORER* ms);
void load_map(GAME_MAP* map, MAP_STORER* ms, int map_number);
void print_map_storer(MAP_STORER* ms);
void print_map(GAME_MAP* map);
void free_map(int** map, int height);
void free_map_storer(MAP_STORER* ms);

#endif