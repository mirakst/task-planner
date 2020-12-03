#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include "tasks.h"
#include "graphics.h"

/** Print a list of all non-empty tasks in the task array.
 *  @param[i] task_list Active array of task structures.
 *  @param[i] task_amount Amount of non-empty task structures in the array. */
void Print_Task_List (task *task_list, int task_amount, struct tm time, int print_all) {
    int i;
    if (task_amount == 0) {
        printf("There are currently no tasks. Enter 'add' to begin adding some.\n"); 
        return;
    }

    Sort_Task_List(task_list);

    Print_Line(1, "Tasklist");
    printf("%7s%6s%23s%13s%15s%8s\n", "ID", "Name", "Power", "Duration", "Energy usage", "Status");
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {
            printf("Task %2d: %-20s %4.1f kW %8.1f hrs %10.3f kWh %6c %6c\n", 
                    (i + 1),
                    task_list[i].name, 
                    task_list[i].power,
                    task_list[i].duration, 
                    task_list[i].kWh,
                    task_list[i].bool_is_passive == 1 ? 'P' : 'A',
                    task_list[i].days[time.tm_wday] ? 'Y' : 'N');
        }
    }
    Print_Line(0, "");
}

/** Load tasks from the config file.
 *  @param[i/o] task_list Active array of task structures.
 *  @param[i/o] task_amount Amount of non-empty task structures in the array. 
 *  @return Returns -1 if the file was not found, 0 if nothing was loaded, and 1 otherwise. */
int Load_Tasks (task *task_list, int *task_amount, char *file_location) {
    char temp_string[READ_LINE_MAX];
    FILE *fp = fopen(file_location, "r");

    if (fp == NULL)
        return -1;
    
    while (fgets(temp_string, READ_LINE_MAX, fp) && *task_amount < TASK_AMOUNT_MAX) {
        sscanf(temp_string, " %*s %[^,] %*c %*s %lf %*c %*s %lf %*c %*s %d %*c %*s %d %d %d %d %d %d %d",
               task_list[*task_amount].name,
               &task_list[*task_amount].power,
               &task_list[*task_amount].duration,
               &task_list[*task_amount].bool_is_passive,
               &task_list[*task_amount].days[0],
               &task_list[*task_amount].days[1],
               &task_list[*task_amount].days[2],
               &task_list[*task_amount].days[3],
               &task_list[*task_amount].days[4],
               &task_list[*task_amount].days[5],
               &task_list[*task_amount].days[6]);
        task_list[*task_amount].kWh = Calculate_kWh(task_list[*task_amount]);
        *task_amount += 1;
    }
    
    Sort_Task_List(task_list);
    
    fclose(fp);
    return *task_amount > 0 ? 1 : 0;
}

/** Calculate the kWh usage of a given task.
 *  @param[i] task A non-empty task structure from the task array. 
 *  @return Returns the kWh usage of the task as a double. */
double Calculate_kWh (task task) {
    return task.power * task.duration;
}

/** Save all tasks from the task array to the config file.
 *  @param[i] task_list Active array of task structures.
 *  @param[i] task_amount Amount of non-empty tasks in the array. 
 *  @return Returns -1 if the file could not be saved, and 1 otherwise. */
int Save_Tasks (task *task_list, int task_amount, char *file_location) {
    int i;
    FILE *fp = fopen(file_location, "w");
    
    if(task_amount == 0) 
        return 0;

    if (fp == NULL) 
        return -1;

    Sort_Task_List(task_list);
    for (i = 0; i < task_amount; i++) {
        fprintf(fp, "name: %s, power: %f, duration: %.1f, passive: %d, days: %d %d %d %d %d %d %d\n",
                task_list[i].name, task_list[i].power, task_list[i].duration, task_list[i].bool_is_passive,
                task_list[i].days[0], task_list[i].days[1], task_list[i].days[2], 
                task_list[i].days[3], task_list[i].days[4], task_list[i].days[5], 
                task_list[i].days[6]);
    }

    fclose(fp);
    return 1;
}

/** Reset all variables in the given task structure.
 *  @param[o] p_task Pointer to a task structure to be reset. */
void Reset_Task (task *p_task) {
    strcpy(p_task->name, EMPTY_TASK_NAME);
    p_task->bool_is_passive = 0;
    p_task->power = 0.0;
    p_task->duration = 0.0; 
    p_task->kWh = 0.0;
    p_task->start_hr = 0;
    p_task->end_hr = 0;
    p_task->is_assigned = 0;
    p_task->max_price = 0;
    p_task->min_price = 0;
}

/** Add a user-defined task to the task array.
 *  If the max task amount has been reached, or the user input is incorrect, the function returns before adding any task.
 *  @param[i/o] task_list Active array of task structures.
 *  @param[i/o] task_amount Pointer to the amount of non-empty tasks in the array. */
void Add_Task (task *task_list, int *task_amount, struct tm time) {
    int id = *task_amount;

    if (id >= TASK_AMOUNT_MAX) {
        printf("The maximum amount of tasks has been added.\n");
        return;
    }
    
    Set_Task_Name(task_list[id].name);
    Set_Task_Power(&task_list[id].power);
    Set_Task_Duration(&task_list[id].duration);
    Set_Task_Status(&task_list[id].bool_is_passive);
    Set_Task_Days(task_list[id].days);

    task_list[id].kWh = Calculate_kWh(task_list[id]);
    *task_amount += 1;

    if(!strcmp(task_list[id].name, EMPTY_TASK_NAME) || task_list[id].power <= 0 || task_list[id].duration <= 0) {
        printf("Error in user input, cancelling...\n"); 
        Reset_Task(&task_list[id]);
        return;
    }

    printf("Task: %s was successfully added.\n", task_list[id].name);
    Sort_Task_List(task_list);
}

/** Changes the name of a task.
 *  @param[i] name String that contains the name of the task. */
void Set_Task_Name (char *name) {
    printf("Task name (max %d): ", TASK_NAME_MAX);
    fgets(name, TASK_NAME_MAX, stdin);
    strtok(name, "\n");
}

/** Changes the power of a task.
 *  @param[i] power Power usage of the task (in Watts). */
void Set_Task_Power (double *power) {
    char temp_string[TASK_NAME_MAX];
    printf("Power usage (watts): ");
    fgets(temp_string, TASK_NAME_MAX, stdin);
    sscanf(temp_string, " %lf", power);
    *power /= W_PER_KW;
}

/** Changes the duration of a task.
 *  @param[i] duration Duration of the task (in hours). */
void Set_Task_Duration (double *duration) {
    char temp_string[TASK_NAME_MAX];
    printf("Task duration (min): ");
    fgets(temp_string, TASK_NAME_MAX, stdin);
    sscanf(temp_string, " %lf", duration);
    *duration /= MIN_PER_HOUR;
}

/** Changes the status of a task.
 *  @param[i] is_passive Boolean (int in C) that represents whether the task is passive or not (true = 1, false = 0). */
void Set_Task_Status (int *is_passive) {
    char temp_string[TASK_NAME_MAX],
         bool_input;
    printf("Is the task passive? (y/n): ");
    fgets(temp_string, TASK_NAME_MAX, stdin);
    sscanf(temp_string, " %c", &bool_input);
        
    if (tolower(bool_input) == 'y')
        *is_passive = 1;
    else 
        *is_passive = 0;
}

/** Changes the weekdays that a task is done.
 *  @param[o] days Array of booleans that decide whether a task is done on a certain weekday (true = 1, false = 0). */
void Set_Task_Days (int days[7]) {
    char temp_string[TASK_NAME_MAX],
         *weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    char bool_input;
    int i = 0;

    printf("What days should the task be done? (y/n)\n");
    for (i = 0; i < 7; i++) {
        printf("%s: ", weekdays[i]);
        fgets(temp_string, TASK_NAME_MAX, stdin);
        sscanf(temp_string, " %c", &bool_input);
            
        if (tolower(bool_input) == 'y')
            days[i] = 1;
        else 
            days[i] = 0;
    }
}

/** Remove a task from the task array.
 *  The selected task is effectively reset, since it cannot actually be removed from a static array in C.
 *  @param[i] task_list Active array of task structures.
 *  @param[i] task_amount Pointer to the amount of non-empty task structures in the array.
 *  @param[i] id The id/index of the task to be removed. */
void Remove_Task (task *task_list, int *task_amount, int id) {
    char task_name[TASK_NAME_MAX];
    id--;

    if (id < 0 || id >= *task_amount) {
        printf("The id was not recognized.\n");
        return;
    }
    strcpy(task_name, task_list[id - 1].name);

    strcpy(task_list[id].name, EMPTY_TASK_NAME);
    task_list[id].power = 0.0;
    task_list[id].duration = 0.0;
    task_list[id].kWh = 0.0;
    task_list[id].bool_is_passive = 0;
    task_list[id].start_hr = 0;
    task_list[id].end_hr = 0;
    task_list[id].is_assigned = 0;

    *task_amount -= 1;
    Sort_Task_List(task_list);
    printf("Task: %s was successfully removed.\n", task_name);
}


/** Sort the input task array.
 *  The compare function Compare_Tasks() prioritizes non-empty tasks with the highest kWh usage.
 *  @param[i] task_list Active array of task structures. */
void Sort_Task_List (task *task_list) {
    qsort(task_list, TASK_AMOUNT_MAX, sizeof(task), Compare_Tasks);
}

/** Compare function for qsort.
 *  Non-empty tasks with the highest kWh usage are prioritized. */
 int Compare_Tasks (const void *ip1, const void *ip2) {
    const task *task1 = (task *) ip1,
               *task2 = (task *) ip2;
        
    if (strcmp(task1->name, EMPTY_TASK_NAME) == 0 && strcmp(task2->name, EMPTY_TASK_NAME) != 0)
        return 1;
    else if (strcmp(task1->name, EMPTY_TASK_NAME) != 0 && strcmp(task2->name, EMPTY_TASK_NAME) == 0)
        return -1;
    else if (!task1->days[current_weekday] && task2->days[current_weekday])
        return 1;
    else if (task1->days[current_weekday] && !task2->days[current_weekday])
        return -1;
    else if (task1->kWh < task2->kWh)
        return 1;
    else if (task1->kWh > task2->kWh)
        return -1;
    else
        return 0;
}

/** Allows the user to edit a selected task in the task array. 
 * @param [i/o] task_list Active array of task structures. 
 * @param [i] task_amount Amount of non-empty tasks in the task array.
 * @param [i] task_id Selected task structure to be edited */
void Edit_Task (task *task_list, int task_amount, int task_id) {
    char temp_string[TASK_NAME_MAX];
    int input = 0,
        id = task_id - 1;

    if (id < 0 || id >= task_amount) {
        printf("The id was not recognized.\n");
        return;
    }

    Print_Line(1, "Edit Task");
    printf("Selected task: %s\n", task_list[id].name);
    printf("0: Go back\n"
            "1: Change name\n"
            "2: Change power\n"
            "3: Change duration\n"
            "4: Change status\n"
            "5: Change days of occurence\n");
    Print_Line(0, "");
    
    do {
        printf("Please select what you would like to change: ");
        fgets(temp_string, 5, stdin);
        sscanf(temp_string, " %d\n", &input);

        switch (input) {
        case 0:
            printf("Exiting task settings.\n");
            return;
        case 1:
            Set_Task_Name(task_list[id].name);
            break;
        case 2:
            Set_Task_Power(&task_list[id].power);
            break;
        case 3:
            Set_Task_Duration(&task_list[id].duration);
            break;
        case 4:
            Set_Task_Status(&task_list[id].bool_is_passive);
            break;
        case 5:
            Set_Task_Days(task_list[id].days);
            break;
        default:
            printf("The command was not recognized.\n"); 
            break;
        }
    } while (input);
}

/* Testing functions below */
void Test_Tasks(void) {
    task expected;
    strcpy(expected.name, "Name");
    expected.power = 3265.0;
    expected.duration = 60;
    expected.kWh = 3.265;
    expected.bool_is_passive = 0;
    
    assert(1 == Test_Load_Tasks(&expected));
    assert(1 == Test_Save_Tasks(&expected));
    assert(1 == Test_Remove_Task(&expected));
    assert(1 == Test_KW(expected));
}

int Test_Load_Tasks(task *expected) {
    int expected_task_amount = 0, result;
    result = Load_Tasks(expected, &expected_task_amount, FILE_CONTROL);
    return result;
}

int Test_Save_Tasks(task *expected) {
    int expected_task_amount = 1, result;
    result = Save_Tasks(expected, expected_task_amount, FILE_CONTROL);
    return result;
} 

int Test_Remove_Task(task *expected) {
    int remove_id = 1, expected_task_amount = 1;
    Remove_Task(expected, &expected_task_amount, remove_id);

    if (strcmp(expected[0].name, EMPTY_TASK_NAME) == 1)
        return 1;
    else
        return 0;
}

int Test_KW(task expected) {
    return expected.kWh == Calculate_kWh(expected) ? 1 : 0; 
}