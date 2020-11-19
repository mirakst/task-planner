/* TODO: 
    int for start og slut tiderne, sæt dem i et for-loop, og loop tiden efter den 23 time til 0 
    24 bools i et array der siger om timen er åben eller ej
    dialogtræ om redigering af profilen
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INPUT_MAX 20

enum command {cmd_exit, cmd_help, cmd_unrecognized, name, hours};
typedef enum command command;

void string_to_lower(char *);
int compare_command(char *);

void Get_Name (char *);
void Get_available_Hours (unsigned int time[][1]);
//void Make_Timetable (unsigned int *time);



struct User_Details {
    unsigned int available_schedule[24][1];
    char user_name [];
};

int main (void) {
    struct User_Details User;
    char cmd_input[INPUT_MAX];
    int cmd;



    do {
        /* Get user input and convert to lower case letters */
        printf("Please enter a command: ");
        scanf(" %s", cmd_input);
        string_to_lower(cmd_input);
        cmd = compare_command(cmd_input);
        
        printf("[DEBUG]: cmd = %d: %s\n", cmd, cmd_input); /* For debugging */
        
        /* Run the appropriate function based on the command */
        switch (cmd) {
            case cmd_exit:
                printf("Exiting.\n");
                break;
            
            case cmd_help:
                printf("Printing all available commands.\n");
                break;

            case name:
                Get_Name (User.user_name);
                printf("Hello %s\n", User.user_name);
                break;

            case hours:
                Get_available_Hours (User.available_schedule);
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
    else
        return cmd_unrecognized;
}

/* Takes the name of the user */
void Get_Name (char *name) {

    printf("Please enter your name: ");
    scanf("%s", name);

}

/* Adds the available times to an array for easy access */
void Get_available_Hours (unsigned int time[][1]) {
    int i;
    unsigned int start_time, end_time;

    printf("Enter an interval of when you've unavailable between 0 and 23: ");
    scanf("%u %u", &start_time, &end_time);

    for (i = 0; i < 24; i++) {
        if (start_time > end_time) {
            
        }
        
        
        if (i >= start_time && i <= end_time) {
            time[i][1] = 1;
        }
        else time[i][1] = 0;

        printf("%u - %.2d\n", time[i][1], i);
    }

}

/* Creates a visualized timetable based on the available hours */
/*void Make_Timetable (unsigned int *time) {

}*/