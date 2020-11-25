/* 
 * Navne: Admir Bebab, Daniel Berwald, Anders Mariegaard, Nikolaj Dam, Mikkel Stridsen, Torbjørn "ting", Kasper Klam.
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
    int task_amount = 0, 
        task_id = 0;

    user.bool_outside_hours = 0;

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
            Reset_Available_Hours(user.available_schedule);
        else if (!strcmp(cmd_input, SAVE_USER)) 
            Save(user, task_list, task_amount);

        /* Prices */
        else if (!strcmp(cmd_input, LIST_PRICES))
            List_Prices (prices);
        else if (!strcmp(cmd_input, LIST_PRICES_SORTED))
            List_Prices (prices_sorted);
        
        /* Tasks */
        else if (!strcmp(cmd_input, LIST_TASKS))
            Print_Task_List(task_list, task_amount);
        else if (!strcmp(cmd_input, SUGGEST))
            Suggest_Tasks(user, task_list, task_amount, prices_sorted, prices);
        else if (!strcmp(cmd_input, ADD_TASK))
            Add_Task(task_list, &task_amount);
        else if (strstr(cmd_input, REMOVE_TASK)) {
            if(sscanf(cmd_input, " %*s %*s %d", &task_id) == 1)
                Remove_Task(task_list, &task_amount, task_id);
            else
                printf("Task ID not specified.\n") ;
        }
        else
            printf("Command not recognized. Type 'help' for the list of commands.\n");

    } while (!(!strcmp(cmd_input, EXIT)));

    Save(user, task_list, task_amount);

    return EXIT_SUCCESS;
}

/* Performs all initialization and loading of structs and variables */
void Initialize (double prices[][2], double prices_sorted[][2], User *user, task task_list[TASK_AMOUNT_MAX], int *task_amount) {
    int file_status = 0;

    file_status = Calculate_Prices (prices, 0);
    if(file_status == -1) {
        printf("Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Calculate_Prices (prices_sorted, 1);

    /* Attempt to load user details. Starts setup if there is no file */
    user->bool_hours = 0;
    user->bool_name = 0;

    file_status = Load_User_Details(user);
    if(file_status == -1) {
        First_Time_Setup (user);
        Save_User_Details(*user);
    }

    Initialize_Tasks(task_list, task_amount);
    file_status = Load_Tasks(task_list, task_amount);
    if (file_status == 1)
        printf("Loaded %d tasks successfully.\n", *task_amount);
    else if (file_status == -1)
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);
}

/* Saves the user details and task list */
void Save (User user, task task_list[TASK_AMOUNT_MAX], int task_amount) {
    if (Save_User_Details(user) == -1) {
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
void Suggest_Tasks (User user, task *task_list, int task_amount, double prices_sorted[][2], double prices[][2]) {
    int i, j, task_id = 0, hour, a, b, penis, should_continue = 1;
    double task_hours = 0.0, task_price = 0.0, 
           price = 0.0, price_max = 0.0;

    Print_Line(1,"Suggest tasks");
    printf("%-30s %s %20s\n", "Name", "Hours", "Savings");

    /* TODO: 
     * Alle de PASSIVE tasks skal bare smides i den billigste time 
     * (indenfor user availability hvis det er gældende!)
     * 
     * Alle de AKTIVE tasks skal så assignes som før, men ikke oven i de timer
     * hvor der er passive tasks.
     */

    /* FINGRENDE VÆK!!! */
    for (i = 0; i < HOURS_PR_DAY; i++) {
        if(task_id == task_amount)
            break;

        should_continue = 1;
        task_hours = (double) task_list[task_id].duration / 60;
        task_price = 0.0;

        /* Check availability for all hours between the start and end hours */
        if(!user.bool_outside_hours) {
            a = (int)prices_sorted[i][1];
            b = (int)prices_sorted[i][1] + (int)task_hours;
            for (penis = a; penis <= b; penis++)   
                if (user.available_schedule[penis] == 0)
                    should_continue = 0;      
        }
        
        /* If all hours between the start and end hours are available, assign task */
        if (should_continue) {
            
            for (j = 0; j < task_hours; j++) {
                hour = i;
                if ((int) prices_sorted[i][1] + j >= 23)
                    hour = 0 - j;

                price = prices[(int) prices_sorted[hour][1] + j][0];
               /*DEBUG printf("Price at hour %d = %.3f\n", (int) prices_sorted[hour][1] + j, price); */
                if (task_hours > j && task_hours < (j + 1)) {
                    task_price += (task_hours - j) * price * ((double) task_list[task_id].power / 1000);
                    /*printf("Added remainder %.3f to price \n", task_rem * price * ((double) task_list[task_id].power / 1000));*/
                } 
                else {
                    task_price += ((double) task_list[task_id].power / 1000) * price;
                    /*printf("Added %.3f to price\n", ((double) task_list[task_id].power / 1000) * price);*/
                 /*DEBUG   printf("Added %.3f to result\n", ((double) task_list[task_id].power / 1000) * price); */
                }
            }
            
            price_max = Calculate_Max_Price(prices_sorted, prices, task_hours, task_list[task_id]);

            printf("%-30s %2.2d-%2.2d %19.2f%% \n", 
                    task_list[task_id].name,
                    (int)prices[(int) prices_sorted[hour][1]][1], 
                    (int)(prices[(int) prices_sorted[hour][1]][1] + task_hours),
                    (((price_max / task_price) - 1) * 100.0));
            task_id++;
        }
    }

    Print_Line(0,"");
}

/* Assumes that the hour 17 is the start of the highest peak, and calculates the price from there */
double Calculate_Max_Price (double prices_sorted[][2], double prices[][2], double task_hours, task task) {
    int j = 0, i, hour = 17;
    double result = 0.0, price = 0.0;
    
    for (i = hour; j < task_hours; i++) {
        if (i > 23)
            i = 0;

        price = prices[i][0];

        if (task_hours > j && task_hours < (j + 1))
            result += (task_hours - j) * price * ((double) task.power / 1000);
        else
            result += ((double) task.power / 1000) * price;
        j++;
    }
    return result;
}

