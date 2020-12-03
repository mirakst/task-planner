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

#define INPUT_MAX 20
#define DAYS_PER_YEAR 365
#define FIRST_WEEKDAY_OF_YEAR 3
#define DAYS_PER_WEEK 7

/* Commands */
#define EXIT                "exit\n"
#define LIST_PRICES         "list prices\n"
#define LIST_PRICES_SORTED  "list prices sorted\n"
#define LIST_TASKS          "task list\n"
#define LIST_ALL_TASKS      "task list all\n"
#define HELP                "help\n"
#define HELP_TASKS          "help task\n"
#define HELP_PRICES         "help prices\n"
#define HELP_SETTINGS       "help settings\n"
#define HELP_USER_DETAILS   "help user details\n"
#define RECOMMEND           "recommend\n"
#define ADD_TASK            "task add\n"
#define REMOVE_TASK         "task remove"
#define EDIT_TASK           "task edit"
#define SUGGEST             "suggest\n"
#define SUGGEST_YEAR        "suggest year\n"
#define CHANGE_DAY          "change day\n"
#define SETTINGS            "settings\n"
#define SAVE_USER           "save\n"

/*#define LIST_SETTINGS       "list settings\n"
#define SET_NAME            "set name\n"
#define RESET_HOURS         "reset hours\n"
#define SET_HOURS           "set hours\n"
*/

/** Converts the input string to lower case */
void String_To_Lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
}

void Get_User_Input (char *);
void Initialize (double[][2], double[][2], User *, task[TASK_AMOUNT_MAX], struct tm, int *);
void Save (User, task[TASK_AMOUNT_MAX], int);

void Find_Lowest_Price (User, task*, task[TASK_AMOUNT_MAX], int, double[][2], int);
int Wrap_Hour (int);
double Fixed_Percent (double, double);
int Day_To_Weekday (int);
void Print_Suggestions (int, task[TASK_AMOUNT_MAX], int, struct tm);

/* Testing */
void Test_All(void);