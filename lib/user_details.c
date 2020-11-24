#include <stdio.h>
#include <stdlib.h>
#include "user_details.h"

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

/* Takes the name of the user */
void Get_Name (User *user, int *bool_name) {

    printf("Please enter your name: ");
    gets(user->user_name);
    printf("Hello %s\n", user->user_name);
    *bool_name = 1;
}

/* Function for resetting the unavailable hours */
void Reset_Available_Hours (int time[][1]) {
    int i;
    for (i = 0; i < 24; i++) {
        time[i][0] = 0;
    }
    printf("Reset complete\n");
}

/* Adds the available times to an array for easy access */
void Get_Available_Hours (int time[][1], int *bool_hours) {
    int i;
    int start_time, end_time;

    printf("Enter an interval where you're unavailable between 0 and 23: ");
    scanf("%d %d", &start_time, &end_time);

    /* For loop setting every hour to 0 on first run */
    if (*bool_hours == 0) {
        for (i = 0; i < 24; i++) {
            time[i][0] = 0;
        }
    }

    /* For loop setting the boolean array for the hours */
    for (i = 0; i < 24; i++) {
        
        /* If the start and end time is the same */
        if (start_time == end_time) {
            time[start_time][0] = 1;
            break;
        }

        /* Sets the boolean to true if i is in the interval of start_time to end_time */
        else if (start_time < end_time) {
            if (i >= start_time && i <= end_time) {
                time[i][0] = 1;
            } 
        }

        /* Wrap time if the interval goes from one day to another */
        else if (start_time > end_time) {
            if ((i >= start_time && i < 24) || (i <= end_time && i >= 0)) {
                time[i][0] = 1;
            }
        }
        printf("Hour %d - %d", i, time[i][0]);
    }

    /* boolean keeping track of whether the hours have been set */
    *bool_hours = 1;
}

void Save_User_Details (User user, int bool_name, int bool_hours) {
    int i = 0;
    FILE *p_File;
    p_File = fopen(FILE_USER_DETAILS, "w");
    
    /* If both a name and unavailable hours have been set the user can save their settings */
    if (bool_hours == 1 && bool_name == 1) {
        fprintf(p_File, "%s\n", user.user_name);

        /* The 24 hours in the day and their boolean value*/
        for (i = 0; i < 24; i++) {
            fprintf(p_File, "%d-%d\n", i, user.available_schedule[i][0]);
        }
        fclose(p_File);
        printf("Finished saving...\n");
    } /* If a name or available hours hasn't been set it shouldn't be able to save */
    else {
        printf("You have to set a name and time of unavailability to save.\n");
    }
}
