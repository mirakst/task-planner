#include "help.h"

/* Startup help */
void Print_Welcome (void) {
    Print_Line(1, "Welcome!");
    printf("This program is designed to help the user with scheduling power-consuming tasks at\n"
           "times that are most environmentally friendly, or cheapest, based on user settings.\n\n"
           "Begin adding a task with the command 'task add'. Enter 'task list' for a list of  \n"
           "all tasks.                                                                        \n\n");
    printf("The user settings include an interval of hours where the user is available (and   \n"
           "can start tasks), and a switch to toggle whether this interval should be ignored  \n"
           "or not when suggesting tasks.                                                     \n"
           "These settings can be changed at any time by entering 'settings'.                 \n");
    printf("Enter 'help' for a detailed list of all commands.                                 \n");
    Print_Line(0, "");
}

/** Calls all the other help functions. */
void Help_All(void) {
    Help_Tasks(0);
    Help_Prices(0);
    Help_Settings(1);
}

/** Prints a list of helpful commands for tasks. 
 * @param b[i] int, if the list should end in a line. */
void Help_Tasks(int b) {
    Print_Line(1, "Tasks");
    printf("%-20s -- Lists all tasks.\n", "task list");
    printf("%-20s -- Add a new task.\n", "task add");
    printf("%-20s -- Remove a task\n", "task remove");
    printf("%-20s -- Edit a task\n", "task edit");
    printf("\n");
    if(b)
        Print_Line(0, "");
}

/** Prints a list of helpful commands for prices. 
 * @param b[i] int, if the list should end in a line. */
void Help_Prices(int b) {
    Print_Line(1, "Prices");
    printf("%-20s -- Lists prices pr hour\n", "List prices");
    printf("%-20s -- Lists the hourly prices, sorted\n", "List prices sorted");
    printf("\n");
    if(b)
        Print_Line(0, "");
}

/** Prints a list of helpful commands for User_Details. 
 * @param b[i] int, if the list should end in a line. */
void Help_Settings(int b) {
    Print_Line(1, "Settings");
    printf("%-20s -- Enter the settings menu.\n", "Settings");
    printf("\n");
    if(b)
        Print_Line(0, "");
}

void Help_Settings_Extended(void) {
    Print_Line(1, "Settings  Detailed");
    printf("1: Display all the current settings\n"
           "2: Change the current name of your profile\n"
           "3: Insert another interval into your schedule\n"
           "4: Reset your entire schedule so every hour is occupied\n"
           "5: Set whether you want tasks recommended outside of your schedule\n"
           "6: Save all your settings for further use\n");
    Print_Line(0, "");
}