#include "tasks.h"
#include "help.h"

/** Print a list of all non-empty tasks in the task array.
 *  @param task_list[i/o] Active array of task structures.
 *  @param task_amount[i] Amount of non-empty task structures in the array. 
 *  @param current_day[i] The day the program is currently loading data from */
void Print_Task_List (task *task_list, int task_amount, int current_day) {
    int i;
    char type[3] = {'A', 'P', 'T'};

    if (task_amount == 0) {
        printf("There are currently no tasks. Enter 'task add' to begin adding some.\n"); 
        return;
    }

    Sort_Task_List(task_list);

    Print_Line(1, "Task list");
    printf("%7s%6s%23s%14s%15s%9s%8s\n", "ID", "Name", "Power", "Duration", "Energy usage", "Type  ", "Today?");
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {            
            printf("Task %2d: %-20s %4.1f kW %8.1f hrs %10.3f kWh %6c %6c\n", 
                    (i + 1),
                    task_list[i].name, 
                    task_list[i].power,
                    task_list[i].duration, 
                    task_list[i].kWh,
                    type[task_list[i].type],
                    task_list[i].days[current_day] ? 'Y' : 'N');
        }
    }
    Print_Line(0, "");
    printf("Hint: Use command 'suggest' to receive suggestions on when to do your tasks today!\n");
}

/** Load tasks from the config file.
 *  @param task_list[i/o] Active array of task structures.
 *  @param task_amount[i/o] Amount of non-empty task structures in the array. 
 *  @param file_location[i] Location of the file where tasks are stored locally. 
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
               &task_list[*task_amount].type,
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

/** Save all tasks from the task array to the config file.
 *  @param task_list[i] Active array of task structures.
 *  @param task_amount[i]  Amount of non-empty tasks in the array.
 *  @param file_location[i] Location of the file where tasks are stored locally.
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
        fprintf(fp, "name: %s, power: %f, duration: %f, type: %d, days: %d %d %d %d %d %d %d\n",
                task_list[i].name, task_list[i].power, task_list[i].duration, task_list[i].type,
                task_list[i].days[0], task_list[i].days[1], task_list[i].days[2], 
                task_list[i].days[3], task_list[i].days[4], task_list[i].days[5], 
                task_list[i].days[6]);
    }

    fclose(fp);
    return 1;
}

/** Calculate the kWh usage of a given task.
 *  @param task[i] A non-empty task structure from the task array. 
 *  @return The kWh usage of the task as a double. */
double Calculate_kWh (task task) {
    return task.power * task.duration;
}

/** Reset all variables in the given task structure.
 *  @param p_task[o] Pointer to a task structure to be reset. */
void Reset_Task (task *p_task) {
    strcpy(p_task->name, EMPTY_TASK_NAME);
    p_task->type = active;
    p_task->power = 0.0;
    p_task->duration = 0.0; 
    p_task->kWh = 0.0;
    p_task->start_hr = 0;
    p_task->end_hr = 0;
    p_task->max_value = 0;
    p_task->min_value = 0;
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
    Set_Task_Status(&task_list[id].type);
    Set_Task_Days(task_list[id].days);
    task_list[id].kWh = Calculate_kWh(task_list[id]);

    *task_amount += 1;
    printf("Task: %s was successfully added.\n", task_list[id].name);
    Sort_Task_List(task_list);
}

/** Sets or changes the name of a task.
 *  @param name[o] String that contains the name of the task. */
void Set_Task_Name (char *name) {
    do {
        printf("Task name (max %d): ", TASK_NAME_MAX - 1);
        fgets(name, TASK_NAME_ALLOC, stdin);
        strtok(name, "\n");  
        name[TASK_NAME_MAX - 1] = '\0';
    } while (!strcmp(name, EMPTY_TASK_NAME));
}

/** Sets or changes the power of a task.
 *  @param power[o] Power usage of the task (in Watts). */
void Set_Task_Power (double *power) {
    printf("Power usage (watts): ");
    *power = Get_Double_Input (TASK_POWER_MAX);
    *power /= W_PER_KW;
}

/** Sets or changes the duration of a task.
 *  @param duration[o] Duration of the task (in hours). */
void Set_Task_Duration (double *duration) {
    printf("Task duration (minutes): ");
    *duration = Get_Double_Input (TASK_DURATION_MAX);
    *duration /= MIN_PER_HOUR;
}

/** Sets or changes the status of a task.
 *  @param type[o] enum value that represents task type (0 = active, 1 = Passive, 2 = Timed). */
void Set_Task_Status (int *type) {
    printf("Choose the task type (1 = Active, 2 = Passive, 3 = Timed): ");
    *type = (int)Get_Double_Input(3) - 1;
}

/** Sets or changes the weekdays that a task is done.
 *  @param days[o] Array of booleans that decide whether a task is done on a certain weekday (true = 1, false = 0). */
void Set_Task_Days (int days[7]) {
    char *weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    int i = 0;

    printf("Do task on day? (y/n)\n");
    for (i = 0; i < 7; i++) {
        printf("%-12s: ", weekdays[i]);
        days[i] = Get_Bool_Input();
    }
}

/** Remove a task from the task array.
 *  The selected task is effectively reset, since it cannot actually be removed from a static array in C.
 *  @param task_list[i] Active array of task structures.
 *  @param task_amount[i] Pointer to the amount of non-empty task structures in the array.
 *  @param id[i] The id/index of the task to be removed. 
 *  @return right away if the ID input does not refer to a task. */
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
    task_list[id].type = active;
    task_list[id].start_hr = 0;
    task_list[id].end_hr = 0;
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
            "6: More info\n"
            "0: Go back\n");
    Print_Line(0, "");
    
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
        task_list[id].kWh = Calculate_kWh(task_list[id]);
        break;
    case 3:
        Set_Task_Duration(&task_list[id].duration);
        task_list[id].kWh = Calculate_kWh(task_list[id]);
        break;
    case 4:
        Set_Task_Status(&task_list[id].type);
        break;
    case 5:
        Set_Task_Days(task_list[id].days);
        break;
    case 6:
        Help_Tasks_Extended();
        break;
    default:
        printf("The command was not recognized.\n"); 
        break;
    }
}

/** Prints the suggested starting times and potential savings for all tasks.
 *  @param task_amount[i] Amount of non-empty task structures in the task array.
 *  @param task_list[i] Active array of task structures.
 *  @param current_day[i] The day the program is currently loading data from 
 *  @param use_emissions[i] Bool informing the function wether it's printing CO2 emissions or prices **/
void Print_Suggestions_Day (int task_amount, task task_list[TASK_AMOUNT_MAX], int current_day, int use_emissions) {
    int i;
    char type[3] = {'A', 'P', 'T'};

    Print_Line(1, "Task Suggestions");
    printf("%-20s %-12s %-10s %-14s %-14s %-8s\n", "Task", "Type  ", "Hours", "Min value", "Max value", "Savings");
    for (i = 0; i < task_amount; i++) {
        if (!task_list[i].days[current_day] || !task_list[i].min_value)
            continue;

        printf("%-20s %3c %11.2d-%-2.2d %14.2f %s %10.2f %s %7.1f%%\n",
            task_list[i].name,
            type[task_list[i].type],
            task_list[i].start_hr,
            task_list[i].end_hr,
            task_list[i].min_value,
            use_emissions ? "g  " : "DKK",
            task_list[i].max_value,
            use_emissions ? "g  " : "DKK",
            Fixed_Percent(task_list[i].max_value, task_list[i].min_value));
    }
    Print_Line(0, "");
}

/** Prints the potential savings across a year for all tasks.
 *  @param task_amount[i] Amount of non-empty task structures in the task array.
 *  @param task_list[i] Active array of task structures. 
 *  @param use_emissions[i] Bool informing the function wether it's printing CO2 emissions or prices */
void Print_Suggestions_Year (int task_amount, task task_list[TASK_AMOUNT_MAX], int use_emissions) {
    int i;
    double min_value, max_value;
    
    Print_Line(1, "Potential yearly savings");
    printf("%-20s %-9s %17s %18s %14s\n", "Task", "Days/year", "Min value", "Max value", "Savings");
    for (i = 0; i < task_amount; i++) {
        min_value = task_list[i].min_value;
        max_value = task_list[i].max_value;

        if (use_emissions) {
            min_value /= G_PER_KG;
            max_value /= G_PER_KG;
        }

        printf("%-20s %6d %20.2f %s %14.2f %s %9.1f%%\n",
            task_list[i].name,
            task_list[i].total_days_yr,
            min_value,
            use_emissions ? "kg " : "DKK",
            max_value,
            use_emissions ? "kg " : "DKK",
            Fixed_Percent(task_list[i].max_value, task_list[i].min_value));
    }
    Print_Line(0, "");
}

/** Calculates the difference between two numbers in fixed percent.
 *  @param max[i] Highest input.
 *  @param min[i] Lowest input.
 *  @return Returns the difference between max and min in fixed percent. */
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

/* Test the Fixed_Percent function */
void Test_Fixed_Percent(void) {
    double   min[5] = {11, 55, 33},
             max[5] = {65, 120, 85};
    int expected[5] = {83, 54, 61};

    assert(expected[0] == (int)Fixed_Percent(max[0], min[0]));
    assert(expected[1] == (int)Fixed_Percent(max[1], min[1]));
    assert(expected[2] == (int)Fixed_Percent(max[2], min[2]));
}
