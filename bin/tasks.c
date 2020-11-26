/* TODO: 
 * Test-cases (too long task name, char instead of int, etc...)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    printf("%7s%6s%23s%13s%15s%8s\n", "ID", "Name", "Power", "Duration", "Energy usage", "Status");
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {
            printf("Task %2d: %-20s %4d W %8d min %10.3f kWh %7c\n", (i + 1),
                    task_list[i].name, 
                    task_list[i].power,
                    task_list[i].duration_min, 
                    task_list[i].kW / task_list[i].duration_hrs, 
                    task_list[i].bool_is_passive == 1 ? 'P' : 'A');
        }
    }
    Print_Line(0, "");
}

/* Loads tasks from the config file. Returns 0 if nothing is loaded */
int Load_Tasks (task *task_list, int *task_amount) {
    FILE *fp;
    char temp_string[READ_LINE_MAX];

    fp = fopen(FILE_TASKLIST, "r");
    if (fp == NULL)
        return -1;
    
    while (fgets(temp_string, READ_LINE_MAX, fp) && *task_amount < TASK_AMOUNT_MAX) {
        sscanf(temp_string, " %*s %[^,] %*c %*s %d %*c %*s %d %*c %*s %d",
               task_list[*task_amount].name,
               &task_list[*task_amount].power,
               &task_list[*task_amount].duration_min,
               &task_list[*task_amount].bool_is_passive);
    
        task_list[*task_amount].duration_hrs = Calculate_Hours(task_list[*task_amount]);
        task_list[*task_amount].kW = Calculate_kW(task_list[*task_amount]);
        *task_amount += 1;
    }
    
    Sort_Task_List(task_list);
    
    fclose(fp);
    return *task_amount > 0 ? 1 : 0;
}

/* Returns the kW usage of the input task */
double Calculate_kW (task task) {
    return (double)task.power / W_PER_KW;
}

double Calculate_Hours (task task) {
    return (double)task.duration_min / MIN_PER_HOUR;
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
        fprintf(fp, "name: %s, power: %d, duration: %d, passive: %d\n",
                task_list[i].name, task_list[i].power, task_list[i].duration_min, task_list[i].bool_is_passive);
    }

    fclose(fp);
    return 1;    
}

/* Initializes the input task list to be empty */
void Initialize_Tasks (task *task_list, int *task_amount) {
    int i;
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        strcpy(task_list[i].name, EMPTY_TASK_NAME);
        task_list[i].bool_is_passive = 0;
        task_list[i].power = 0;
        task_list[i].duration_min = 0;
        task_list[i].duration_hrs = 0.0; 
        task_list[i].kW = 0.0;
        task_list[i].start_hr = 0;
    }
}

/* Adds a user-defined task to the task list */
void Add_Task (task *task_list, int *task_amount) {
    task result;
    char name[TASK_NAME_MAX],
         temp[TASK_NAME_MAX];
    int power = 0, duration = 0, bool_is_passive = 0;

    if (*task_amount >= TASK_AMOUNT_MAX) {
        printf("The maximum amount of tasks has been added.\n");
        return;
    }
    
    Get_Task_Input(temp, name, &power, &duration, &bool_is_passive);

    if(!strcmp(name, EMPTY_TASK_NAME) || power <= 0 || duration <= 0) {
        printf("Error in user input, cancelling...\n"); 
        return;
    }

    strcpy(result.name, name);
    result.power = power;
    result.duration_min = duration;
    result.duration_hrs = Calculate_Hours(result);
    result.kW = Calculate_kW(result);
    result.bool_is_passive = bool_is_passive;

    task_list[*task_amount] = result;
    *task_amount += 1;

    Sort_Task_List(task_list);
    printf("Task: %s was successfully added.\n", name);
}

/* Get input for a new task before adding it to the task list */
void Get_Task_Input (char *temp, char *name, int *power, int *duration, int *is_passive) {
    char bool_input;

    printf("Task name (max %d): ", TASK_NAME_MAX);
    fgets(name, TASK_NAME_MAX, stdin);
    strtok(name, "\n");

    printf("Power usage (watts): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    sscanf(temp, " %d", power);

    printf("Task duration (min): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    sscanf(temp, " %d", duration);

    printf("Is the task passive? (y/n): ");
    fgets(temp, TASK_NAME_MAX, stdin);
    sscanf(temp, " %c", &bool_input);
        
    if (tolower(bool_input) == 'y')
        *is_passive = 1;
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
    result.duration_min = 0;
    result.duration_hrs = 0.0;
    result.kW = 0.0;
    result.bool_is_passive = 0;
    result.start_hr = 0;

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
    
    if ((task1->kW / task1->duration_hrs) > (task2->kW / task2->duration_hrs))
        return -1;
    else if ((task1->kW / task1->duration_hrs) < (task2->kW / task2->duration_hrs))
        return 1;
    else
        return 0;
}
