#include "scoreboard.h"

void init_sb(SCOREBOARD* sb)
{
    sb->sb_size = 0;
    sb->scores_array = NULL;
}

void get_file(SCOREBOARD* sb)
{
    if(check_file_existence() == false)
    {
        printf("Created score file, because it couldn't be found!\n");
        create_file(sb);
        sb->sb_size = 0;
    }
    else
    {
        get_size(sb);
        fetch_sb(sb);
    }
}

void routine_add(SCOREBOARD* sb, long score)
{
    if(sb->sb_size == 0)
    {
        sb->scores_array = NULL;
        if(can_add_score(sb, score) == true)
            add_sb_score(sb, score);
    }
    else if(sb->sb_size <= 10)
    {
        allocate_scores(sb);
        fetch_sb(sb);
        if(can_add_score(sb, score) == true)
            add_sb_score(sb, score);
        else
            change_score(sb, score);
        insertion_sort(sb);
    }
}

void get_size(SCOREBOARD* sb)
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "r");
    fscanf(f_score, "%d", &(sb->sb_size));
    fclose(f_score);
}

bool check_file_existence()
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "r");
    if(f_score == NULL)
        return false;
    fclose(f_score);
    return true;
}

void create_file(SCOREBOARD* sb)
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "w");
    fclose(f_score);
}

void print_scores(SCOREBOARD* sb)
{
    for(int i = 0; i < sb->sb_size; i++)
        printf("[%d] -> %ld", (i + 1), sb->scores_array[i]);
}

void fetch_sb(SCOREBOARD* sb)
{
    FILE* f_score;

    if(check_file_existence() == false)
    {
        create_file(sb);
        fprintf(stderr, "Just created file, there is nothing to be fetched, returning...\n");
        return;
    }
    else
    {
        f_score = fopen("./resources/sb.info", "r");
        fscanf(f_score, "%d", &(sb->sb_size));
        if(sb->sb_size > 0)
            allocate_scores(sb);
        else
        {
            fprintf(stderr, "There is nothing to be fetched, returning...\n");
            return;
        }

        for(int i = 0; i < sb->sb_size; i++)
            fscanf(f_score, "%ld", &(sb->scores_array[i]));
        fclose(f_score);
    }
}

void allocate_scores(SCOREBOARD* sb)
{
    sb->scores_array = (long*)malloc(sb->sb_size * sizeof(long));
    if(sb->scores_array == NULL)
        perror("Insufficient memory:");
}

bool can_add_score(SCOREBOARD* sb, long score)
{
    if(sb->sb_size <= 9)
        return true;
    return false;
}

bool can_change_score(SCOREBOARD* sb, long score)
{
    if(sb->sb_size == 10)
    {
        for(int i = 0; i < sb->sb_size; i++)
        {
            if(score > sb->scores_array[i])
                return true;
        }
    }
    return false;
}

void add_sb_score(SCOREBOARD* sb, long score)
{
    long* score_aux;

    if(sb->sb_size <= 9)
        sb->sb_size = sb->sb_size + 1;
    printf("%d\n", sb->sb_size);
    score_aux = (long*)malloc(sb->sb_size * sizeof(long));
    if(score_aux == NULL)
        perror("Insufficient memory:");
            
    for(int i = 0; i < (sb->sb_size - 1); i++)
        score_aux[i] = sb->scores_array[i];
    score_aux[(sb->sb_size - 1)] = score;

    if(sb->sb_size > 0 && sb->scores_array != NULL)
        free(sb->scores_array);
    sb->scores_array = score_aux;
}

void change_score(SCOREBOARD* sb, long score)
{
    for(int i = 0; i < sb->sb_size; i++)
    {
        if(score > sb->scores_array[i])
        {
            sb->scores_array[0] = score;
            return;
        }
    }
}

void insertion_sort(SCOREBOARD* sb)
{
    int j;
    long key;

    for(int i = 0; i < sb->sb_size; i++)
    {
        key = sb->scores_array[i];
        j = i - 1;

        while(j >= 0 && sb->scores_array[j] > key)
        {
            sb->scores_array[j + 1] = sb->scores_array[j];
            j = j - 1;
        }
        sb->scores_array[j + 1] = key;
    }
}

void write_to_file(SCOREBOARD* sb)
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "w");

    fprintf(f_score, "%d\n", sb->sb_size);

    for(int i = 0; i < sb->sb_size; i++)
        fprintf(f_score, "%ld\n", sb->scores_array[i]);
    fclose(f_score);
}