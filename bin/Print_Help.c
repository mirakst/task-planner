#include "Help.h"

void Help_All(void) {
    Help_Tasks();
    Help_Prices();
    Help_Settings();
    Help_User_Details();
}

/** Prints a list of helpfull comands for Tasks */
void Help_Tasks(void) {
    Print_Line(0, "");
    printf("%-13s -- Lists all tasks.\n", "List tasks");
    printf("%-13s -- Add a new task.\n", "Tasks add");
    printf("%-13s -- Remove a task\n", "Tasks remove");
    Print_Line(0, "");
}

/** Prints a list of helpfull comands for Prices */
void Help_Prices(void) {
    Print_Line(0, "");
    printf("%-18s -- Lists prices pr hour\n", "List prices");
    printf("%-18s -- Lists the hourly prices, sorted\n", "List prices sorted");
    Print_Line(0, "");
}

/** Prints a list of helpfull comands for Settings */
void Help_Settings(void) {
    Print_Line(0, "");
    printf("%-11s -- ***\n", "Missing command");
    Print_Line(0, "");
}

/** Prints a list of helpfull comands for User_Details */
void Help_User_Details(void) {
    Print_Line(0, "");
    printf("%-11s -- ***\n", "Missing command");
    Print_Line(0, "");
}

/* Prints a list of helpfull comands for Devices
void Help_Devices(void) {
    printf("%-11s -- ***\n", "***");
} */