#define FILE_USER_DETAILS "config/settings"
#define USERNAME_MAX 50
#define HOURS_PER_DAY 24
#define SETTINGS_MAX 30

typedef struct User_Details {
    int available_hours[HOURS_PER_DAY];
    char user_name [USERNAME_MAX];
    int bool_name,
        bool_hours,
        bool_ignore_hours;
} User;

void First_Time_Setup (User *);
void Get_Name (User *);
void Reset_Available_Hours (int[HOURS_PER_DAY]);
void Set_Available_Hours (User *);
int Load_User_Details (User *);
int Save_User_Details (User);
void Print_Settings (User);
void Set_Ignore_Hours(User *);
