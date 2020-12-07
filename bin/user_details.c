#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user_details.h"
#include "graphics.h"
#include "help.h"

/** Performs a setup if there is no settings file.
 * @param[o] The active user struct. */
void First_Time_Setup (User *user) {
    Print_Line(1, "Starting first setup");
    Get_Name(user);
    Set_Available_Hours(user);
    Set_Data_Type(user);
    Print_Line(0, "");
}

/** Lets the user write in their name.
 * @param[o] The active user struct. */
void Get_Name (User *user) {
    char temp_string[USERNAME_MAX];

    printf("Please enter your name: ");
    fgets(temp_string, USERNAME_MAX, stdin);
    sscanf(temp_string, "%[^\n]", user->user_name);
    printf("Hello %s\n", user->user_name);
    user->has_name = 1;
}

/** Reset the available hours.
 * @param[i/o] time - The user's current available hours. */
void Reset_Available_Hours (int time[HOURS_PER_DAY]) {
    int i;
    
    for (i = 0; i < HOURS_PER_DAY; i++)
        time[i] = 0;

    printf("The available hours were succesfully reset.\n");
}

/** Adds available hours to the available hours int array
 *  @param user[i/o] The active user struct */
void Set_Available_Hours (User *user) {
    int i;
    int start_time = 24, end_time = 24;
    char temp_string[10];

    printf("Enter your available hours (0-23): ");
    fgets(temp_string, 10, stdin);
    sscanf(temp_string, " %d %d", &start_time, &end_time);

    if ((start_time > HOURS_PER_DAY || start_time < 0) || (end_time > HOURS_PER_DAY || end_time < 0)) {
        printf("Please enter only valid hours (1-24). Cancelling...\n");
        return;
    }

    /* For loop setting every hour to 0 on first run */
    if (!user->has_set_hours) {
        for (i = 0; i < HOURS_PER_DAY; i++)
            user->available_hours[i] = 0;
    }

    /* For loop setting the boolean array for the hours */
    for (i = 0; i < HOURS_PER_DAY; i++) {
        
        /* If the start and end time is the same */
        if (start_time == end_time) {
            user->available_hours[start_time] = 1;
            break;
        }

        /* Sets the boolean to true if i is in the interval of start_time to end_time */
        else if (start_time < end_time) {
            if (i >= start_time && i <= end_time)
                user->available_hours[i] = 1;
        }

        /* Wrap time if the interval goes from one day to another */
        else if (start_time > end_time) {
            if ((i >= start_time && i < HOURS_PER_DAY) || (i <= end_time && i >= 0))
                user->available_hours[i] = 1;
        }
        
    }
    user->has_set_hours = 1;
}

/** Prints the input user details
 * @param user[i] The active user struct */
void Print_User (User user) {
    int i;
    
    Print_Line(1, "User");
    printf("Username: %s\n", user.user_name);
    printf("Ignore available hours: %s\n", user.ignore_availability ? "Yes" : "No");
    printf("Using data: %s\n\n", user.xXxcumShotxXx == 1 ? "Emissions" : "Prices");
    Print_Line(1, "Availability");

    for (i = 0; i < HOURS_PER_DAY; i++)
        printf("%2.2d ", i);
    
    printf("\n");
    Print_Line(0, "");

    for (i = 0; i < HOURS_PER_DAY; i++)
        printf("%2c ", user.available_hours[i] ? 'Y' : 'n');

    printf("\n");
    Print_Line(0, "");
}

/** Attempts to load user details from file
 * @param user[o] The active user struct 
 * @return -1 if the file was not loaded, otherwise 1 */
int Load_User_Details (User *user) {
    char temp_string[100];
    int i = 0;
    FILE *fp;

    fp = fopen(FILE_USER_DETAILS, "r");
    if (fp == NULL)
        return -1;

    for (i = 0; i < HOURS_PER_DAY + 3; i++) {
        fgets(temp_string, 100, fp);

        if (i == 0) 
            sscanf(temp_string, " %*[^:] %*c %[^\n]", user->user_name);
        else if (i == 1)
            sscanf(temp_string, " %*[^:] %*c %d\n", &user->ignore_availability);
        else if (i == 2)
            sscanf(temp_string, " %*[^:] %*c %d\n", &user->xXxcumShotxXx);
        else
            sscanf(temp_string, " %*d %*c %d\n", &user->available_hours[i - 3]);
    }

    fclose(fp);
    return 1;
}

/** Saves the user's settings to file
 * @param user[i] The active user struct
 * @return -1 if the file could not be saved, 0 if there is nothing to save, and otherwise 1 */
int Save_User_Details (User user) {
    int i = 0;
    FILE *p_File;

    if (user.has_set_hours != 1 && user.has_name != 1)
        return 0;

    p_File = fopen(FILE_USER_DETAILS, "w");
    if(p_File == NULL)
        return -1;
    
    fprintf(p_File, "Username: %s\n", user.user_name);
    fprintf(p_File, "Ignore hours: %d\n", user.ignore_availability);
    fprintf(p_File, "Use emissions: %d\n", user.xXxcumShotxXx);
    
    for (i = 0; i < HOURS_PER_DAY; i++)
        fprintf(p_File, "%d-%d\n", i, user.available_hours[i]);

    fclose(p_File);
    return 1;
}

/** Sets ignore_hours in the user struct according to the given input
 * @param user[o] the active user struct  */
void Set_Ignore_Hours(User *user) {
    char temp[5],
         bool_input;

    printf("Suggest tasks outside your available hours? (y/n): ");
    fgets(temp, 5, stdin);
    sscanf(temp, " %c", &bool_input);
        
    if (tolower(bool_input) == 'y')
        user->ignore_availability = 1;
    else
        user->ignore_availability = 0;
    printf("Settings have been saved succesfully.\n");
}

/** Sets use_emissions in the user struct according to the given input
 * @param user[o] the active user struct  */
void Set_Data_Type(User *user) {
    char temp[5],
         bool_input;

    printf("Suggest tasks based on CO2 emissions? (y/n): ");
    fgets(temp, 5, stdin);
    sscanf(temp, " %c", &bool_input);
        
    if (tolower(bool_input) == 'y')
        user->xXxcumShotxXx = 1;
    else
        user->xXxcumShotxXx = 0;
    printf("Settings have been saved succesfully.\n");
}


/** Prints the settings menu, and awaits user input. 
 * Then runs the function associated with the input.
 * @param user[i/o] The active user struct. */
void Print_Settings(User *user) {
    int input = 0;
    char temp_string[5];

    Print_Line(1, "Settings");
    printf("1: Show current settings\n"
           "2: Set name\n"
           "3: Set available hours\n"
           "4: Reset available hours\n"
           "5: Set ignore hours\n"
           "6: Set data type\n"
           "7: Save user details\n"
           "8: More info\n"
           "0: Go back\n");
    Print_Line(0, "");
    
    do {
        printf("Choose a setting: ");
        fgets(temp_string, 5, stdin);
        sscanf(temp_string, " %d", &input);

        switch (input) {
        case 0:
            Save_User_Details(*user);
            printf("Exiting user settings.\n");
            return;
        case 1:
            Print_User(*user);
            break;
        case 2:
            Get_Name(user);
            break;
        case 3:
            Set_Available_Hours(user);
            break;
        case 4:
            Reset_Available_Hours(user->available_hours);
            break;
        case 5:
            Set_Ignore_Hours(user);
            break;
        case 6:
            Set_Data_Type(user);
            break;
        case 7:
            Save_User_Details(*user);
            printf("Settings have been saved succesfully.\n");
            break;
        case 8:
            Help_Settings_Extended();
            break;
        default:
            printf("The command was not recognized.\n"); 
            break;
        }
    } while (input);
}