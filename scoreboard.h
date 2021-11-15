#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct SCOREBOARD
{
    int sb_size;
    long* scores_array;
}SCOREBOARD;

/* Setup functions */
void init_sb(SCOREBOARD* sb);
void get_file(SCOREBOARD* sb);
void routine_add(SCOREBOARD* sb, long score);

/* Internal functions */
void get_size(SCOREBOARD* sb);
void allocate_scores(SCOREBOARD* sb);
bool check_file_existence();
void create_file(SCOREBOARD* sb);
void print_scores(SCOREBOARD* sb);
void fetch_sb(SCOREBOARD* sb);
bool can_add_score(SCOREBOARD* sb, long score);
bool can_change_score(SCOREBOARD* sb, long score);
void add_sb_score(SCOREBOARD* sb, long score);
void change_score(SCOREBOARD* sb, long score);
void insertion_sort(SCOREBOARD* sb);
void write_to_file(SCOREBOARD* sb);

#endif