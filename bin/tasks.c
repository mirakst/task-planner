#include "tasks.h"

/** Print a list of all non-empty tasks in the task array.
 *  @param task_list[i] Active array of task structures.
 *  @param task_amount[i] Amount of non-empty task structures in the array. */
void Print_Task_List (task *task_list, int task_amount, struct tm time) {
    int i;
    if (task_amount == 0) {
        printf("There are currently no tasks. Enter 'add' to begin adding some.\n"); 
        return;
    }

    Sort_Task_List(task_list);

    Print_Line(1, "Tasklist");
    printf("%7s%6s%23s%14s%15s%9s%8s\n", "ID", "Name", "Power", "Duration", "Energy usage", "Status", "Today?");
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {
            printf("Task %2d: %-20s %4.1f kW %8.1f hrs %10.3f kWh %6c %6c\n", 
                    (i + 1),
                    task_list[i].name, 
                    task_list[i].power,
                    task_list[i].duration, 
                    task_list[i].kWh,
                    task_list[i].is_passive == 1 ? 'P' : 'A',
                    task_list[i].days[time.tm_wday] ? 'Y' : 'N');
        }
    }
    Print_Line(0, "");
}

/** Load tasks from the config file.
 *  @param task_list[i/o] Active array of task structures.
 *  @param task_amount[i/o] Amount of non-empty task structures in the array. 
 *  @return -1 if the file was not found, 0 if nothing was loaded, and 1 otherwise. */
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
               &task_list[*task_amount].is_passive,
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
 *  @param task[i] A non-empty task structure from the task array. 
 *  @return The kWh usage of the task as a double. */
double Calculate_kWh (task task) {
    return task.power * task.duration;
}

/** Save all tasks from the task array to the config file.
 *  @param task_list[i] Active array of task structures.
 *  @param task_amount[i]  Amount of non-empty tasks in the array. 
 *  @return -1 if the file could not be saved, and 1 otherwise. */
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
                task_list[i].name, task_list[i].power, task_list[i].duration, task_list[i].is_passive,
                task_list[i].days[0], task_list[i].days[1], task_list[i].days[2], 
                task_list[i].days[3], task_list[i].days[4], task_list[i].days[5], 
                task_list[i].days[6]);
    }

    fclose(fp);
    return 1;
}

/** Reset all variables in the given task structure.
 *  @param p_task[o] Pointer to a task structure to be reset. */
void Reset_Task (task *p_task) {
    strcpy(p_task->name, EMPTY_TASK_NAME);
    p_task->is_passive = 0;
    p_task->power = 0.0;
    p_task->duration = 0.0; 
    p_task->kWh = 0.0;
    p_task->start_hr = 0;
    p_task->end_hr = 0;
    p_task->is_assigned = 0;
    p_task->max_price = 0;
    p_task->min_price = 0;
    p_task->total_days_yr = 0;
}

/** Add a user-defined task to the task array.
 *  If the max task amount has been reached, or the user input is incorrect, the function returns before adding any task.
 *  @param task_list[i/o] Active array of task structures.
 *  @param task_amount[i/o] Pointer to the amount of non-empty tasks in the array. */
void Add_Task (task *task_list, int *task_amount) {
    int id = *task_amount;

    if (id >= TASK_AMOUNT_MAX) {
        printf("The maximum amount of tasks has been added.\n");
        return;
    }
    
    Set_Task_Name(task_list[id].name);
    Set_Task_Power(&task_list[id].power);
    Set_Task_Duration(&task_list[id].duration);
    Set_Task_Status(&task_list[id].is_passive);
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
 *  @param name[i] String that contains the name of the task. */
void Set_Task_Name (char *name) {
    printf("Task name (max %d): ", TASK_NAME_MAX - 1);
    fgets(name, TASK_NAME_ALLOC, stdin);
    strtok(name, "\n");
    name[TASK_NAME_MAX - 1] = '\0'; 
}

/** Changes the power of a task.
 *  @param power[i] Power usage of the task (in Watts). */
void Set_Task_Power (double *power) {
    char temp_string[TASK_NAME_MAX];
    printf("Power usage (watts): ");
    fgets(temp_string, TASK_NAME_MAX, stdin);
    sscanf(temp_string, " %lf", power);
    *power /= W_PER_KW;
}

/** Changes the duration of a task.
 *  @param duration[i] Duration of the task (in hours). */
void Set_Task_Duration (double *duration) {
    char temp_string[TASK_NAME_MAX];
    printf("Task duration (min): ");
    fgets(temp_string, TASK_NAME_MAX, stdin);
    sscanf(temp_string, " %lf", duration);
    *duration /= MIN_PER_HOUR;
}

/** Changes the status of a task.
 *  @param is_passive[i] Boolean that represents whether the task is passive or not (true = 1, false = 0). */
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
 *  @param days[o] Array of booleans that decide whether a task is done on a certain weekday (true = 1, false = 0). */
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
 *  @param task_list[i] Active array of task structures.
 *  @param task_amount[i] Pointer to the amount of non-empty task structures in the array.
 *  @param id[i] The id/index of the task to be removed. */
void Remove_Task (task *task_list, int *task_amount, int id) {
    char task_name[TASK_NAME_ALLOC];
    id--;

    if (id < 0 || id >= *task_amount) {
        printf("The id was not recognized.\n");
        return;
    }
    strcpy(task_name, task_list[id].name);

    strcpy(task_list[id].name, EMPTY_TASK_NAME);
    task_list[id].power = 0.0;
    task_list[id].duration = 0.0;
    task_list[id].kWh = 0.0;
    task_list[id].is_passive = 0;
    task_list[id].start_hr = 0;
    task_list[id].end_hr = 0;
    task_list[id].is_assigned = 0;
    task_list[id].total_days_yr = 0;

    *task_amount -= 1;
    Sort_Task_List(task_list);
    printf("Task: %s was successfully removed.\n", task_name);
}

/** Sort the input task array.
 *  The compare function Compare_Tasks() prioritizes non-empty tasks with the highest kWh usage.
 *  @param task_list[i] Active array of task structures. */
void Sort_Task_List (task *task_list) {
    qsort(task_list, TASK_AMOUNT_MAX, sizeof(task), Compare_Tasks);
}

/** Compare function for qsort for sorting tasks.
 *  Non-empty tasks with the highest kWh usage are prioritized.
 *  @param ip1[i] The first task to be sorted.
 *  @param ip2[i] The second task to be sorted.
 *  @return 1 if the ip1 is empty or has a lower kWh than ip2, -1 if ip2 is empty or has a lower kWh than ip1, 0 if they are equal. */
 int Compare_Tasks (const void *ip1, const void *ip2) {
    const task *task1 = (task *) ip1,
               *task2 = (task *) ip2;
        
    if (strcmp(task1->name, EMPTY_TASK_NAME) == 0 && strcmp(task2->name, EMPTY_TASK_NAME) != 0)
        return 1;
    else if (strcmp(task1->name, EMPTY_TASK_NAME) != 0 && strcmp(task2->name, EMPTY_TASK_NAME) == 0)
        return -1;
    else if (task1->kWh < task2->kWh)
        return 1;
    else if (task1->kWh > task2->kWh)
        return -1;
    else
        return 0;
}

/** Allows the user to edit a selected task in the task array. 
 * @param task_list[i/o] Active array of task structures. 
 * @param task_amount[i] Amount of non-empty tasks in the task array.
 * @param task_id[i] Selected task structure to be edited */
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
    printf("1: Set name\n"
            "2: Set power\n"
            "3: Set duration\n"
            "4: Set status\n"
            "5: Set days of occurrence\n"
            "0: Go back\n");
    Print_Line(0, "");
    
    do {
        printf("Choose a setting: ");
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
            Set_Task_Status(&task_list[id].is_passive);
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

/** Prints the suggested starting times and potential savings for all tasks.
 *  @param task_amount[i] Amount of non-empty task structures in the task array.
 *  @param task_list[i] Active array of task structures.
 *  @param time[i] Struct containing info on the current date/time. **/
void Print_Suggestions_Day (int task_amount, task task_list[TASK_AMOUNT_MAX], struct tm time) {
    int i;
    Print_Line(1, "Task Suggestions");
    printf("%-20s %-12s %-10s %-14s %-14s %-8s\n", "Task", "Status", "Hours", "Min price", "Max price", "Savings");
    for (i = 0; i < task_amount; i++) {
        if (!task_list[i].days[time.tm_wday] || !task_list[i].min_price)
            continue;
        printf("%-20s %3c %11.2d-%-2.2d %11.2f %14.2f %14.1f%%\n",
            task_list[i].name,
            task_list[i].is_passive ? 'P' : 'A',
            task_list[i].start_hr,
            task_list[i].end_hr,
            task_list[i].min_price,
            task_list[i].max_price,
            Fixed_Percent(task_list[i].max_price, task_list[i].min_price));
    }
    Print_Line(0, "");
}

/** Prints the suggested starting times and potential savings for all tasks.
 *  @param task_amount[i] Amount of non-empty task structures in the task array.
 *  @param task_list[i] Active array of task structures. */
void Print_Suggestions_Year (int task_amount, task task_list[TASK_AMOUNT_MAX]) {
    int i;
    Print_Line(1, "Potential yearly savings");
    printf("%-20s %-9s %12s %12s %13s\n", "Task", "Days/year", "Min price", "Max price", "Savings");
    for (i = 0; i < task_amount; i++) {
        printf("%-29s %6d %13.2f %12.2f %13.1f%%\n",
            task_list[i].name,
            task_list[i].total_days_yr,
            task_list[i].min_price,
            task_list[i].max_price,
            Fixed_Percent(task_list[i].max_price, task_list[i].min_price));
    }
    Print_Line(0, "");
}

/** Calculates the difference between two numbers in fixed percent.
 *  @param min[i] Lowest input.
 *  @param max[i] Highest input.
 *  @return Returns the difference between min and max in fixed percent. */
double Fixed_Percent(double max, double min) {
    return (min != 0) ? (-((min / max) - 1) * 100.0) : 0;
}

/* Testing functions below */
void Test_KW(void) {
    task actual1,
         actual2,
         actual3;
    double expected1 = 195.9,
           expected2 = 5.031,
           expected3 = 9315;

    actual1.power = 3.2650;
    actual1.duration = 60;

    actual2.power = 0.3354;
    actual2.duration = 15;

    actual3.power = 69;
    actual3.duration = 135;

    assert(expected1 == Calculate_kWh(actual1));
    assert(expected2 == Calculate_kWh(actual2));
    assert(expected3 == Calculate_kWh(actual3));
}