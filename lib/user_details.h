#define FILE_USER_DETAILS "config/user_details"
#define HOURS_PR_DAY 24

typedef struct User_Details {
    int available_schedule[HOURS_PR_DAY];
    char user_name [100];
} User;

void First_Time_Setup (User *, int *, int *);
void Get_Name (User *, int *);
void Reset_Available_Hours (int time[HOURS_PR_DAY]);
void Set_Available_Hours (int time[HOURS_PR_DAY], int *);
int Load_User_Details (User *);
int Save_User_Details (User, int, int);
void Print_User_Details (User user);