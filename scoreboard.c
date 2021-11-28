/* GRR20209948 Gabriel Pimentel Dolzan */

#include "scoreboard.h"

/* Initializes scoreboard structure */
void init_sb(SCOREBOARD* sb)
{
    sb->sb_size = 0;
    sb->scores_array = NULL;
}

/* Get scoreboard file "sb.info" */
void get_file(SCOREBOARD* sb)
{
    /* Verifies if file already exists */
    if(check_file_existence() == false)
    {
        /* Create file and set size to 0 */
        create_file(sb);
        sb->sb_size = 0;
    }
    else /* If files exists */
    {
        /* get size and fetch scores from file */
        get_size(sb);
        fetch_sb(sb);
    }
}

/* Add score to scoreboard */
void routine_add(SCOREBOARD* sb, long score)
{
    /* If there is no score saved */
    if(sb->sb_size == 0)
    {
        /* Sets array to null and verify if score can be added to scoreboard */
        sb->scores_array = NULL;
        if(can_add_score(sb, score) == true)
            add_sb_score(sb, score);
    }
    else if(sb->sb_size <= 10) /* If there are scores saved on scoreboard */
    {
        /* Allocate scores array and fetch scores from file */
        allocate_scores(sb);
        fetch_sb(sb);

        /* Verify if score can be added */
        if(can_add_score(sb, score) == true)
            add_sb_score(sb, score); /* Add score */
        else
            change_score(sb, score); /* Iterates through array to check if score should be overwritten */
        insertion_sort(sb); /* Applies insertion sort to array */
    }
}

/* Get size of scoreboard from file */
void get_size(SCOREBOARD* sb)
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "r");
    fscanf(f_score, "%d", &(sb->sb_size));
    fclose(f_score);
}

/* Check for file existence */
bool check_file_existence()
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "r");
    if(f_score == NULL)
        return false; /* File doesn't exist */
    fclose(f_score);
    return true; /* File exists */
}

/* Creates file */
void create_file(SCOREBOARD* sb)
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "w");
    fclose(f_score);
}

/* Print scores */
void print_scores(SCOREBOARD* sb)
{
    /* Function used for debugging */
    for(int i = 0; i < sb->sb_size; i++)
        printf("[%d] -> %ld", (i + 1), sb->scores_array[i]);
}

/* Get score size and information from file */
void fetch_sb(SCOREBOARD* sb)
{
    FILE* f_score;

    /* Checks file existence */
    if(check_file_existence() == false)
    {
        create_file(sb);
        return;
    }
    else /* File exists */
    {
        f_score = fopen("./resources/sb.info", "r");
        fscanf(f_score, "%d", &(sb->sb_size));
        if(sb->sb_size > 0) /* Has scores stored */
            allocate_scores(sb); /* Allocate said scores */
        else /* Size is less or equal than 0 */
            return; /* Nothing to allocate */

        /* Get scores and place them on scores_array */
        for(int i = 0; i < sb->sb_size; i++)
            fscanf(f_score, "%ld", &(sb->scores_array[i]));
        fclose(f_score);
    }
}

/* Allocate scores based on sb_size */
void allocate_scores(SCOREBOARD* sb)
{
    sb->scores_array = (long*)malloc(sb->sb_size * sizeof(long));
    if(sb->scores_array == NULL)
        perror("Insufficient memory:");
}

/* Verifies if score can be added to scores_array */
bool can_add_score(SCOREBOARD* sb, long score)
{
    if(sb->sb_size <= 9)
        return true;
    return false;
}

/* Verifies if score can be overwritten to scores_array */
bool can_change_score(SCOREBOARD* sb, long score)
{
    if(sb->sb_size == 10) /* Only if all score spaces are full */
    {
        for(int i = 0; i < sb->sb_size; i++) /* Iterate through array */
        {
            if(score > sb->scores_array[i]) /* If score is higher than score inside array, you can overwrite */
                return true;
        }
    }
    return false; /* Can't overwrite */
}

/* Function resizes and add scores based on score value */
void add_sb_score(SCOREBOARD* sb, long score)
{
    long* score_aux;

    /* Checks if can add a slot to scores_array */
    if(sb->sb_size <= 9)
        sb->sb_size = sb->sb_size + 1; /* Added a score slot */
    score_aux = (long*)malloc(sb->sb_size * sizeof(long)); /* Allocate score slot */
    if(score_aux == NULL)
        perror("Insufficient memory:");
            
    /* Copies all existing scores to new array */
    for(int i = 0; i < (sb->sb_size - 1); i++)
        score_aux[i] = sb->scores_array[i];
    score_aux[(sb->sb_size - 1)] = score; /* Places new score at the new slot */

    /* Checks if free can be performed */
    if(sb->sb_size > 0 && sb->scores_array != NULL)
        free(sb->scores_array); /* Frees old array */
    sb->scores_array = score_aux; /* Replaces array with new one */
}

/* If all slots are created, overwrite score to end of scoreboard */
void change_score(SCOREBOARD* sb, long score)
{
    for(int i = 0; i < sb->sb_size; i++) /* Iterates through array */
    {
        if(score > sb->scores_array[i]) /* if score is higher than any score inside scores_array */
        {
            sb->scores_array[0] = score; /* Replaces lowest score */
            return;
        }
    }
}

/* Basic insertion sort algorithm to sort scores in a ascending order */
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

/* Based on score structure, writes new info to file */
void write_to_file(SCOREBOARD* sb)
{
    FILE* f_score;
    f_score = fopen("./resources/sb.info", "w");

    fprintf(f_score, "%d\n", sb->sb_size);

    for(int i = 0; i < sb->sb_size; i++)
        fprintf(f_score, "%ld\n", sb->scores_array[i]);
    fclose(f_score);
}