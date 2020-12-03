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
        cmd,
        price_day;

    Test_All();
    Initialize(prices, prices_sorted, &user, task_list, time, &task_amount, &price_day);

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
            List_Prices(prices); break;
        case list_prices_sorted:
            List_Prices(prices_sorted); break;
        case change_day:
            Change_Day(prices, &price_day); break;
        case list_tasks:
            Print_Task_List(task_list, task_amount, time); break;
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
            Suggest_Day (user, task_list, task_amount, prices, price_day, time); break;
        case suggest_year:
            Suggest_Year (user, task_list, task_amount, prices, price_day, time); break;
            
        default:
            printf("Command not recognized. Type 'help for the list of commands.\n"); break;
        }
    } while (cmd != _exit);

    Save(user, task_list, task_amount);

    return EXIT_SUCCESS;
}

/** Performs all necessary initialization and loading of structs and variables.
 *  @param p[i/o] Unsorted array of prices.
 *  @param p_sorted[i/o] Sorted array of prices.
 *  @param user[i/o] Pointer to the user structure with all user details.
 *  @param time[i]  Structure containing a calendar date and time broken down into its components.
 *  @param task_list[i/o]  Active array of tasks. 
 *  @param task_amount[i/o] Amount of non-empty tasks in the task array. */
void Initialize(double p[][2], double p_sorted[][2], User *user, task task_list[TASK_AMOUNT_MAX], struct tm time, int *task_amount, int *price_day) {
    int file_status = 0,
        i;
    char *weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    *price_day = time.tm_yday;
    /* Attempts to load price file. Terminates if there is none */
    file_status = Calculate_Prices(p, 0, *price_day);
    if (file_status == -1) {
        printf("Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Calculate_Prices(p_sorted, 1, *price_day);

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

void Suggest_Day (User user, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], int price_day, struct tm time) {
    int i;
    for (i = 0; i < task_amount; i++) {
        task_list[i].max_price = 0;
        task_list[i].min_price = 0;
        Find_Lowest_Price (user, &task_list[i], task_list, task_amount, p, 0);  
    }
    Print_Suggestions_Day (task_amount, task_list, time);
}

/** Suggest a year! 
 *  @param user RØVsnapS */
void Suggest_Year (User user, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], int price_day, struct tm time) {
    int i, day;
    for (i = 0; i < task_amount; i++) {
        task_list[i].max_price = 0;
        task_list[i].min_price = 0;
    }

    for (day = 1; day <= DAYS_PER_YEAR; day++) {
        Calculate_Prices (p, 0, day);
        for (i = 0; i < task_amount; i++) {
            if (!task_list[i].days[Day_To_Weekday(day)])
                continue;
            Find_Lowest_Price (user, &task_list[i], task_list, task_amount, p, 1);
            task_list[i].total_days_yr++;
        }
    }
    Print_Suggestions_Year (task_amount, task_list);
    Calculate_Prices (p, 0, price_day); 
}

/** Finds the starting time with the lowest average price over the task duration.
 *  @param user[i] User structure with all user details.
 *  @param p_task[i/o] Pointer to a task structure.
 *  @param task_list[i] Array of task structures.
 *  @param task_amount[i] Amount of task structures in the task array.
 *  @param p[i] Unsorted array of prices.
 *  @param do_year[i] Boolean to determine whether the yearly or daily savings should be calculated (true = 1, false = 0). */
void Find_Lowest_Price (User user, task *p_task, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], int do_year) {
    int i, j, k, q, end_hr = 0, avg_hr_start = 0, avg_hr_end = 0, skip_hr = 0;
    double price = 0.0, avg = 0.0, avg_min = 100, avg_max = 0, price_min = 0, price_max = 0;
    int duration = (int) ceil(p_task->duration);
    
    p_task->is_assigned = 0;

    for (i = 0; i < HOURS_PER_DAY; i++) {
        end_hr = Wrap_Hour(i + duration);
        skip_hr = 0;

        if (!user.is_available)
            if (!user.available_hours[i])
                continue;

        if (!p_task->is_passive) {
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
                        if (!strcmp(task_list[k].name, p_task->name) || !task_list[k].is_assigned || task_list[k].is_passive)
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

/* Run all tezts FU*/
void Test_All(void){
    Test_KW();
    Test_Wrap_Houreeeees();
    Test_Day_To_Weekday();
    Test_Fixed_Percent();
}

void Test_Wrap_Houreeeees(void) {
    int hour1 = 22,
        hour2 = 4,
        hour3 = 25,
        hour4 = 12,
        hour5 = 36,
        expected_hour1 = 22,
        expected_hour2 = 4,
        expected_hour3 = 1,
        expected_hour4 = 12,
        expected_hour5 = 12;
    
    assert(expected_hour1 == Wrap_Hour(hour1));
    assert(expected_hour2 == Wrap_Hour(hour2));
    assert(expected_hour3 == Wrap_Hour(hour3));
    assert(expected_hour4 == Wrap_Hour(hour4));
    assert(expected_hour5 == Wrap_Hour(hour5));
}

void Test_Day_To_Weekday(void) {
    int day1 = 30,
        day2 = 600,
        day3 = 45,
        day4 = 143,
        day5 = 262,
        expected_day1 = 5,
        expected_day2 = 1,
        expected_day3 = 6,
        expected_day4 = 6,
        expected_day5 = 6;

    assert(expected_day1 == Day_To_Weekday(day1));
    assert(expected_day2 == Day_To_Weekday(day2));
    assert(expected_day3 == Day_To_Weekday(day3));
    assert(expected_day4 == Day_To_Weekday(day4));
    assert(expected_day5 == Day_To_Weekday(day5));
}

void Test_Fixed_Percent(void) {
    double min[5] = {11, 55, 33, 98, 45},
           max[5] = {65, 120, 85, 464, 97};
    int expected[5] = {83, 54, 61, 78, 53};

    assert(expected[0] == (int)Fixed_Percent(max[0], min[0]));
    assert(expected[1] == (int)Fixed_Percent(max[1], min[1]));
    assert(expected[2] == (int)Fixed_Percent(max[2], min[2]));
    assert(expected[3] == (int)Fixed_Percent(max[3], min[3]));
    assert(expected[4] == (int)Fixed_Percent(max[4], min[4]));
}