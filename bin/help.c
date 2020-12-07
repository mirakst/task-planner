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
}

/** Calls all the other help functions. */
void Help_All(void) {
    Help_Tasks(0);
    Help_Data(0);
    Help_Settings(0);
    Print_Line(1, "General");
    printf("%-20s -- Saves user settings and tasks\n", "Save");
    printf("%-20s -- Exits the program and saves settings and tasks\n", "Exit");
    Print_Line(0, "");
}

/** Prints a list of helpful commands for tasks. 
 * @param b[i] int, if the list should end in a line. */
void Help_Tasks(int b) {
    Print_Line(1, "Tasks");
    printf("%-20s -- Lists all tasks.\n", "Task list");
    printf("%-20s -- Add a new task.\n", "Task add");
    printf("%-20s -- Remove a task\n", "Task remove (ID)");
    printf("%-20s -- Edit a task\n", "Task edit (ID)");
    if(b)
        Print_Line(0, "");
}

/** Prints a list of helpful commands for prices. 
 * @param b[i] int, if the list should end in a line. */
void Help_Data(int b) {
    Print_Line(1, "Data");
    printf("%-20s -- Lists relevant data per hour\n", "List data");
    printf("%-20s -- Suggests when to do your tasks\n", "Suggest");
    printf("%-20s -- Calculates potential savings across a full year\n", "Suggest year");
    if(b)
        Print_Line(0, "");
}

/** Prints a list of helpful commands for User_Details. 
 * @param b[i] int, if the list should end in a line. */
void Help_Settings(int b) {
    Print_Line(1, "Settings");
    printf("%-20s -- Enter the settings menu.\n", "Settings");
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
           "6: Set whether you want recommendations based on prices or emissions\n"
           "7: Save all your settings for further use\n");
    Print_Line(0, "");
}

void Help_Tasks_Extended(void) {
    Print_Line(1, "Tasks  Detailed");
    printf("1: Change the name of the task\n"
           "2: Change the power usage of the task\n"
           "3: Change the duration of the task\n"
           "4: Change the status of the task.\n"
           "Active: The user must be present during the entire task duration.\n"
           "Passive: The user must only be present to start the task.\n"
           "5: Set the days when this task should be done.\n");
    Print_Line(0, "");
}