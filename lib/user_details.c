#include <stdio.h>
#include <stdlib.h>
#include "user_details.h"
#include "graphics.h"

/* Append in main.c
int main (void) {
    User user;
    int bool_hours = 0, bool_name = 0;
}*/

/* Calls to the functions
Get_Name (&user, &bool_name);
Get_available_Hours (user.available_schedule, &bool_hours);
Reset_available_Hours (user.available_schedule);
Save_User_Details(user, bool_name, bool_hours);
*/

void First_Time_Setup (User *user, int *bool_hours, int *bool_name) {
    Print_Line(1, "Starting first setup");
    Get_Name(user, bool_name);
    Set_Available_Hours(user->available_schedule, bool_hours);
    Print_Line(1, "First time setup done");
}

/* Takes the name of the user */
void Get_Name (User *user, int *bool_name) {

    printf("Please enter your name: ");
    gets(user->user_name);
    printf("Hello %s\n", user->user_name);
    *bool_name = 1;
}

/* Function for resetting the unavailable hours, sets every hour to 0 for available */
void Reset_Available_Hours (int time[HOURS_PR_DAY]) {
    int i;
    
    for (i = 0; i < HOURS_PR_DAY; i++)
        time[i] = 0;

    /* printf("Reset complete\n"); */
}

/* Adds the available times to an array for easy access */
void Set_Available_Hours (int time[HOURS_PR_DAY], int *bool_hours) {
    int i;
    int start_time = 24, end_time = 24;
    char temp_string[10];

    printf("Enter an interval where you're available between 0 and 23: ");
    fgets(temp_string, 10, stdin);
    sscanf(temp_string, " %d %d", &start_time, &end_time);

    /* For loop setting every hour to 0 on first run */
    if (!*bool_hours) {
        for (i = 0; i < HOURS_PR_DAY; i++)
            time[i] = 0;
    }

    /* For loop setting the boolean array for the hours */
    for (i = 0; i < HOURS_PR_DAY; i++) {
        
        /* If the start and end time is the same */
        if (start_time == end_time) {
            time[start_time] = 1;
            break;
        }

        /* Sets the boolean to true if i is in the interval of start_time to end_time */
        else if (start_time < end_time) {
            if (i >= start_time && i <= end_time)
                time[i] = 1;
        }

        /* Wrap time if the interval goes from one day to another */
        else if (start_time > end_time) {
            if ((i >= start_time && i < HOURS_PR_DAY) || (i <= end_time && i >= 0))
                time[i] = 1;
        }
        
        printf("Hour %2d - %d\n", i, time[i]); 
    }
}

/* Prints the input user details */
void Print_User_Details (User user) {
    int i;
    
    printf("User: %s\n", user.user_name);

    Print_Line(1, "Availability");

    for (i = 0; i < HOURS_PR_DAY; i++) {
        printf("%2.2d ", i);
    }
    
    printf("\n");
    Print_Line(0, "");

    for (i = 0; i < HOURS_PR_DAY; i++) {
        printf("%2c ", user.available_schedule[i] ? 251 : 120);
    }
    printf("\n");
    Print_Line(0, "");

}

/* Attempts to load user details from file */
int Load_User_Details (User *user) {
    char chunk[100];
    int i = 0;
    FILE *fp;

    fp = fopen(FILE_USER_DETAILS, "r");
    if (fp == NULL)
        return -1;

    for (i = 0; i < HOURS_PR_DAY + 1; i++) {
        fgets(chunk, 100, fp);
        
        if (i == 0)
            sscanf(chunk, " %[^\n]", user->user_name);
        else
            sscanf(chunk, " %*d %*c %d", &user->available_schedule[i - 1]);
    }
    
    fclose(fp);
    return 1;
}

/* If both a name and unavailable hours have been set the user can save their settings */
int Save_User_Details (User user, int bool_name, int bool_hours) {
    int i = 0;
    FILE *p_File;

    if (bool_hours != 1 && bool_name != 1)
        return 0;

    p_File = fopen(FILE_USER_DETAILS, "w");
    if(p_File == NULL)
        return -1;
    
    fprintf(p_File, "%s\n", user.user_name);

    for (i = 0; i < HOURS_PR_DAY; i++) {
        fprintf(p_File, "%d-%d\n", i, user.available_schedule[i]);
    }

    fclose(p_File);
    return 1;
}

