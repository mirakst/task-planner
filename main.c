/* 
 * Navne: Admir Kebab, Daniel Berwald, Anders Mariegaard, Nikolaj Dum, Mikkel Stridsen, Torbjørn "ting", Kasper Klam.
 * Mails: sw1a405a@student.aau.dk
 * Gruppe: A405a
 * Studieretning: Software 
 */

/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/* Custom libraries */
#include "tasks.h"
#include "calculate_prices.h"

#define INPUT_MAX 40

typedef enum command {cmd_exit, cmd_help, cmd_help_tasks, 
                      cmd_help_prices, cmd_prices, cmd_prices_sorted, cmd_list_tasks, 
                      cmd_task_add, cmd_task_remove, cmd_suggest, cmd_unrecognized} command;

void String_To_Lower(char*);
int Compare_Command(char*);
void Print_Help (int, int);
void suggest_Tasks (task*, int, double[HOURS_PR_DAY][2], double[HOURS_PR_DAY][2]);
int Get_Index_Unsorted (int, double[HOURS_PR_DAY][2], double[HOURS_PR_DAY][2]);

int main (void) {
    char cmd_input[INPUT_MAX];
    double prices[HOURS_PR_DAY][2],
           prices_sorted[HOURS_PR_DAY][2];
    task task_list[TASK_AMOUNT_MAX]; 
    int task_amount = 0, 
        task_id = 0,
        cmd = 0,
        i,
        file_status = 0;
    
    /* Initialization */
    Initialize_Tasks(task_list, &task_amount);
    file_status = Load_Tasks(task_list, &task_amount);                           
    if (file_status == 1)                                                        
        printf("Loaded %d tasks successfully.\n", task_amount);                   
    else if (file_status == -1)                                                  
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);  
    Calculate_Prices (prices, 0);
    Calculate_Prices (prices_sorted, 1);

    do {
        /* Get user input and convert to lower case letters */
        printf("\nPlease enter a command: ");
        fgets(cmd_input, INPUT_MAX, stdin);
        
        String_To_Lower(cmd_input);
        cmd = Compare_Command(cmd_input);
        
        printf("[DEBUG]: cmd = %d: %s\n", cmd, cmd_input); /* For debugging */
        
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
                suggest_Tasks(task_list, task_amount, prices_sorted, prices);
                break;

            default:
                printf("Command not recognized. Type 'help' for the list of commands.\n");
                break;
        }

    } while (cmd != cmd_exit);

    Save_Tasks(task_list, task_amount);
    return EXIT_SUCCESS;
}

void String_To_Lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
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
    if (!strcmp(str, "task add\n"))
        return cmd_task_add;
    if (strstr(str, "task remove"))
        return cmd_task_remove;
    if (!strcmp(str, "suggest\n"))
        return cmd_suggest;
    if (!strcmp(str, "exit\n"))
        return cmd_exit;

    return cmd_unrecognized;
}

/* suggest a task */
void suggest_Tasks (task *task_list, int task_amount, double prices_sorted[HOURS_PR_DAY][2], double prices[HOURS_PR_DAY][2]) {
    int i, j, unsorted_index = 0;
    double total_task_price = 0.0, task_price_hr = 0.0, task_hours = 0;

    printf("------------------------- Suggestions ---------------------------\n");
    printf("%s%10s%10s", "Task", "***", "Total price")

    /* For 1 hr pr task */
    for (i = 0; i < task_amount; i++) {
        task_hours = (double)task_list[i].duration / 60.0;
        printf("task_hours: %lf\n", task_hours);

        task_price_hr = prices_sorted[i][0];
        unsorted_index = Get_Index_Unsorted(i, prices_sorted, prices);

        for (j = 1; j < task_hours; j++) {
            task_price_hr += prices[unsorted_index + j][0];
            if(unsorted_index + j >= 23)
                unsorted_index = 0;
        }

        total_task_price = task_price_hr * ((double)task_list[i].power / 1000.0);
        printf("%20s: %.0lf %.3lf", task_list[i].name, prices_sorted[i][1], total_task_price);
    }
    printf("----------------------------------------------------------------\n");
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
    printf("-------------------------- Commands ----------------------------\n");
    switch (cmd) {
    case cmd_help_tasks:
        printf("%-13s -- Lists all tasks.\n", "List tasks");
        printf("%-13s -- Add a new task.\n", "Tasks add");
        printf("%-13s -- Remove a task\n", "Tasks remove");
        if(!print_all)
            break;
    case cmd_help_prices:
        printf("%-13s -- Lists prices pr hour\n", "List prices");
        if(!print_all)
            break;
    }
    printf("----------------------------------------------------------------\n");
}