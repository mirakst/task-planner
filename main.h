/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <time.h>

/* Custom libraries */
#include "bin/tasks.h"
#include "bin/calculate_prices.h"
#include "bin/user_details.h"
#include "bin/graphics.h"
#include "bin/help.h"
#include "bin/get_input.h"

#define FUCKING_BIG_ASS_NUMBER 10000.0
#define INPUT_MAX 30
#define DAYS_PER_YEAR 365
#define FIRST_WEEKDAY_OF_YEAR 5
#define DAYS_PER_WEEK 7

/* Commands */
#define EXIT                  "exit\n"
#define LIST_DATA             "list data\n"
#define LIST_TASKS            "task list\n"
#define HELP                  "help\n"
#define HELP_TASKS            "help task\n"
#define HELP_PRICES           "help prices\n"
#define HELP_SETTINGS         "help settings\n"
#define ADD_TASK              "task add\n"
#define REMOVE_TASK           "task remove"
#define EDIT_TASK             "task edit"     
#define SUGGEST               "suggest\n"
#define SUGGEST_YEAR          "suggest year\n"
#define CHANGE_DAY            "change day\n"
#define SETTINGS              "settings\n"
#define SAVE_USER             "save\n"

typedef enum Commands {_exit, unrecognized,
                       help, help_tasks, help_prices, help_settings,
                       settings, change_day, save_user,
                       list_data, list_data_sorted,
                       list_tasks, add_task, remove_task, edit_task,
                       suggest, suggest_year
                       } Commands;

/* Prototypes */
void Initialize(double[][2], User *, task[TASK_AMOUNT_MAX], int *, int *);
void Save (User, task[TASK_AMOUNT_MAX], int);
void Suggest_Day (User, task[TASK_AMOUNT_MAX], int, double[][2], int);
void Suggest_Year (User, task[TASK_AMOUNT_MAX], int, double[][2], int); 
void Find_Start_Hour (User, task *, int[HOURS_PER_DAY], double[][2], int, int);
int Should_Skip_Hour (User, task *, int[HOURS_PER_DAY], int, int, int, int);
void Assign_Task (task *, int, int, double, double, int[HOURS_PER_DAY], int);

/* Test functions */
void Test_All(void);
void Test_Wrap_Hours(void);
void Test_Day_To_Weekday(void);

/** Converts the input string to lower case
 *  @param str[i/o] The string to be made lowercase. */
void String_To_Lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
}

/** Returns the input string as a command in the Commands enum.
 *  @param str[i] The input string.
 *  @return The Command enum equivalent to the input string. */
int Get_Command_From_String(char *str) {
    if (!strcmp(str, EXIT))
        return _exit;
    else if (!strcmp(str, HELP))
        return help;
    else if (!strcmp(str, HELP_PRICES))
        return help_prices;
    else if (!strcmp(str, HELP_TASKS))
        return help_tasks;
    else if (!strcmp(str, HELP_SETTINGS))
        return help_settings;
    else if (!strcmp(str, SETTINGS))
        return settings;
    else if (!strcmp(str, SAVE_USER))
        return save_user;
    else if (!strcmp(str, LIST_DATA))
        return list_data;
    else if (!strcmp(str, CHANGE_DAY))
        return change_day;
    else if (!strcmp(str, LIST_TASKS))
        return list_tasks;
    else if (!strcmp(str, ADD_TASK))
        return add_task;
    else if (strstr(str, REMOVE_TASK))
        return remove_task;
    else if (strstr(str, EDIT_TASK))
        return edit_task;
    else if (!strcmp(str, SUGGEST))
        return suggest;
    else if (!strcmp(str, SUGGEST_YEAR))
        return suggest_year;

    return unrecognized;
}

/** Gets input from the user. 
 *  @param input[o] String containing the user command.
 *  @return The command index in the command enum. */
int Get_User_Input (char *input) {
    printf("\nPlease enter a command: ");
    fgets(input, INPUT_MAX, stdin);
    String_To_Lower(input);
    return Get_Command_From_String(input);
}

/** Wraps the input hour if it surpasses the amount of hours in a day.
 *  @param hour[i] Amount of hours to be wrapped.
 *  @return Returns the input hour in the range 0-23. */
int Wrap_Hour (int hour) {
    return (hour >= HOURS_PER_DAY) ? (hour - HOURS_PER_DAY) : hour;
}

/** Converts the input day to its weekday.
 *  @param day[i] Day to be converted.
 *  @return Returns the input day as a weekday (0-6). */
int Day_To_Weekday (int day) {
    return (FIRST_WEEKDAY_OF_YEAR + ((day - 1) % DAYS_PER_WEEK)) % DAYS_PER_WEEK;
}