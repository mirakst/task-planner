/* 
 * Navne: Admir Kebab, Daniel Berwald, Anders Mariegaard, Nikolaj Dum, Mikkel Stridsen, Torbjørn "ting", Kasper Klam.
 * Mails: sw1a405a@student.aau.dk
 * Gruppe: A405a
 * Studieretning: Software 
 */

/* TODO:
 * FLERE STREGER!
 * Make function "suggest tasks" in another file
 * Make function "help" in another file!
 * Add function for initialization/loading and saving
*/

#include "main.h"

/* Main program function */
int main (int argc, char *argv[]) {
    User user;
    char cmd_input[INPUT_MAX];
    double prices[HOURS_PR_DAY][2],
           prices_sorted[HOURS_PR_DAY][2];
    task task_list[TASK_AMOUNT_MAX]; 
    int debug = 0,
        task_amount = 0, 
        task_id = 0,
        cmd = 0,
        file_status = 0,
        bool_hours = 0,
        bool_name = 0;
    
    /* Check for -debug input argument */
    if (argc == 2 && (strcmp(argv[1], "-debug") == 0))
        debug = 1;

    /* Initialization */
    file_status = Calculate_Prices (prices, 0);
    if(file_status == -1) {
        printf("Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Calculate_Prices (prices_sorted, 1);

    /* Attempt to load user details. Starts setup if there is no file */
    file_status = Load_User_Details(&user);
    if(file_status == -1) {
        First_Time_Setup (&user, &bool_hours, &bool_name);
        Save_User_Details(user, bool_name, bool_hours);
    }
    else {
        bool_hours = 1;
        bool_name = 1;
    }

    Initialize_Tasks(task_list, &task_amount);
    file_status = Load_Tasks(task_list, &task_amount);
    if (file_status == 1)
        printf("Loaded %d tasks successfully.\n", task_amount);
    else if (file_status == -1)
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);

    do {
        /* Get user input and convert to lower case letters */
        printf("\nPlease enter a command: ");
        fgets(cmd_input, INPUT_MAX, stdin);
        
        String_To_Lower(cmd_input);
        cmd = Compare_Command(cmd_input);
        
        if(debug)
            printf("[DEBUG]: cmd = %d: %s\n", cmd, cmd_input);
        
        /* Run the appropriate function based on the command */
        switch (cmd) {
            /* Exit program */
            case cmd_exit:
                printf("Exiting.\n");
                break;

            /* Help commands */
            case cmd_help:
                printf("Printing all available commands.\n");
                Print_Help(2, 1);
                break;
            case cmd_help_tasks:
                Print_Help(cmd, 0);
                break;
            case cmd_help_prices:
                Print_Help(cmd, 0);
                break;

            /* User details */
            case cmd_list_settings:
                Print_User_Details(user);
                break;
            case cmd_availability:
                Set_Available_Hours(user.available_schedule, &bool_hours);
                break;
            case cmd_reset_hours:
                Reset_Available_Hours(user.available_schedule);
                break;
            case cmd_save_user_details:
                file_status = Save_User_Details(user, bool_hours, bool_name);
                if (file_status == -1)
                    printf("Failed to save user details.\n");
                else if (file_status == 0)
                    printf("You have to set a name and time of availability to save.\n");
                else
                    printf("Saved user details succesfully.\n");
                break;
                
            /* Prices */
            case cmd_prices:
                List_Prices (prices);
                break;
            case cmd_prices_sorted:
                List_Prices (prices_sorted);
                break;

            /* Tasks */
            case cmd_list_tasks:
                Print_Task_List(task_list, task_amount);
                break;          
            case cmd_task_add:
                Add_Task(task_list, &task_amount);
                break;
            case cmd_task_remove:
                sscanf(cmd_input, " %*s %*s %d", &task_id);
                Remove_Task(task_list, &task_amount, task_id);
                break;

            /* Suggestations */
            case cmd_suggest:
                Suggest_Tasks(user, task_list, task_amount, prices_sorted, prices);
                break;

            default:
                printf("Command not recognized. Type 'help' for the list of commands.\n");
                break;
        }

    } while (cmd != cmd_exit);

    file_status = Save_User_Details(user, bool_name, bool_hours);
    if (file_status == -1)
        printf("Failed to save user details.\n");

    Save_Tasks(task_list, task_amount);

    return EXIT_SUCCESS;
}

int Compare_Command (char *str) {
    if (!strcmp(str, "help\n"))
        return cmd_help;
    if (!strcmp(str, "help tasks\n"))
        return cmd_help_tasks;
    if (!strcmp(str, "help prices\n"))
        return cmd_help_prices;
    if (!strcmp(str, "list prices\n"))
        return cmd_prices;
    if (!strcmp(str, "list prices sorted\n"))
        return cmd_prices_sorted;
    if (!strcmp(str, "list tasks\n"))
        return cmd_list_tasks;
    if (!strcmp(str, "list settings\n"))
        return cmd_list_settings;
    if (!strcmp(str, "set hours\n"))
        return cmd_availability;
    if (!strcmp(str, "task add\n"))
        return cmd_task_add;
    if (!strcmp(str, "reset hours\n"))
        return cmd_reset_hours;
    if (!strcmp(str, "save settings\n"))
        return cmd_save_user_details;
    if (strstr(str, "task remove"))
        return cmd_task_remove;
    if (!strcmp(str, "suggest\n"))
        return cmd_suggest;
    if (!strcmp(str, "exit\n"))
        return cmd_exit;

    return cmd_unrecognized;
}

/* Suggest a task */
void Suggest_Tasks (User user, task *task_list, int task_amount, double prices_sorted[HOURS_PR_DAY][2], double prices[HOURS_PR_DAY][2]) {
    int i, j, unsorted_index = 0;
    double total_task_price = 0.0, task_price_hr = 0.0, task_hours = 0;

    Print_Line(1, "Suggestions list");
    printf("%-20s%16s%16s\n", "Task", "Starting time", "Total price");

    for (i = 0; i < task_amount; i++) {
        task_hours = (double)task_list[i].duration / 60.0;
        unsorted_index = Get_Index_Unsorted(i, prices_sorted, prices);

        printf("index: %d, user availability: %d\n", unsorted_index, user.available_schedule[unsorted_index]);
        while (user.available_schedule[unsorted_index] != 1) {
            unsorted_index++;
        }

        task_price_hr = prices_sorted[i][0];

        for (j = 1; j < task_hours; j++) {
            task_price_hr += prices[unsorted_index + j][0];
            if(unsorted_index + j >= 23)
                unsorted_index = 0;
        }

        total_task_price = task_price_hr * ((double)task_list[i].power / 1000.0);
        printf("%-20s %15.2d %11.3f DKK\n", task_list[i].name, (int) prices_sorted[i][1], total_task_price);
    }
    Print_Line(0,"");
}

/* Takes an index from the sorted array and returns its index in the unsorted array */
int Get_Index_Unsorted (int sorted_index, double prices_sorted[HOURS_PR_DAY][2], double prices[HOURS_PR_DAY][2]) {
    int i;
    double target_hour = prices_sorted[sorted_index][1];

    for (i = 0; i < HOURS_PR_DAY; i++) {
        if(prices[i][1] == target_hour)
            return i;
    }
    return 0;
}

/* Prints available commands based on the input */
void Print_Help (int cmd, int print_all) {
    Print_Line(1, "Commands");
    switch (cmd) {
    case cmd_help_tasks:
        printf("%-13s -- Lists all tasks.\n", "List tasks");
        printf("%-13s -- Add a new task.\n", "Tasks add");
        printf("%-13s -- Remove a task\n", "Tasks remove");
        if(!print_all)
            break;
    case cmd_help_prices:
        printf("%-18s -- Lists prices pr hour\n", "List prices");
        printf("%-18s -- Lists the hourly prices, sorted\n", "List prices sorted");
        if(!print_all)
            break;
    }
    Print_Line(0,"");
}