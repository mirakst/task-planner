#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include "graphics.h"

#define FILE_TASKLIST           "config/tasklist.cfg"
#define EMPTY_TASK_NAME         "none"
#define TASK_AMOUNT_MAX         20
#define TASK_NAME_ALLOC         100
#define TASK_NAME_MAX           21
#define MIN_PER_HOUR            60
#define W_PER_KW                1000
#define READ_LINE_MAX           100

/** The task structure.
 *  Contains all information needed for a task.
 *  @param is_passive Boolean that determines if the task is passive (1 = true, 0 = false).
 *  @param name String that contains the name of the task.
 *  @param power Double that contains the power of the task. 
 *  @param kWh Double that contains the power consumption of the task.
 *  @param duration Double that contains the amount of time the task takes to complete.
 *  @param is_assigned Boolean that decides if a task is assigned to a start hour.
 *  @param start_hr Int for the start time of the task.
 *  @param end_hr Int for the end time of the task.
 *  @param days[] An array of the days in a week (there are 7).
 *  @param min_price Double for the minimum price of completing the task.
 *  @param max_price Double for the maximum price of completing the task. */
typedef struct task {
    int is_passive;
    char name[TASK_NAME_MAX];
    double power,
           kWh,
           duration;
    
    /* Assignment */
    int is_assigned,
        start_hr,
        end_hr,
        days[7],
        total_days_yr;
    double min_price,
           max_price;
} task;

double Calculate_kWh (task);

void Reset_Task (task *);
int Load_Tasks (task *, int *, char *);
int Save_Tasks (task *, int, char *);

void Add_Task (task *, int *);
void Remove_Task (task *, int *, int);
void Edit_Task (task *, int, int);

void Set_Task_Status (int *);
void Set_Task_Duration (double *);
void Set_Task_Power (double *);
void Set_Task_Name (char *);
void Set_Task_Days (int *);

void Sort_Task_List (task *);
int Compare_Tasks (const void *, const void *);
void Print_Task_List(task *, int, struct tm);
void Print_Suggestions_Day (int, task[TASK_AMOUNT_MAX], struct tm);
void Print_Suggestions_Year (int, task[TASK_AMOUNT_MAX]);
double Fixed_Percent(double, double);

/* Testing */
void Test_KW(void);