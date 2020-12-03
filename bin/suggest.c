#include "suggest.h"

/** Finds the starting time with the lowest average price over the task duration.
 *  @param[i] user User structure with all user details.
 *  @param[i/o] p_task Pointer to a task structure.
 *  @param[i] p Unsorted array of prices.
 *  @param[i] do_year Boolean to determine whether the yearly or daily savings should be calculated (true = 1, false = 0). */
void Find_Lowest_Price (User user, task *p_task, task task_list[TASK_AMOUNT_MAX], int task_amount, double p[][2], int do_year) {
    int i, j, k, end_hr = 0, avg_hr_start = 0, avg_hr_end = 0, skip_hr = 0;
    double price = 0.0, avg = 0.0, avg_min = 100, avg_max = 0, price_min = 0, price_max = 0;

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
                else {
                    for (k = 0; k < task_amount; k++) {
                        printf("checking task %s\n", task_list[k].name);
                        if (!task_list[k].bool_is_passive && task_list[k].start_hr == j) {
                            if(!strcmp(task_list[k].name, p_task->name) || task_list[k].is_assigned)
                                continue;
                        printf("FUCK %s\n", task_list[k].name);
                        skip_hr = 1;
                        price = 0.0;
                        break;
                        }
                    }
                }
            }
        }
        /* Sørg for at aktive tasks ikke overlapper - måske et occupied array i user som fortæller hvilke timer der er aktive tasks
         * og så tjek for om timen er 1 i availability og 0 i occupied? Hvad er din idé tykke */
        if(!skip_hr) {
            for (j = 0; j < duration; j++)
                price += p[Wrap_Hour(i + j)][0] * p_task->kW;
            
            printf("Price for index p[%d]= %.3f\n", Wrap_Hour(i + j), p[Wrap_Hour(i + j)][0] * p_task->kW);
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
            if (!task_list[i].days[time.tm_wday])
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