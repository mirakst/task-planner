/* Navne - Mails? - Gruppe - Software */
/* For plebs  <      > ellers anders røv {   } for the cool kids -=xXx<({[L 3 3 7]})>xXx=- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calculate_prices.h"
#define INPUT_MAX 20
#define HOURS_PR_DAY 24

enum command {cmd_exit, cmd_help, cmd_prices, cmd_unrecognized};
typedef enum command command;

void string_to_lower(char*);
int compare_command(char*);

int main (void) {
    char cmd_input[INPUT_MAX];
    int cmd,
        hour,
        i;
    double prices[HOURS_PR_DAY][2];

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
            
            case cmd_prices:
                Calculate_Prices(prices);
                if (!prices[0][0]) {
                    printf("The sorted hours are:\n");
                    for (i = 0; i < HOURS_PR_DAY; i++)
                        printf("%.2d - %.2d priced at: %.2f DKK/kWh\n", (int) prices[i][1], (int) prices[i][1] + 1, prices[i][0]);
                }
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
    else if (!strcmp(str, "prices"))
        return cmd_prices;
    else
        return cmd_unrecognized;
}