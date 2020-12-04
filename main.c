/*
 * Navne: Admir Bevab, Daniel Berwald, Anders Mariegaard, Nikolaj Dam, Mikkel Stidsen, Torbjørn Helstad, Kasper Henningsen.
 * Mails: SW1A405a@student.aau.dk
 * Gruppe: A405a
 * Studieretning: Software 
 */
#include "main.h"

int main(void) {
    User user;
    char cmd_input[INPUT_MAX];
    double prices[HOURS_PER_DAY + 6][2],
           prices_sorted[HOURS_PER_DAY + 6][2],
           emissions[HOURS_PER_DAY + 6][2],
           emissions_sorted[HOURS_PER_DAY + 6][2];
    task task_list[TASK_AMOUNT_MAX];
    int task_amount = 0,
        task_id = 0,
        cmd,
        price_day;

    Test_All();
    Initialize(prices, prices_sorted, emissions, emissions_sorted, &user, task_list, &task_amount, &price_day);
    Help_All();

    do {
        cmd = Get_User_Input(cmd_input);

        switch (cmd) {
        case _exit: 
            printf("Exiting...\n"); break;
        case help:  
            Help_All(); break;
        case help_prices:
            Help_Prices(1); break;
        case help_tasks:
            Help_Tasks(1); break;
        case help_settings:
            Help_Settings(1); break;
        case settings:
            Print_Settings(&user); break;
        case save_user:
            Save(user, task_list, task_amount); break;
        case list_prices:
            List_kWh_Data(prices, 1); break;
        case list_prices_sorted:
            List_kWh_Data(prices_sorted, 1); break;
        case list_emissions:
            List_kWh_Data(emissions, 0); break;
        case list_emissions_sorted:
            List_kWh_Data(emissions_sorted, 0); break;
        case change_day:
            Change_Day(prices, emissions, &price_day); break;
        case list_tasks:
            Print_Task_List(task_list, task_amount, Day_To_Weekday(price_day)); break;
        case add_task:
            Add_Task(task_list, &task_amount); break;
            
        case remove_task:
            if (sscanf(cmd_input, " %*s %*s %d", &task_id) == 1)
                Remove_Task(task_list, &task_amount, task_id);
            else
                printf("Task ID not specified.\n");
            break;

        case edit_task:
            if (sscanf(cmd_input, " %*s %*s %d", &task_id) == 1)
                Edit_Task(task_list, task_amount, task_id);
            else
                printf("Task ID not specified.\n");
            break;

        case suggest:
            Suggest_Day (user, task_list, task_amount, prices, emissions, price_day); break;
        case suggest_year:
            Suggest_Year (user, task_list, task_amount, prices, emissions, price_day); break;
            
        default:
            printf("Command not recognized. Type 'help for the list of commands.\n"); break;
        }
    } while (cmd != _exit);

    Save(user, task_list, task_amount);

    return EXIT_SUCCESS;
}

/* p og e er uoverskueligt !!!!!!!! -anders (dumpap) */
/** Performs all necessary initialization and loading of structs and variables.
 *  @param p[i/o] Unsorted array of prices.
 *  @param p_sorted[i/o] Sorted array of prices.
 *  @param user[i/o] Pointer to the user structure with all user details.
 *  @param task_list[i/o]  Active array of tasks. 
 *  @param task_amount[i/o] Amount of non-empty tasks in the task array. */
void Initialize(double p[][2], double p_sorted[][2], double e[][2], double e_sorted[][2], User *user, task task_list[TASK_AMOUNT_MAX], int *task_amount, int *price_day) {
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    int file_status = 0,
        i;
    char *weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    *price_day = time.tm_yday;
    
    /* Attempts to load price and emission data. Terminates if there are none */
    file_status = Calculate_kWh_Data(p, e, 0, *price_day);
    if (!file_status) {
        printf("Could not load prices. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    Calculate_kWh_Data(p_sorted, e_sorted, 1, *price_day);

    /* Attempt to load user details. Starts setup if there is no file */
    user->has_set_hours = 0;
    user->has_name = 0;

    file_status = Load_User_Details(user);
    if (file_status != 1) {
        First_Time_Setup(user);
        Save_User_Details(*user);
    }
    user->has_set_hours = file_status;
    user->has_name = file_status;

    /* Resets all tasks in the task array */
    for (i = 0; i < TASK_AMOUNT_MAX; i++)
        Reset_Task(&task_list[i]);
    
    /* Attempts to load tasks from tasklist file */
    file_status = Load_Tasks(task_list, task_amount, FILE_TASKLIST);
    if (file_status == 1)
        printf("Loaded %d tasks successfully.\n", *task_amount);
    else if (file_status == -1)
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);
    
    if (user->has_name)
        printf("Hello %s\n", user->user_name);
    printf("Today is %s, %.2d-%.2d-%.2d\n", weekday[time.tm_wday], time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);
}

/** Saves the user details and task list.
 *  @param user[i] User structure with all user details.
 *  @param task_list[i] Active array of task structures.
 *  @param task_amount[i] Amount of non-empty task structures in the task array. */
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

/** Suggests a day for specific tasks
 *  @param user[i] User structure with all user details.
 *  @param task_list[][i] Array of task structures.
 *  @param task_amount[i] Amount of task structures in the task array.
 *  @param p[][][i] Unsorted array of prices.
 *  @param price_day[i] The day that prices are loaded from. Can be edited for debugging. */
void Suggest_Day (User user, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], double e[][2], int price_day) {
    int *assigned_hours = calloc(HOURS_PER_DAY, sizeof(int));
    int i;

    for (i = 0; i < task_amount; i++) {
        task_list[i].max_price = 0;
        task_list[i].min_price = 0;
        if (!task_list[i].days[Day_To_Weekday(price_day)]) {
            continue;
        }
        Find_Lowest_Price (user, &task_list[i], assigned_hours, p, 0);  
    }
    Print_Suggestions_Day (task_amount, task_list, Day_To_Weekday(price_day));
}

/** Calculates the savings if the plan is followed for a year
 *  @param user[i] User structure with all user details.
 *  @param task_list[][i] Array of task structures.
 *  @param task_amount[i] Amount of task structures in the task array.
 *  @param p[][][i] Unsorted array of prices.
 *  @param price_day[i] The day that prices are loaded from. Can be edited for debugging. */
void Suggest_Year (User user, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], double e[][2], int price_day) {
    int *assigned_hours = calloc(HOURS_PER_DAY, sizeof(int));
    int i, day;
    
    /* Reset the relevant task variables */
    for (i = 0; i < task_amount; i++) {
        task_list[i].max_price = 0;
        task_list[i].min_price = 0;
        task_list[i].total_days_yr = 0;
    }

    for (day = 1; day <= DAYS_PER_YEAR; day++) {
        Calculate_kWh_Data(p, e, 0, day);

        for (i = 0; i < task_amount; i++) {
            if (!task_list[i].days[Day_To_Weekday(day)])
                continue;
            Find_Lowest_Price (user, &task_list[i], assigned_hours, p, 1);
            task_list[i].total_days_yr++;
        }
    }
    Print_Suggestions_Year (task_amount, task_list);
    Calculate_kWh_Data (p, e, 0, price_day);
}

/** Finds the starting time with the lowest average price over the task duration.
 *  @param user[i] User structure with all user details.
 *  @param p_task[i/o] Pointer to a task structure.
 *  @param assigned_hours[][i] Sees if there is an active task on the hour.
 *  @param p[][][i] Unsorted array of prices.
 *  @param do_year[i] Boolean to determine whether the yearly or daily savings should be calculated (true = 1, false = 0). */
void Find_Lowest_Price (User user, task *p_task, int assigned_hours[HOURS_PER_DAY], double p[][2], int do_year) {
    int i, j,
        duration,
        end_hr = 0,
        best_hr_start = 0,
        best_hr_end = 0,
        should_skip_hr = 0;
    double cur_price = 0.0,
           price_avg = 0.0,
           price_avg_max = 0.0,
           price_avg_min = 100.0,
           price_min = 0.0, 
           price_max = 0.0;

    duration = (int) ceil(p_task->duration);

    for (i = 0; i < HOURS_PER_DAY; i++) {
        /* Calculate end hour for the task and reset variables for each hour */
        end_hr = Wrap_Hour(i + duration);
        should_skip_hr = 0;
        cur_price = 0.0;

        should_skip_hr = Should_Skip_Hour(user, p_task, assigned_hours, i, end_hr, do_year);

        /* If the user is available in this hour, calculate the avg price values and attempt to assign them */
        if(!should_skip_hr) {
            for (j = 0; j < duration; j++) {
                /* Handle 'overflowing' hours */
                /* Eks. duration = 3, p_task->duration = 2.5, så skal den sidste times pris ganges med den resterende længde af tasken (0.5 timer) */
                if (p_task->duration > j && p_task->duration < (j + 1)) 
                    cur_price += p_task->power * (p_task->duration - j) * p[i + j][0];
                else
                    cur_price += p_task->power * p[i + j][0];
            }
            
            price_avg = cur_price / duration;
            
            if (price_avg > price_avg_max) {
                price_avg_max = price_avg;
                price_max = cur_price;
            }
            else if (price_avg <= price_avg_min) {
                price_avg_min = price_avg;
                price_min = cur_price;
                best_hr_start = i;
                best_hr_end = Wrap_Hour(best_hr_start + j);
            }
        }
    }
    
    /* If price_min is unchanged, assume that no start hour was found */
    if (price_avg_min == 100 && !do_year) {
        printf("Could not find a suitable start hour for task %s\n", p_task->name);
        return;
    }
    Assign_Task(p_task, best_hr_start, best_hr_end, price_min, price_max, assigned_hours, do_year);
}

/** Checks whether a task can be assigned in the given start_hr and end_hr interval or not.
 *  @param user[i] User structure with all user details.
 *  @param p_task[i] Pointer to a task structure.
 *  @param assigned_hours[][i] Sees if there is an active task on the hour.
 *  @param start_hr[i] The start hour that is being checked for the given task.
 *  @param end_hr[i] The end hour that is being checked for the given task.
 *  @param do_year[i] Boolean to determine whether the yearly or daily savings should be calculated (true = 1, false = 0). */
int  Should_Skip_Hour (User user, task *p_task, int assigned_hours[HOURS_PER_DAY], int start_hr, int end_hr, int do_year) {
    int i;

    /* Skip to next start hour if the user is unavailable and does not want to use all hours */
    if (!user.is_available && !user.available_hours[start_hr])
            return 1;
    
    /* Prevent active tasks from ending outside the user's available hours */
    if (!p_task->is_passive) {
        for (i = start_hr; i != end_hr; i++) {
            i = Wrap_Hour(i);
            if (!user.available_hours[i] || (!do_year ? assigned_hours[i] : 0))
                return 1;
        }
    }
    return 0;
}

/** Assigns start/end hours and min/max prices to a given task.
 *  @param p_task[o] Pointer to a task structure.
 *  @param start_hr[i] The start hour that is being checked for the given task.
 *  @param end_hr[i] The end hour that is being checked for the given task.
 *  @param price_min[i] The lowest possible price for the task.
 *  @param price_max[i] The highest possible price for the task.
 *  @param assigned_hours[][o] Sees if there is an active task on the hour.
 *  @param do_year[i] Boolean to determine whether the yearly or daily savings should be calculated (true = 1, false = 0). */
void Assign_Task (task *p_task, int start_hr, int end_hr, double price_min, double price_max, int assigned_hours[HOURS_PER_DAY], int do_year) {
    int i;
    
    if (do_year) { 
        p_task->min_price += price_min;
        p_task->max_price += price_max;
    }
    else {
        p_task->start_hr = start_hr;
        p_task->end_hr = end_hr;
        p_task->min_price = price_min;
        p_task->max_price = price_max;

        if (!p_task->is_passive) {
            for (i = start_hr; i != end_hr; i++) {
                Wrap_Hour(start_hr + i);
                assigned_hours[i] = 1;
            }
        }
    }
}

/* Run all tests */
void Test_All(void){
    Test_KW();
    Test_Wrap_Hours();
    Test_Day_To_Weekday();
    Test_Fixed_Percent();
}

/* Test the Wrap_hours function */
void Test_Wrap_Hours(void) {
    int hour1 = 22,
        hour2 = 4,
        hour3 = 25,
        expected_hour1 = 22,
        expected_hour2 = 4,
        expected_hour3 = 1;
    
    assert(expected_hour1 == Wrap_Hour(hour1));
    assert(expected_hour2 == Wrap_Hour(hour2));
    assert(expected_hour3 == Wrap_Hour(hour3));
}

/* Test the Day_To_Weekday function */
void Test_Day_To_Weekday(void) {
    int day1 = 30,
        day2 = 600,
        day3 = 45,
        expected_day1 = 4,
        expected_day2 = 0,
        expected_day3 = 5;

    assert(expected_day1 == Day_To_Weekday(day1));
    assert(expected_day2 == Day_To_Weekday(day2));
    assert(expected_day3 == Day_To_Weekday(day3));
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