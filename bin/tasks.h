#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include "graphics.h"

#define FILE_TASKLIST           "config/tasklist.cfg"
#define MIN_PER_HOUR             60
#define W_PER_KW                 1000
#define G_PER_KG                 1000
#define READ_LINE_MAX            100
#define TASK_NAME_ALLOC          100
#define EMPTY_TASK_NAME         "none"
#define TASK_AMOUNT_MAX          5000
#define TASK_NAME_MAX            21
#define TASK_DURATION_MAX        1440

typedef enum task_type {active, passive, timed} task_type;

/** The task structure.
 *  Contains all information needed for a task.
 *  @param type Enum that determines if the task is Active, Passive or Timed. (0, 1 & 2).
 *  @param name String that contains the name of the task.
 *  @param power Double that contains the power of the task. 
 *  @param kWh Double that contains the power consumption of the task.
 *  @param duration Double that contains the amount of time the task takes to complete.
 *  @param start_hr Int for the start time of the task.
 *  @param end_hr Int for the end time of the task.
 *  @param days[] An array of the days in a week (there are 7).
 *  @param total_days_yr Int for calculating the amount of days task is performed throughout the year.
 *  @param min_value Double for the minimum value of completing the task.
 *  @param max_value Double for the maximum value of completing the task. */
typedef struct task {
    int type;
    char name[TASK_NAME_MAX];
    double power,
           kWh,
           duration;
    
    int start_hr,
        end_hr,
        days[7],
        total_days_yr;
    double min_value,
           max_value;
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

void Sort_Task_List (task *, int);
int Compare_Tasks (const void *, const void *);
void Print_Task_List(task *, int, int);
void Print_Suggestions_Day (int, task[TASK_AMOUNT_MAX], int, int);
void Print_Suggestions_Year (int, task[TASK_AMOUNT_MAX], int);
double Fixed_Percent(double, double);

int Get_Bool_Input(void);
double Get_Double_Input(int);

/* Testing */
void Test_KW(void);
void Test_Fixed_Percent(void);