/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/* Custom libraries */
#include "bin/tasks.h"
#include "bin/calculate_prices.h"
#include "bin/user_details.h"
#include "bin/graphics.h"
#include "bin/help.h"

#define INPUT_MAX 40

/* Commands */
#define EXIT "exit\n"
#define LIST_PRICES "list prices\n"
#define LIST_PRICES_SORTED "list prices sorted\n"
#define LIST_TASKS "list tasks\n"
#define HELP "help\n"
#define HELP_TASKS "help tasks\n"
#define HELP_PRICES "help prices\n"
#define HELP_SETTINGS "help settings\n"
#define HELP_USER_DETAILS "help user details\n"
#define ADD_TASK "task add\n"
#define REMOVE_TASK "task remove"
#define SUGGEST "suggest\n"
#define LIST_SETTINGS "list settings\n"
#define SET_NAME "set name\n"
#define RESET_HOURS "reset hours\n"
#define SET_HOURS "set hours\n"
#define SAVE_USER "save\n"

/* Converts the input string to lower case */
void String_To_Lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
}

void Initialize (double[][2], double[][2], User *, task[TASK_AMOUNT_MAX], int *);
void Save (User, task[TASK_AMOUNT_MAX], int);
void Suggest_Tasks (User, task *, int, double[][2], double[][2]);
double Calculate_Max_Price (double[][2], double[][2], double, task);



























































































































































void Save_User_Value(void){
    printf("\nFuck Nikolaj");
    return;
}