#include "help.h"

void Help_All(void) {
    Help_Tasks(0);
    Help_Prices(0);
    Help_User_Details(1);
}

/* Prints a list of helpful commands for tasks */
void Help_Tasks(int b) {
    Print_Line(1, "Tasks");
    printf("%-20s -- Lists all tasks.\n", "List tasks");
    printf("%-20s -- Add a new task.\n", "Tasks add");
    printf("%-20s -- Remove a task\n", "Tasks remove");
    printf("\n");
    if(b)
        Print_Line(0, "");
}

/* Prints a list of helpful commands for prices */
void Help_Prices(int b) {
    Print_Line(1, "Prices");
    printf("%-20s -- Lists prices pr hour\n", "List prices");
    printf("%-20s -- Lists the hourly prices, sorted\n", "List prices sorted");
    /* printf("%-20s -- Plz insert the D:\n", "Missing D"); */
    printf("\n");
    if(b)
        Print_Line(0, "");
}

/* Prints a list of helpful commands for User_Details */
void Help_User_Details(int b) {
    Print_Line(1, "User details");
    printf("%-20s -- Changes the name in the user details.\n", "Set name");
    printf("%-20s -- Changes the available hours in user details.\n", "Set hours");
    printf("%-20s -- Resets the schedule.\n", "Reset hours");
    printf("%-20s -- Saves the the user details to a file.\n", "Save");
    printf("%-20s -- Enables recommendation outside available hours\n", "Recommend");
    printf("\n");
    if(b)
        Print_Line(0, "");
}
    