#define FILE_USER_DETAILS "config/user_details"
#define USERNAME_MAX 100
#define HOURS_PR_DAY 24

typedef struct User_Details {
    int available_schedule[HOURS_PR_DAY];
    char user_name [USERNAME_MAX];
    int bool_name,
        bool_hours,
        bool_outside_hours;
} User;

void First_Time_Setup (User *);
void Get_Name (User *);
void Reset_Available_Hours (int[HOURS_PR_DAY]);
void Set_Available_Hours (User *);
int Load_User_Details (User *);
int Save_User_Details (User);
void Print_Settings (User);