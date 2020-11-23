#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INPUT_MAX 20

enum command {cmd_exit, cmd_help, name, hours, save, cmd_unrecognized};
typedef enum command command;

void string_to_lower(char *);
int compare_command(char *);

struct User_Details {
    int available_schedule[24][1];
    char user_name [100];
};
typedef struct User_Details User;

void Get_Name (User *);
void Get_available_Hours (int time[][1]);
void Save_To_File (User User);
//void Make_Timetable (int *time);


int main (void) {
    User user;
    char cmd_input[INPUT_MAX];
    int cmd;
    
    int bool_hours = 0, bool_name = 0;

    do {
        /* Get user input and convert to lower case letters */
        printf("Please enter a command: ");
        scanf(" %s", cmd_input);
        string_to_lower(cmd_input);
        cmd = compare_command(cmd_input);
        
        /* Run the appropriate function based on the command */
        switch (cmd) {
            case cmd_exit:
                printf("Exiting.\n");
                break;
            
            case cmd_help:
                printf("Printing all available commands:\n name\n hours\n save\n exit\n");
                break;

            case name:
                Get_Name (&user);
                printf("Hello %s\n", user.user_name);
                bool_name = 1;
                break;

            case hours:
                Get_available_Hours (user.available_schedule);
                bool_hours = 1;
                break;

            case save:
                if (bool_hours == 1 && bool_name == 1) 
                    Save_To_File(user);
                else 
                    printf("You have to set a name and time of unavailability to save\n");
                break;

            default:
                printf("Command not recognized.\n");
                break;
        }
        
    } while (cmd != cmd_exit);

    return EXIT_SUCCESS;
}

/* string to lower from main.c */
void string_to_lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
            str[i] = tolower(str[i]);
}

/* Command compare function from main.c */
int compare_command (char *str) {
    if (!strcmp(str, "help"))
        return cmd_help;
    else if (!strcmp(str, "exit"))
        return cmd_exit;
    else if (!strcmp(str, "name"))
        return name;
    else if (!strcmp(str, "hours"))
        return hours;
    else if (!strcmp(str, "save"))
        return save;
    else
        return cmd_unrecognized;
}

/* Takes the name of the user */
void Get_Name (User *user) {

    printf("Please enter your name: ");
    scanf("%s", user->user_name);

}

/* Adds the available times to an array for easy access */
void Get_available_Hours (int time[][1]) {
    int i;
    int start_time, end_time;

    printf("Enter an interval where you're unavailable between 0 and 23: ");
    scanf("%d %d", &start_time, &end_time);

    for (i = 0; i < 24; i++) {
        /* Wrap time if the interval goes from one day to another */
        if (start_time >= end_time) {
            if (i >= start_time && i < 24 || i <= end_time && i >= 0) {
                time[i][0] = 1;
            }
            else time[i][0] = 0;
        }
        
        /* Sets the boolean to true if i is in the interval */
        if (start_time <= end_time) {
            if (i >= start_time && i <= end_time) {
                time[i][0] = 1;
            }
            else time[i][0] = 0;
        }
    }
}

void Save_To_File (User user) {
    int i = 0;
    FILE *p_File;

    p_File = fopen("User_Details.txt", "w");
    fprintf(p_File, "%s\n", user.user_name);

    for (i = 0; i < 24; i++) {
        fprintf(p_File, "%d-%d\n", i, user.available_schedule[i][0]);
    }

    fclose(p_File);
    printf("Finished saving...\n");
}