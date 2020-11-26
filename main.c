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
        task_id = 0;

    user.bool_ignore_hours = 1;

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

        /* Prices */
        else if (!strcmp(cmd_input, LIST_PRICES))
            List_Prices(prices);
        else if (!strcmp(cmd_input, LIST_PRICES_SORTED))
            List_Prices(prices_sorted);

        /* Tasks */
        else if (!strcmp(cmd_input, LIST_TASKS))
            Print_Task_List(task_list, task_amount);
        else if (!strcmp(cmd_input, SUGGEST))
            Suggest_Tasks(user, task_list, task_amount, prices_sorted, prices);
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

    /* Attempts to load price file. Terminates if there is none */
    file_status = Calculate_Prices(prices, 0);
    if (file_status == -1) {
        printf("Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Calculate_Prices(prices_sorted, 1);

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

/* Calculates an efficient starting time for each task */
void Suggest_Tasks(User user, task *task_list, int task_amount, double prices_sorted[][2], double prices[][2]) {
    double task_price = 0.0,
           task_price_max = 0.0;
    int i = 0, j = 0,
        task_id = 0,
        skip_hr = 0,
        start_hr = 0,
        end_hr = 0,
        prev_task_start_hr = 0,
        cheapest_available_hr = 0;

    Print_Line(1, "Suggest tasks");
    printf("%-29s %s %17s %17s\n", "Name", "Status", "Hours", "Savings");
    
    /* Handle all passive tasks first - set to start at cheapest availabel hr */
    for (i = 0; i < HOURS_PER_DAY; i++) {
        if (user.bool_ignore_hours || user.available_hours[(int)prices_sorted[i][1]]) {
            cheapest_available_hr = (int)prices_sorted[i][1];
            break;
        }
    }
    
    start_hr = cheapest_available_hr;
    end_hr = cheapest_available_hr + task_list[task_id].duration_hrs;

    while (task_id < (task_amount - 1)) {
        while (!task_list[task_id].bool_is_passive) {
            task_id++;
            continue;
        }
        
        task_price = 0.0;
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

    /* Go through the hours in the sorted price array from cheapest to most expensive h */
    for (i = 0; i < HOURS_PER_DAY; i++) {
        /* If the previous task is passive, another task can also be assigned to that hour */
        if (task_id == task_amount)
            break;

        while (task_list[task_id].bool_is_passive) {
            if (task_id == task_amount)
                break;
            task_id++;
        }

        /* Reset variables for each hour */
        skip_hr = 0;
        task_price = 0.0;
        start_hr = (int)prices_sorted[i][1];
        end_hr = start_hr + (int)task_list[task_id].duration_hrs;
        if (end_hr > HOURS_PER_DAY)
            end_hr -= HOURS_PER_DAY;
        
        /* Check availability for all hours between the start and end hours */
        if (!user.bool_ignore_hours) {
            while (j != end_hr) {
                if (j == HOURS_PER_DAY)
                    j = 0;
                    
                if (!user.available_hours[j])
                    skip_hr = 1;
                j++;
            }
        }

        for (j = start_hr; j < end_hr; j++) {
            if (j == prev_task_start_hr) {
                skip_hr = 1;
                break;
            }
        }
        
        /* If all hours between the start and end hours are available, assign task and calculate potential savings */
        if (!skip_hr) {
            task_price = Calculate_Task_Price(i, task_list[task_id], prices_sorted, prices);
            task_price_max = Calculate_Peak_Price(prices_sorted, prices, task_list[task_id]);

            printf("%-29s %3s %14s %2.2d-%2.2d %16.2f%% \n",
                   task_list[task_id].name,
                   task_list[task_id].bool_is_passive ? "P" : "A",
                   "",
                   start_hr,
                   end_hr,
                   Calculate_Savings_Percent(task_price_max, task_price));
            task_id++;
            prev_task_start_hr = start_hr;
        }
    }
    Print_Line(0, "");
}

/* Calculates the task price for the given input task and hour */
double Calculate_Task_Price(int hour, task task, double prices_sorted[][2], double prices[][2]) {
    int j;
    double price = 0.0,
           task_price = 0.0;

    for (j = 0; j < task.duration_hrs; j++) {
        if ((int)prices_sorted[hour][1] + j >= 23)
            hour = 0 - j;

        price = prices[(int)prices_sorted[hour][1] + j][0];
        if (task.duration_hrs > j && task.duration_hrs < (j + 1))
            task_price += task.kW * (task.duration_hrs - j) * price;
        else
            task_price += task.kW * price;
    }
    return task_price;
}

/* Returns the difference between the found price and max price in percent */
double Calculate_Savings_Percent(double task_price_max, double task_price) {
    return ((task_price_max / task_price) - 1) * 100.0;
}

/* Assumes that the hour 17 is the start of the highest peak, and calculates the price from there */
double Calculate_Peak_Price(double prices_sorted[][2], double prices[][2], task task) {
    int j = 0, i, hour = 17;
    double result = 0.0, price = 0.0;

    for (i = hour; j < task.duration_hrs; i++) {
        if (i > 23)
            i = 0;

        price = prices[i][0];

        if (task.duration_hrs > j && task.duration_hrs < (j + 1))
            result += task.kW * (task.duration_hrs - j) * price;
        else
            result += task.kW * price;
        j++;
    }
    return result;
}