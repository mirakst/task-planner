#include "help.h"

/** Prints a startup message. 
 *  For better understanding of the program. */
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
    printf("Enter 'help' for a list of all available commands.                                \n");
}

/** Calls Help_Tasks, Help_Data and Help_Settings. */
void Help_All(void) {
    Help_Tasks(FALSE);
    Help_Data(FALSE);
    Help_Settings(FALSE);

    Print_Line(TRUE, "General");
    printf("%-20s -- Saves user settings and tasks\n", "Save");
    printf("%-20s -- Exits the program and saves settings and tasks\n", "Exit");
    Print_Line(FALSE, "");
}

/** Prints a list of helpful commands for tasks. 
 * @param b[i] int, if the list should end in a line. */
void Help_Tasks(int print_end_line) {
    Print_Line(TRUE, "Tasks");
    printf("About:\n");
    printf("A 'task' contains information on its power usage in watts and duration in hours. \n"
           "They also contain a list of which days they should be done.\n\n"
           "Tasks can have various different types:\n");
    printf("Active : The user must be present during the entire task duration.\n"
           "Passive: The user must only be present to start the task.\n"
           "Timed  : The appliance can scheduled to start automatically at a given hour.\n\n");
    printf("Commands:\n");
    printf("%-20s -- Show a lists all tasks.\n", "Task list");
    printf("%-20s -- Create a new task.\n", "Task add");
    printf("%-20s -- Remove an existing task\n", "Task remove (ID)");
    printf("%-20s -- Change settings for a task\n", "Task edit (ID)");
    printf("%-20s -- Get more information on the task settings menu.\n", "Help task edit");
    if(print_end_line)
        Print_Line(FALSE, "");
}

/** Prints a list of helpful commands for data. 
 * @param b[i] int, if the list should end in a line. */
void Help_Data(int print_end_line) {
    Print_Line(TRUE, "Data");
    printf("About:\n");
    printf("The data used is dependent on the user setting. It can either be CO2 emmissions  \n"
            "(g / kWh) or prices (DKK / kWh). This data is used to suggest starting times for\n");
    printf("tasks based on their power consumption, duration and type.\n\n");
    printf("Commands:\n");
    printf("%-20s -- Show a list of relevant data per hour\n", "List data");
    printf("%-20s -- Suggests when to do your tasks\n", "Suggest");
    printf("%-20s -- Calculates potential savings across a full year\n", "Suggest year");
    if(print_end_line)
        Print_Line(FALSE, "");
}

/** Prints a list of helpful commands for User_Details. 
 *  @param b[i] int, if the list should end in a line. */
void Help_Settings(int print_end_line) {
    Print_Line(TRUE, "Settings");
    printf("About:\n");
    printf("The user settings include the username, which data should be used, and a list of \n"
           "user's available hours (Y = available, N = unavailable)\n");
    printf("The data that can be used is either emissions (g / kWh) or prices (DKK / kWh)    \n\n");
    printf("Commands:\n");
    printf("%-20s -- Enter the settings menu.\n\n", "Settings");
    printf("1: Display all the current settings\n"
           "2: Change the current name of your profile\n"
           "3: Insert another interval into your available hours\n"
           "4: Reset all hours so every hour is unavailable\n"
           "5: Set whether to suggest tasks outside of the available hours\n"
           "6: Set whether suggestions should be based on prices or emissions\n");
    if (print_end_line)
        Print_Line(FALSE, "");
}

/** A function that prints an extended help list,
 *  for the function settings. */
void Help_Settings_Extended(void) {
    Print_Line(TRUE, "Settings  Detailed");
    printf("1: Display all the current settings\n"
           "2: Change the current name of your profile\n"
           "3: Insert another interval into your available hours\n"
           "4: Reset all hours so every hour is unavailable\n"
           "5: Set whether to suggest tasks outside of the available hours\n"
           "6: Set whether suggestions should be based on prices or emissions\n");
    Print_Line(FALSE, "");
}

/** A function that prints an extended help list,
 *  for editing a task. */
void Help_Tasks_Extended(void) {
    Print_Line(TRUE, "Tasks  Detailed");
    printf("1: Change the name of the task\n"
           "2: Change the power usage of the task\n"
           "3: Change the duration of the task\n"
           "4: Change the status of the task.\n"
           "Active : The user must be present during the entire task duration.\n"
           "Passive: The user must only be present to start the task.\n"
           "Timed  : The appliance can scheduled to start automatically at a given hour.\n"
           "5: Set the days when this task should be done.\n");
    Print_Line(FALSE, "");
}