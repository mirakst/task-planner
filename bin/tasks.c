#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "tasks.h"
#include "graphics.h"

/** Prints a list of all non-empty tasks.
 *  @param[i] task_list The active array of tasks.
 *  @param[i] task_amount An int that keeps track of the amount of non-empty tasks in the array. 
 */
void Print_Task_List (task *task_list, int task_amount) {
    int i;
    if (task_amount == 0) {
        printf("There are currently no tasks. Enter 'add' to begin adding some.\n"); 
        return;
    }

    Print_Line(1, "Tasklist");
    printf("%7s%6s%23s%13s%15s%8s\n", "ID", "Name", "Power", "Duration", "Energy usage", "Status");
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {
            printf("Task %2d: %-20s %4d W %8.1f hrs %10.3f kWh %7c\n", 
                    (i + 1),
                    task_list[i].name, 
                    task_list[i].power,
                    task_list[i].duration, 
                    task_list[i].kW * task_list[i].duration, 
                    task_list[i].bool_is_passive == 1 ? 'P' : 'A');
        }
    }
    Print_Line(0, "");
}

/** Loads tasks from the config file.
 *  @param[i] task_list The active array of tasks.
 *  @param[i] task_amount An int that keeps track of the amount of non-empty tasks in the array. 
 *  @return Returns -1 if the file was not found, 0 if nothing was loaded, and 1 otherwise.
 */
int Load_Tasks (task *task_list, int *task_amount) {
    FILE *fp;
    char temp_string[READ_LINE_MAX];

    fp = fopen(FILE_TASKLIST, "r");
    if (fp == NULL)
        return -1;
    
    while (fgets(temp_string, READ_LINE_MAX, fp) && *task_amount < TASK_AMOUNT_MAX) {
        sscanf(temp_string, " %*s %[^,] %*c %*s %d %*c %*s %lf %*c %*s %d",
               task_list[*task_amount].name,
               &task_list[*task_amount].power,
               &task_list[*task_amount].duration,
               &task_list[*task_amount].bool_is_passive);
    
        task_list[*task_amount].kW = Calculate_kW(task_list[*task_amount]);
        *task_amount += 1;
    }
    
    Sort_Task_List(task_list);
    
    fclose(fp);
    return *task_amount > 0 ? 1 : 0;
}

/** Calculates the kWh usage of a given task.
 *  @param[i] task A non-empty task from the task array. 
 *  @return Returns the kWh usage of the task as a double. */
double Calculate_kW (task task) {
    return (double)task.power / W_PER_KW;
}

/** Saves all tasks from the task array to the config file.
 *  @param[i] task_list The active array of tasks.
 *  @param[i] task_amount An int that keeps track of the amount of non-empty tasks in the array. 
 *  @return Returns -1 if the file could not be saved, and 1 otherwise. */
int Save_Tasks (task *task_list, int task_amount) {
    FILE *fp;
    int i;

    if(task_amount == 0) 
        return 0;

    fp = fopen(FILE_TASKLIST, "w");
    if (fp == NULL) 
        return -1;

    Sort_Task_List(task_list);
    for (i = 0; i < task_amount; i++) {
        fprintf(fp, "name: %s, power: %d, duration: %.1f, passive: %d\n",
                task_list[i].name, task_list[i].power, task_list[i].duration, task_list[i].bool_is_passive);
    }

    fclose(fp);
    return 1;    
}

/** Initializes the values of all possible tasks in the task array to 0.
 *  @param[i] task_list The active array of tasks. */
void Initialize_Tasks (task *task_list) {
    int i;
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        strcpy(task_list[i].name, EMPTY_TASK_NAME);
        task_list[i].bool_is_passive = 0;
        task_list[i].power = 0;
        task_list[i].duration = 0.0; 
        task_list[i].kW = 0.0;

        task_list[i].start_hr = 0;
        task_list[i].end_hr = 0;
        task_list[i].is_assigned = 0;
        task_list[i].max_price = 0;
        task_list[i].min_price = 0;
    }
}

/** Adds a user-defined task to the task array.
 *  If the max task amount has been reached, or the user input is incorrect, the function returns before adding any task.
 *  @param[i] task_list The active array of tasks.
 *  @param[i] task_amount A pointer to an int that keeps track of the amount of non-empty tasks in the array. */
void Add_Task (task *task_list, int *task_amount) {
    task result;
    char name[TASK_NAME_MAX];
    int power = 0, bool_is_passive = 0;
    double duration = 0.0;

    if (*task_amount >= TASK_AMOUNT_MAX) {
        printf("The maximum amount of tasks has been added.\n");
        return;
    }
    
    Get_Task_Input(name, &power, &duration, &bool_is_passive);

    if(!strcmp(name, EMPTY_TASK_NAME) || power <= 0 || duration <= 0) {
        printf("Error in user input, cancelling...\n"); 
        return;
    }

    strcpy(result.name, name);
    result.power = power;
    result.duration = duration;
    result.kW = Calculate_kW(result);
    result.bool_is_passive = bool_is_passive;

    task_list[*task_amount] = result;
    *task_amount += 1;

    Sort_Task_List(task_list);
    printf("Task: %s was successfully added.\n", name);
}

/** Gets user input for the Add_Task() function.
 *  @param[i] name A string that contains the name of the task.
 *  @param[i] power An int that represents the power usage of the task (in Watts).
 *  @param[i] duration A double that represents the duration of the task (in hours).
 *  @param[i] is_passive A boolean (int in C) that represents whether the task is passive or not (true = 1, false = 0). */
void Get_Task_Input (char *name, int *power, double *duration, int *is_passive) {
    char temp[TASK_NAME_MAX];
    char bool_input;

    printf("Task name (max %d): ", TASK_NAME_MAX);
    fgets(name, TASK_NAME_MAX, stdin);
    strtok(name, "\n");

    printf("Power usage (watts): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    sscanf(temp, " %d", power);

    printf("Task duration (hrs): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    sscanf(temp, " %lf", duration);

    printf("Is the task passive? (y/n): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    sscanf(temp, " %c", &bool_input);
        
    if (tolower(bool_input) == 'y')
        *is_passive = 1;
}

/** Removes a task from the task array.
 *  The selected task is effectively reset, since it cannot actually be removed from a static array in C.
 *  @param[i] task_list The active array of tasks.
 *  @param[i] task_amount A pointer to an int that keeps track of the amount of non-empty tasks in the array.
 *  @param[i] id A user-input int that represents the id/index of the task to be removed. */
void Remove_Task (task *task_list, int *task_amount, int id) {
    task result;
    char task_name[TASK_NAME_MAX];

    if (id < 1 || id > *task_amount) {
        printf("The id was not recognized.\n");
        return;
    }
    strcpy(task_name, task_list[id - 1].name);

    strcpy(result.name, EMPTY_TASK_NAME);
    result.power = 0;
    result.duration = 0.0;
    result.kW = 0.0;
    result.bool_is_passive = 0;
    result.start_hr = 0;
    result.end_hr = 0;
    result.is_assigned = 0;

    task_list[id - 1] = result;
    *task_amount -= 1;
    Sort_Task_List(task_list);
    printf("Task: %s was successfully removed.\n", task_name);
}

/** Sorts the input task array.
 *  The compare function \fn(Compare_Tasks) prioritizes non-empty tasks with the highest kWh usage.
 *  @param[i] task_list The active array of tasks. */
void Sort_Task_List (task *task_list) {
    qsort(task_list, TASK_AMOUNT_MAX, sizeof(task), Compare_Tasks);
}

/** Compare function for qsort.
 *  Non-empty tasks with the highest kWh usage are prioritized.
 *  @param[i] ip1 The first element to be compared.
 *  @param[i] ip1 The second element to be compared. */
 int Compare_Tasks (const void *ip1, const void *ip2) {
    const task *task1 = (task *) ip1,
               *task2 = (task *) ip2;
    
    if ((task1->kW * task1->duration) > (task2->kW * task2->duration))
        return -1;
    else if ((task1->kW * task1->duration) < (task2->kW * task2->duration))
        return 1;
    else
        return 0;
}