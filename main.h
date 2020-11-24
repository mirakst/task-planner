/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/* Custom libraries */
#include "lib/tasks.h"
#include "lib/calculate_prices.h"
#include "lib/user_details.h"
#include "lib/graphics.h"

#define INPUT_MAX 40

/* Converts the input string to lower case */
void String_To_Lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
}

typedef enum command {cmd_exit, cmd_help, cmd_help_tasks, 
                      cmd_help_prices, cmd_prices, cmd_prices_sorted, 
                      cmd_list_tasks, cmd_task_add, cmd_task_remove, 
                      cmd_suggest, cmd_list_settings, cmd_availability,
                      cmd_reset_hours, cmd_save_user_details, cmd_unrecognized} command;

void String_To_Lower(char*);
int Compare_Command(char*);
void First_Time_Setup (User*, int*, int*);
void Print_Help (int, int);
void Suggest_Tasks (User, task*, int, double[HOURS_PR_DAY][2], double[HOURS_PR_DAY][2]);
int Get_Index_Unsorted (int, double[HOURS_PR_DAY][2], double[HOURS_PR_DAY][2]);