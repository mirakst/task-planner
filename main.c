/* 
 * Navne: Admir Bevab, Daniel Berwald, Anders Mariegaard, Nikolaj Dam, Mikkel Stridsen, Torbjørn Helstad, Kasper Henningsen.
 * Mails: sw1a405a@student.aau.dk
 * Gruppe: A405a
 * Studieretning: Software 
 */

#include "main.h"

/* Main program function */
int main(void) {
    User user;
    char cmd_input[INPUT_MAX];
    double prices[HOURS_PER_DAY][2],
        prices_sorted[HOURS_PER_DAY][2];
    task task_list[TASK_AMOUNT_MAX];
    int task_amount = 0,
        task_id = 0,
        i;

    Initialize(prices, prices_sorted, &user, task_list, &task_amount);

    do {
        /* Get user input and convert to lower case letters */
        printf("\nPlease enter a command: ");
        fgets(cmd_input, INPUT_MAX, stdin);
        String_To_Lower(cmd_input);

        if (!strcmp(cmd_input, EXIT))
            break;

        /* Help commands */
        if (!strcmp(cmd_input, HELP)) {
            printf("Printing all available commands.\n");
            Help_All();
        }
        else if (!strcmp(cmd_input, HELP_PRICES))
            Help_Prices(1);
        else if (!strcmp(cmd_input, HELP_TASKS))
            Help_Tasks(1);
        else if (!strcmp(cmd_input, HELP_SETTINGS))
            Help_User_Details(1);

        /* User settings */
        else if (!strcmp(cmd_input, SETTINGS))
            Acclimation_Index(&user);
        else if (!strcmp(cmd_input, SAVE_USER))
            Save(user, task_list, task_amount);

        /*
        else if (!strcmp(cmd_input, LIST_SETTINGS))
            Print_Settings(user);
        else if (!strcmp(cmd_input, SET_NAME))
            Get_Name(&user);
        else if (!strcmp(cmd_input, SET_HOURS))
            Set_Available_Hours(&user);
        else if (!strcmp(cmd_input, RESET_HOURS))
            Reset_Available_Hours(user.available_hours);
        else if (!strcmp(cmd_input, SAVE_USER))
            Save(user, task_list, task_amount);
        else if (!strcmp(cmd_input, RECOMMEND))
            Set_Ignore_Hours(&user);
        */
       
        /* Prices */
        else if (!strcmp(cmd_input, LIST_PRICES))
            List_Prices(prices);
        else if (!strcmp(cmd_input, LIST_PRICES_SORTED))
            List_Prices(prices_sorted);

        /* Tasks */
        else if (!strcmp(cmd_input, LIST_TASKS))
            Print_Task_List(task_list, task_amount);
        else if (!strcmp(cmd_input, SUGGEST)) {       /* HER SKER DER NOGET !!! */
            for (i = 0; i < task_amount; i++)
                Find_Lowest_Price (user, &task_list[i], prices);
                
            Print_Suggestions (task_amount, task_list);
        }
        else if (!strcmp(cmd_input, CHANGE_DAY)) {
            Change_Day(prices);
        }
        else if (!strcmp(cmd_input, ADD_TASK))
            Add_Task(task_list, &task_amount);
        else if (strstr(cmd_input, REMOVE_TASK)) {
            if (sscanf(cmd_input, " %*s %*s %d", &task_id) == 1)
                Remove_Task(task_list, &task_amount, task_id);
            else
                printf("Task ID not specified.\n");
        }
        else
            printf("Command not recognized. Type 'help' for the list of commands.\n");

    } while (!(!strcmp(cmd_input, EXIT)));

    Save(user, task_list, task_amount);

    return EXIT_SUCCESS;
}

/* Performs all initialization and loading of structs and variables */
void Initialize(double prices[][2], double prices_sorted[][2], User *user, task task_list[TASK_AMOUNT_MAX], int *task_amount) {
    int file_status = 0;
    int default_day = 1;

    /* Attempts to load price file. Terminates if there is none */
    file_status = Calculate_Prices(prices, 0, default_day);
    if (file_status == -1) {
        printf("Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Calculate_Prices(prices_sorted, 1, default_day);

    /* Attempt to load user details. Starts setup if there is no file */
    user->bool_hours = 0;
    user->bool_name = 0;

    file_status = Load_User_Details(user);
    if (file_status != 1) {
        First_Time_Setup(user);
        Save_User_Details(*user);
    }
    user->bool_hours = file_status;
    user->bool_name = file_status;

    /* Attempts to load tasks from tasklist file */
    Initialize_Tasks(task_list, task_amount);
    file_status = Load_Tasks(task_list, task_amount);
    if (file_status == 1)
        printf("Loaded %d tasks successfully.\n", *task_amount);
    else if (file_status == -1)
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);
    printf("Prices have been initialized from day: %d.\n", default_day);
}

/* Saves the user details and task list */
void Save(User user, task task_list[TASK_AMOUNT_MAX], int task_amount) {
    if (Save_User_Details(user) != 1) {
        printf("Failed to save user settings.\n");
        return;
    }
    if (Save_Tasks(task_list, task_amount) == -1) {
        printf("Failed to save tasks.\n");
        return;
    }
    printf("Saved tasks and settings successfully.\n");
}

/* FUCKING SUGGEST */
/* EVT. tilføj at aktive tasks ikke må overlappe */
void Find_Lowest_Price (User user, task *p_task, double p[][2]) {
    int i, j, end_hr = 0, avg_hr_start = 0, avg_hr_end = 0, skip_hr = 0;
    double price = 0.0, avg = 0.0, avg_min = 100.0, avg_max = 0.0;

    int duration = (int) ceil(p_task->duration);

    for (i = 0; i < HOURS_PER_DAY; i++) {
        end_hr = Wrap_Hour(i + duration);
        skip_hr = 0;

        if (!user.bool_ignore_hours)
            if (!user.available_hours[i])
                continue;

        if (!p_task->bool_is_passive) {
            for (j = i; j != end_hr; j++) {
                j = Wrap_Hour(j);
                if (!user.available_hours[j]) {
                    skip_hr = 1;
                    price = 0.0;
                    break;
                }
            }
        }
        
        if(!skip_hr) {
            for (j = 0; j < duration; j++)
                price += p[Wrap_Hour(i + j)][0];
            
            avg = price / duration;
            
            if (avg > avg_max)
                avg_max = avg;
            if (avg <= avg_min) {
                avg_min = avg;
                avg_hr_start = i;
                avg_hr_end = Wrap_Hour(avg_hr_start + j);
            }
        }
        price = 0;
    }
    
    /* Assign the lowest avg price values to the input task */
    p_task->start_hr = avg_hr_start;
    p_task->end_hr = avg_hr_end;
    p_task->min_price = avg_min;
    p_task->max_price = avg_max;
}

/* Returns the input hour in the range 0-23 */
int Wrap_Hour (int hour) {
    return (hour >= HOURS_PER_DAY) ? (hour - HOURS_PER_DAY) : hour;
}
  
/* Prints the suggested starting times and potential savings for all tasks */
void Print_Suggestions (int task_amount, task task_list[TASK_AMOUNT_MAX]) {
    int i;
    
    Print_Line(0, "");
    printf("%-20s %8s %2s %12s %12s %13s\n", "Task", "Hours", "", "Min price", "Max price", "Savings");
    for (i = 0; i < task_amount; i++) {
        printf("%-20s %5.2d-%-2.2d %13.3f %12.3f %13.1f%%\n",
            task_list[i].name,
            task_list[i].start_hr,
            task_list[i].end_hr,
            task_list[i].min_price,
            task_list[i].max_price,
            Fixed_Percent(task_list[i].max_price, task_list[i].min_price));
    }
    Print_Line(0, "");
}

double Fixed_Percent(double max, double min) {
    return (min != 0) ? (((max / min) - 1) * 100.0) : 0;
}








































































/* Don't DO IT */

















































/* TURN BACK */

























































/* GO BACK */








































































































/*
 Calculates an efficient starting time for each task
void Suggest_Tasks(User user, task *task_list, int task_amount, double prices_sorted[][2], double prices[][2]) {
    double task_price = 0.0,
           task_price_max = 0.0;
    int i = 0, j = 0, k = 0,
        task_id = 0,
        skip_hr = 0,
        start_hr = 0,
        end_hr = 0,
        prev_task_start_hr = 0,
        cheapest_available_hr = 0;

    Print_Line(1, "Suggest tasks");
    printf("%-29s %s %17s %17s\n", "Name", "Status", "Hours", "Savings");
    
    /Handle all passive tasks first - set to start at cheapest availabel hr 
    for (i = 0; i < HOURS_PER_DAY; i++) {
        if (user.bool_ignore_hours || user.available_hours[(int)prices_sorted[i][1]]) {
            cheapest_available_hr = (int)prices_sorted[i][1];
            break;
        }
    }
    
    start_hr = cheapest_available_hr;
    end_hr = start_hr + (int) task_list[task_id].duration;

    while (task_id < task_amount) {
        while (!task_list[task_id].bool_is_passive) {
            task_id++;
            continue;
        }
        if (task_id >= task_amount)
            break;
        
        task_price = Calculate_Task_Price(cheapest_available_hr, task_list[task_id], prices_sorted, prices);
        task_price_max = Calculate_Peak_Price(prices_sorted, prices, task_list[task_id]);
        printf("%-29s %3s %14s %2.2d-%2.2d %16.2f%% \n",
                task_list[task_id].name,
                task_list[task_id].bool_is_passive ? "P" : "A",
                "",
                start_hr,
                end_hr,
                Calculate_Savings_Percent(task_price_max, task_price));
        task_id++;
    }
    task_id = 0;

     Go through the hours in the sorted price array from cheapest to most expensive h
    for (i = 0; i < HOURS_PER_DAY; i++) {
         If the previous task is passive, another task can also be assigned to that hour 
            
        while (task_list[task_id].bool_is_passive)
            task_id++;
        
         Reset variables for each hour
        skip_hr = 0;
        task_price = 0.0;
        start_hr = (int)prices_sorted[i][1];
        end_hr = start_hr + (int)task_list[task_id].duration;
        if (end_hr > HOURS_PER_DAY)
            end_hr -= HOURS_PER_DAY;
        
         Check availability for all hours between the start and end hours 
        if (!user.bool_ignore_hours) {
            while (j != end_hr) {
                if (j == HOURS_PER_DAY)
                    j = 0;
                    
                if (!user.available_hours[j])
                    skip_hr = 1;
                j++;
            }
        }

         Prevent active tasks from starting at the same hour 
        for (j = 0; j < task_amount; j++) {
            if (task_list[task_id].is_assigned) {
                start_hr = task_list[task_id].start_hr;
                end_hr = task_list[task_id].end_hr;
                skip_hr = 0;
                break;
            }
            else if (!task_list[j].bool_is_passive && start_hr == task_list[j].start_hr) {
                skip_hr = 1;
                break;
            }
        }


         If all hours between the start and end hours are available, assign task and calculate potential savings
        if (!skip_hr && task_id < task_amount) {
            task_price = 0.0;
            task_price = Calculate_Task_Price(i, task_list[task_id], prices_sorted, prices);
            task_price_max = Calculate_Peak_Price(prices_sorted, prices, task_list[task_id]);

            task_list[task_id].start_hr = start_hr;
            task_list[task_id].end_hr = end_hr;
            task_list[task_id].is_assigned = 1;
            printf("%-29s %3s %14s %2.2d-%2.2d %16.2f%% \n",
                   task_list[task_id].name,
                   task_list[task_id].bool_is_passive ? "P" : "A",
                   "",
                   start_hr,
                   end_hr,
                   Calculate_Savings_Percent(task_price_max, task_price));
            task_id++;
        }
    }
    Print_Line(0, "");
}

 Calculates the task price for the given input task and hour 
double Calculate_Task_Price(int hour, task task, double prices_sorted[][2], double prices[][2]) {
    int j;
    double price = 0.0,
           task_price = 0.0;

    for (j = 0; j < task.duration; j++) {
        if ((int)prices_sorted[hour][1] + j >= 23)
            hour = 0 - j;

        price = task.kW * prices[(int)prices_sorted[hour][1] + j][0];
        if (task.duration > j && task.duration < (j + 1))
            task_price += task.kW * (task.duration - j) * price;
        else
            task_price += task.kW * price;
    }
    return task_price;
}

 Returns the difference between the found price and max price in percent 
double Calculate_Savings_Percent(double task_price_max, double task_price) {
    return ((task_price_max / task_price) - 1) * 100.0;
}

 Assumes that the hour 17 is the start of the highest peak, and calculates the price from there 
double Calculate_Peak_Price(double prices_sorted[][2], double prices[][2], task task) {
    int j = 0, i, hour = 17;
    double result = 0.0, price = 0.0;

    for (i = hour; j < task.duration; i++) {
        if (i > 23)
            i = 0;

        price = task.kW * prices[i][0];

        if (task.duration > j && task.duration < (j + 1))
            result += task.kW * (task.duration - j) * price;
        else
            result += task.kW * price;
        j++;
    }
    return result;
}*/
