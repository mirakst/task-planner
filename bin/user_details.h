#define FILE_USER_DETAILS "config/settings"
#define USERNAME_MAX 50
#define HOURS_PER_DAY 24
#define SETTINGS_MAX 30

/** The user struct.
 * Contains all user settings.
 * @param available_hours[] Array of ints that keep track of user availability for all hours of a day (1 = available, 0 = unavailable).
 * @param user_name String with the name of the user.
 * @param has_name Boolean that determines if a user name has been set (1 = true, 0 = false).
 * @param has_set_hours Boolean that determines if available hours has been set (1 = true, 0 = false).
 * @param is_available Boolean that determines whether the available hours should be ignored when running the Find_Lowest_Prices() function. */
typedef struct User_Details {
    int available_hours[HOURS_PER_DAY];
    char user_name [USERNAME_MAX];
    int has_name,
        has_set_hours,
        is_available;
} User;

void First_Time_Setup (User *);
void Get_Name (User *);
void Reset_Available_Hours (int[HOURS_PER_DAY]);
void Set_Available_Hours (User *);
void Print_User (User);
int Load_User_Details (User *);
int Save_User_Details (User);
void Set_Ignore_Hours (User *);
void Print_Settings (User *);