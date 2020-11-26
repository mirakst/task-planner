#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user_details.h"
#include "graphics.h"

void First_Time_Setup (User *user) {
    Print_Line(1, "Starting first setup");
    Get_Name(user);
    Set_Available_Hours(user);
    Print_Line(0, "");
}

/* Takes the name of the user */
void Get_Name (User *user) {
    char temp_string[USERNAME_MAX];

    printf("Please enter your name: ");
    fgets(temp_string, USERNAME_MAX, stdin);
    sscanf(temp_string, "%[^\n]", user->user_name);
    printf("Hello %s\n", user->user_name);
    user->bool_name = 1;
}

/* Function for resetting the unavailable hours, sets every hour to 0 for available */
void Reset_Available_Hours (int time[HOURS_PER_DAY]) {
    int i;
    
    for (i = 0; i < HOURS_PER_DAY; i++)
        time[i] = 0;

    printf("The available hours were succesfully reset.\n");
}

/* Adds the available times to an array for easy access */
void Set_Available_Hours (User *user) {
    int i;
    int start_time = 24, end_time = 24;
    char temp_string[10];

    printf("Enter an interval where you're available between 0 and 23: ");
    fgets(temp_string, 10, stdin);
    sscanf(temp_string, " %d %d", &start_time, &end_time);

    /* For loop setting every hour to 0 on first r un */
    if (!user->bool_hours) {
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
        
        printf("Hour %2d - %d\n", i, user->available_hours[i]); 
    }
    user->bool_hours = 1;
}

/* Prints the input user details */
void Print_Settings (User user) {
    int i;
    
    Print_Line(1, "Settings");
    printf("Username: %s\n", user.user_name);
    printf("Ignore available hours: %s\n\n", user.bool_ignore_hours ? "Yes" : "No");
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

/* Attempts to load user details from file */
int Load_User_Details (User *user) {
    char temp_string[100];
    int i = 0;
    FILE *fp;

    fp = fopen(FILE_USER_DETAILS, "r");
    if (fp == NULL)
        return -1;

    for (i = 0; i < HOURS_PER_DAY + 2; i++) {
        fgets(temp_string, 100, fp);

        if (i == 0) 
            sscanf(temp_string, " %*[^:] %*c %[^\n]", user->user_name);
        else if (i == 1)
            sscanf(temp_string, " %*[^:] %*c %d", &user->bool_ignore_hours);
        else
            sscanf(temp_string, " %*d %*c %d", &user->available_hours[i - 2]);
        }

    fclose(fp);
    return 1;
}

/* If both a name and unavailable hours have been set the user can save their settings */
int Save_User_Details (User user) {
    int i = 0;
    FILE *p_File;

    if (user.bool_hours != 1 && user.bool_name != 1)
        return 0;

    p_File = fopen(FILE_USER_DETAILS, "w");
    if(p_File == NULL)
        return -1;
    printf("%s\n", user.user_name);
    fprintf(p_File, "Username: %s\n", user.user_name);
    fprintf(p_File, "Ignore hours: %d\n", user.bool_ignore_hours);
    
    for (i = 0; i < HOURS_PER_DAY; i++)
        fprintf(p_File, "%d-%d\n", i, user.available_hours[i]);

    fclose(p_File);
    return 1;
}

/* Sets ignore_hours in the user struct according to the given input */
void Set_Ignore_Hours(User *user) {
    char temp[5],
         bool_input;

    printf("Would you like suggestions outside your available hours? (y/n): ");
    fgets(temp, 5, stdin);
    sscanf(temp, " %c", &bool_input);
        
    if (tolower(bool_input) == 'y')
        user->bool_ignore_hours = 1;
    else
        user->bool_ignore_hours = 0;
    printf("Setting have been saved succesfully.\n");
}