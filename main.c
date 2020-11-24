/* Navne - Mails? - Gruppe - Software */
/* For plebs  <      > ellers anders røv {   } for the cool kids -=xXx<({[L 3 3 7]})>xXx=- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INPUT_MAX 20

enum command {cmd_exit, cmd_help, cmd_unrecognized};
typedef enum command command;

void string_to_lower(char*);
int compare_command(char*);

int main (void) {
    char cmd_input[INPUT_MAX];
    int cmd;

    /* Check for config file here */

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

            default:
                printf("Command not recognized. Type 'help' for the list of commands.\n");
                break;
        }

    } while (cmd != cmd_exit);

    return EXIT_SUCCESS;
}

void string_to_lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
            str[i] = tolower(str[i]);
}

int compare_command (char *str) {
    if (!strcmp(str, "help"))
        return cmd_help;
    else if (!strcmp(str, "exit"))
        return cmd_exit;
    else
        return cmd_unrecognized;
}
