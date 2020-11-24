/* TODO: 
 * Test-cases (too long task name, char instead of int, etc...)
 * Bool in task struct (get_suggestions?)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"
#include "graphics.h"

/* Prints a list of all non-empty tasks */
void Print_Task_List (task *task_list, int task_amount) {
    int i;
    if (task_amount == 0) {
        printf("There are currently no tasks. Enter 'add' to begin adding some.\n"); 
        return;
    }

    Print_Line(1, "Tasklist");
    printf("%7s%6s%23s%13s%15s\n", "ID", "Name", "Power", "Duration", "Energy usage");    
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {
            printf("Task %2d: %-20s %4d W %8d min %10.3lf kWh\n", (i + 1),
                    task_list[i].name, task_list[i].power, task_list[i].duration, task_list[i].kWh);
        }
    }
    Print_Line(0, "");
}

/* Loads tasks from the config file. Returns 0 if nothing is loaded */
int Load_Tasks (task *task_list, int *task_amount) {
    FILE *fp;
    char temp_string[50], task_name[TASK_NAME_MAX];
    int task_power, task_duration;

    fp = fopen(FILE_TASKLIST, "r");
    if (fp == NULL)
        return -1;
    
    while (fgets(temp_string, 50, fp) && *task_amount < TASK_AMOUNT_MAX) {
        sscanf(temp_string, " %*s %[^,] %*c %*s %d %*c %*s %d",
               task_name, &task_power, &task_duration);
    
        strcpy(task_list[*task_amount].name, task_name);
        task_list[*task_amount].power = task_power;
        task_list[*task_amount].duration = task_duration;
        task_list[*task_amount].kWh = (task_power * ((double)task_duration / 60.0)) / 1000;
        *task_amount += 1;
    }
    Sort_Task_List(task_list);
    
    fclose(fp);
    return *task_amount > 0 ? 1 : 0;
}

/* Saves all tasks to the config file. Returns 0 if nothing is saved */
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
        fprintf(fp, "name: %s, power: %d, duration: %d\n",
                task_list[i].name, task_list[i].power, task_list[i].duration);
    }

    fclose(fp);
    return 1;    
}

/* Initializes the input task list to be empty */
void Initialize_Tasks (task *task_list, int *task_amount) {
    int i;
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        strcpy(task_list[i].name, EMPTY_TASK_NAME);
        task_list[i].power = 0;
        task_list[i].duration = 0; 
        task_list[i].kWh = 0.0;
    }
}

/* Gets task details from the user and adds it to the input array */
void Add_Task (task *task_list, int *task_amount) {
    task result;
    char name[TASK_NAME_MAX],
         temp[TASK_NAME_MAX];
    int power = 0, duration = 0;

    if (*task_amount >= TASK_AMOUNT_MAX) {
        printf("The maximum amount of tasks has been added.\n");
        return;
    }
    
    printf("Task name (max %d): ", TASK_NAME_MAX);
    fgets(name, TASK_NAME_MAX, stdin);
    strtok(name, "\n");
    
    printf("Power usage (watts): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    if(sscanf(temp, " %d", &power) == 0)
        return;

    printf("Task duration (min): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    if(sscanf(temp, " %d", &duration) == 0)
        return;

    if(!strcmp(name, EMPTY_TASK_NAME) || power <= 0 || duration <= 0) {
        printf("Error in user input, cancelling...\n"); 
        return;
    }

    strcpy(result.name, name);
    result.power = power;
    result.duration = duration;
    result.kWh = (power * ((double)duration / 60.0)) / 1000;

    task_list[*task_amount] = result;
    *task_amount += 1;
    Sort_Task_List(task_list);
    printf("Task: %s was successfully added.\n", name);
}

/* Empties a task in the input array at the given index */
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
    result.duration = 0;
    result.kWh = 0.0;

    task_list[id - 1] = result;
    *task_amount -= 1;
    Sort_Task_List(task_list);
    printf("Task: %s was successfully removed.\n", task_name);
}

/* Sorts the input array with qsort and the Compare_Tasks function */
void Sort_Task_List (task *task_list) {
    qsort(task_list, TASK_AMOUNT_MAX, sizeof(task), Compare_Tasks);
}

/* Compare two tasks and prioritize non-empty tasks */
int Compare_Tasks (const void *ip1, const void *ip2) {
    const task *task1 = (task *) ip1,
               *task2 = (task *) ip2;
    
    if (task1->kWh > task2->kWh)
        return -1;
    else if (task1->kWh < task2->kWh)
        return 1;
    else
        return 0;
}
