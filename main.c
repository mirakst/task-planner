/* 
 * Navne: Admir Bevab, Daniel Berwald, Anders Mariegaard, Nikolaj Dam, Mikkel Stidsen, Torbjørn Helstad, Kasper Henningsen.
 * Mails: SW1A405a@student.aau.dk
 * Gruppe: A405a
 * Studieretning: Software 
 */

#include "main.h"

int main(void) {
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    User user;
    char cmd_input[INPUT_MAX];
    double prices[HOURS_PER_DAY][2],
        prices_sorted[HOURS_PER_DAY][2];
    task task_list[TASK_AMOUNT_MAX];
    int task_amount = 0,
        task_id = 0,
        i,
        day;

    Initialize(prices, prices_sorted, &user, task_list, time, &task_amount);

    do {
        Get_User_Input(cmd_input);

        if (!strcmp(cmd_input, EXIT))
            break;

        /* Help commands */
        if (!strcmp(cmd_input, HELP))
            Help_All();
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

        /*else if (!strcmp(cmd_input, LIST_SETTINGS))
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
            Set_Ignore_Hours(&user);*/
       
        /* Prices */
        else if (!strcmp(cmd_input, LIST_PRICES))
            List_Prices(prices);
        else if (!strcmp(cmd_input, LIST_PRICES_SORTED))
            List_Prices(prices_sorted);
        else if (!strcmp(cmd_input, CHANGE_DAY))
            Change_Day(prices);

        /* Tasks */
        else if (!strcmp(cmd_input, LIST_TASKS))
            Print_Task_List(task_list, task_amount, time, 0);
        else if (!strcmp(cmd_input, LIST_ALL_TASKS))
            Print_Task_List(task_list, task_amount, time, 1);
        else if (!strcmp(cmd_input, ADD_TASK))
            Add_Task(task_list, &task_amount, time);
        else if (strstr(cmd_input, REMOVE_TASK)) {
            if (sscanf(cmd_input, " %*s %*s %d", &task_id) == 1)
                Remove_Task(task_list, &task_amount, task_id);
            else
                printf("Task ID not specified.\n");
        }
        else if (strstr(cmd_input, EDIT_TASK)) {
            if (sscanf(cmd_input, " %*s %*s %d", &task_id) == 1)
                Edit_Task(task_list, task_amount, task_id);
            else
                printf("Task ID not specified.\n");
        }
        else if (!strcmp(cmd_input, SUGGEST)) {
            for (i = 0; i < task_amount; i++) {
                task_list[i].max_price = 0;
                task_list[i].min_price = 0;
                Find_Lowest_Price (user, &task_list[i], task_list, task_amount, prices, 0);  
            }
                
            Print_Suggestions (task_amount, task_list, 0, time);
        }
        else if (!strcmp(cmd_input, SUGGEST_YEAR)) {
            for (i = 0; i < task_amount; i++) {
                task_list[i].max_price = 0;
                task_list[i].min_price = 0;
            }

            for (day = 1; day <= DAYS_PER_YEAR; day++) {
                Calculate_Prices (prices, 0, day);
                for (i = 0; i < task_amount; i++) {
                    if (!task_list[i].days[Day_To_Weekday(day)])
                        continue;
                    Find_Lowest_Price (user, &task_list[i], task_list, task_amount, prices, 1);
                }
            }
            Print_Suggestions (task_amount, task_list, 1, time);
            Calculate_Prices (prices, 0, time.tm_yday);
        }
        else
            printf("Command not recognized. Type 'help' for the list of commands.\n");

    } while (strcmp(cmd_input, EXIT) != 0);

    Save(user, task_list, task_amount);

    return EXIT_SUCCESS;
}

/** Gets user input and convert to lower case letters.
 *  @param[i/o] input String with user input. */
void Get_User_Input (char *input) {
        printf("\nPlease enter a command: ");
        fgets(input, INPUT_MAX, stdin);
        String_To_Lower(input);
}

/** Performs all necessary initialization and loading of structs and variables.
 *  @param[i/o] p Unsorted array of prices.
 *  @param[i/o] p_sorted Sorted array of prices.
 *  @param[i/o] user Pointer to the user structure with all user details.
 *  @param[i] time Structure containing a calendar date and time broken down into its components.
 *  @param[i/o] task_list Active array of tasks. 
 *  @param[i/o] task_amount Amount of non-empty tasks in the task array. */
void Initialize(double p[][2], double p_sorted[][2], User *user, task task_list[TASK_AMOUNT_MAX], struct tm time, int *task_amount) {
    int file_status = 0,
        i,
        today = time.tm_yday;
    char *weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    /* Attempts to load price file. Terminates if there is none */
    file_status = Calculate_Prices(p, 0, today);
    if (file_status == -1) {
        printf("Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Calculate_Prices(p_sorted, 1, today);

    /* FARLIG GLOBAL VARIABEL */
    current_weekday = time.tm_wday;

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

    /* Resets all tasks in the task array */
    for (i = 0; i < TASK_AMOUNT_MAX; i++)
        Reset_Task(&task_list[i]);
    
    /* Attempts to load tasks from tasklist file */
    file_status = Load_Tasks(task_list, task_amount, FILE_TASKLIST);
    if (file_status == 1)
        printf("Loaded %d tasks successfully.\n", *task_amount);
    else if (file_status == -1)
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);
    
    if (user->bool_name)
        printf("Hello %s\n", user->user_name);
    printf("Today is %s, %.2d-%.2d-%.2d\n", weekday[time.tm_wday], time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);
}

/** Saves the user details and task list.
 *  @param[i] user User structure with all user details.
 *  @param[i] task_list Active array of task structures.
 *  @param[i] task_amount Amount of non-empty task structures in the task array. */
void Save(User user, task task_list[TASK_AMOUNT_MAX], int task_amount) {
    if (Save_User_Details(user) != 1) {
        printf("Failed to save user settings.\n");
        return;
    }
    if (Save_Tasks(task_list, task_amount, FILE_TASKLIST) == -1) {
        printf("Failed to save tasks.\n");
        return;
    }
    printf("Saved tasks and settings successfully.\n");
}

/** Finds the starting time with the lowest average price over the task duration.
 *  @param[i] User user structure with all user details.
 *  @param[i/o] p_task Pointer to a task structure.
 *  @param[i] task_list Array of task structures.
 *  @param[i] task_amount Amount of task structures in the task array.
 *  @param[i] p Unsorted array of prices.
 *  @param[i] do_year Boolean to determine whether the yearly or daily savings should be calculated (true = 1, false = 0). */
void Find_Lowest_Price (User user, task *p_task, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], int do_year) {
    int i, j, k, q, end_hr = 0, avg_hr_start = 0, avg_hr_end = 0, skip_hr = 0;
    double price = 0.0, avg = 0.0, avg_min = 100, avg_max = 0, price_min = 0, price_max = 0;
    int duration = (int) ceil(p_task->duration);
    
    p_task->is_assigned = 0;

    for (i = 0; i < HOURS_PER_DAY; i++) {
        end_hr = Wrap_Hour(i + duration);
        skip_hr = 0;

        if (!user.bool_ignore_hours)
            if (!user.available_hours[i])
                continue;

        if (!p_task->bool_is_passive) {
            for (j = i; j != end_hr; j++) {
                j = Wrap_Hour(j);
                
                if (j == end_hr)
                    break;

                if (!user.available_hours[j]) {
                    skip_hr = 1;
                    price = 0.0;
                    break;
                }

                if (!do_year) {
                    for (k = 0; k < task_amount; k++) {
                        if (!strcmp(task_list[k].name, p_task->name) || !task_list[k].is_assigned || task_list[k].bool_is_passive)
                            continue;

                        else {
                            for (q = task_list[k].start_hr; q != task_list[k].end_hr; q++) {
                                q = Wrap_Hour(q);
                                if (q == i) {
                                    skip_hr = 1;
                                    price = 0.0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if(!skip_hr) {
            for (j = 0; j < duration; j++) {
                if (p_task->duration > j && p_task->duration < (j + 1)) 
                    price += p_task->power * (p_task->duration - j) * p[Wrap_Hour(i + j)][0];
                else
                    price += p_task->power * p[Wrap_Hour(i + j)][0];
            }
            
            /*printf("Price for index p[%d]= %.3f\n", Wrap_Hour(i + j), p[Wrap_Hour(i + j)][0] * p_task->kW);*/
            avg = price / duration;
            
            if (avg > avg_max) {
                avg_max = avg;
                price_max = price;
            }
            else if (avg <= avg_min) {
                avg_min = avg;
                price_min = price;
                avg_hr_start = i;
                avg_hr_end = Wrap_Hour(avg_hr_start + j);
            }
        }
        price = 0;
    }
    
    if (price_min == 0 && !do_year) {
        printf("Could not find a suitable starting time for %s.\n", p_task->name);
        return;
    }

    p_task->is_assigned = 1;
    /* Assign the lowest avg price values to the input task */
    if (do_year) {
        p_task->min_price += price_min;
        p_task->max_price += price_max;
    }
    else {
        p_task->start_hr = avg_hr_start;
        p_task->end_hr = avg_hr_end;
        p_task->min_price = price_min;
        p_task->max_price = price_max;
    }

}

/** Wraps the input hour if it surpasses the amount of hours in a day.
 *  @param[i] hour Amount of hours to be wrapped.
 *  @return Returns the input hour in the range 0-23. */
int Wrap_Hour (int hour) {
    return (hour >= HOURS_PER_DAY) ? (hour - HOURS_PER_DAY) : hour;
}

/** Converts the input day to its weekday (2019).
 *  @param[i] day Day to be converted.
 *  @return Returns the input day as a weekday (0-6). */
int Day_To_Weekday (int day) {
    return (FIRST_WEEKDAY_OF_YEAR + (day % DAYS_PER_WEEK)) % DAYS_PER_WEEK;
}
  
/** Prints the suggested starting times and potential savings for all tasks.
 *  @param[i] task_amount Amount of non-empty task structures in the task array.
 *  @param[i] task_list Active array of task structures.
 *  @param[i] do_year Boolean to determine whether the yearly or daily savings should be printed (true = 1, false = 0). **/
void Print_Suggestions (int task_amount, task task_list[TASK_AMOUNT_MAX], int do_year, struct tm time) {
    int i;
    
    Print_Line(0, "");
    if (do_year) {
        printf("If you were to follow recommendations for a full year, this is the expected outcome.\n");
        printf("%-20s %10s %12s %12s %13s\n", "Task", "", "Min price", "Max price", "Savings");
        for (i = 0; i < task_amount; i++) {
            printf("%-29s %13.2f %12.2f %13.1f%%\n",
                task_list[i].name,
                task_list[i].min_price,
                task_list[i].max_price,
                Fixed_Percent(task_list[i].max_price, task_list[i].min_price));
        }
    }
    else {
        printf("%-20s %8s %2s %12s %12s %13s\n", "Task", "Hours", "", "Min price", "Max price", "Savings");
        for (i = 0; i < task_amount; i++) {
            if (!task_list[i].days[time.tm_wday] || !task_list[i].min_price)
                continue;
            printf("%-20s %5.2d-%-2.2d %13.2f %12.2f %13.1f%%\n",
                task_list[i].name,
                task_list[i].start_hr,
                task_list[i].end_hr,
                task_list[i].min_price,
                task_list[i].max_price,
                Fixed_Percent(task_list[i].max_price, task_list[i].min_price));
        }
    }
    Print_Line(0, "");
}

/** Calculates the difference between two numbers in fixed percent.
 *  @param[i] min Lowest input.
 *  @param[i] max Highest input.
 *  @return Returns the difference between min and max in fixed percent. */
double Fixed_Percent(double max, double min) {
    return (min != 0) ? (((max / min) - 1) * 100.0) : 0;
}

void Test_All(void){
    Test_Tasks();
}